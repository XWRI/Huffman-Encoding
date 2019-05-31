#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <vector>
#include <utility>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator < (const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies

    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode* left() { return left_; }
  HuffmanNode* right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  // Count the frequency of each character in the input file
  static std::vector<size_t> CountInputFreq(std::ifstream &ifs,
                                            std::vector<char> &vec_input_file);

  // Build the huffman tree
  static PQueue<HuffmanNode*, MyClassPtrCompMin<HuffmanNode*>>
                                         BuildTree(std::vector<size_t>& input);

  // Output the huffman tree
  static void OutputTree(PQueue<HuffmanNode*,
                         MyClassPtrCompMin<HuffmanNode*>>& pq,
                         BinaryOutputStream& output);

  // Preorder recursive method
  static void PreOrder(HuffmanNode& n, BinaryOutputStream& output);

  // Output the number of encoded characeters
  static void OutputNumChar(std::vector<size_t>& input,
                            BinaryOutputStream& output);

  // Build the coding table
  static std::vector<std::string> BuildTable(HuffmanNode& n);

  // Helper method
  static void HelperBuildTable(HuffmanNode& n,
                               std::vector<std::string>& code_table,
                               std::vector<char>& encoding);

  // Output the sequence of encoded characters
  static void OutputChar(std::vector<std::string>& code_table,
                         BinaryOutputStream& output,
                         std::vector<char>& vec_input_file);

  // Recreate the tree from the binary input
  static HuffmanNode* ReBuildTree(BinaryInputStream& input_stream);

  // Read the characters from the encoded binary strings
  static void ReEncodeString(PQueue<HuffmanNode*,
                             MyClassPtrCompMin<HuffmanNode*>> &pq,
                             BinaryInputStream& input,
                             BinaryOutputStream& output);
};

//  Objective: Count the frequency of each character in the input file
//             and store the input into a vector
std::vector<size_t> Huffman::CountInputFreq(std::ifstream &ifs,
                                            std::vector<char>& vec_input_file) {
  // Create a vector of 128 spaces filled with 0 to store the frequency
  std::vector<size_t> vec_char_freq(128, 0);
  char cur_char;

  // Find the index of the character and count up the value at that index
  while (ifs >> std::noskipws >> cur_char) {
    vec_input_file.push_back(cur_char);
    vec_char_freq[cur_char]++;  // cur_char is int automatically
  }

  return vec_char_freq;
}

// Objective: Push all characters into the priority queue
//            and adujst into a huffman tree with only one element in the queue
PQueue<HuffmanNode*, MyClassPtrCompMin<HuffmanNode*>>
                              Huffman::BuildTree(std::vector<size_t>& input) {
  PQueue<HuffmanNode*, MyClassPtrCompMin<HuffmanNode*>> pq;
  // Create a HuffmanNode for each character and push them into the queue
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] != 0) {
      HuffmanNode* cur_node = new HuffmanNode(i, input[i], nullptr, nullptr);
      pq.Push(cur_node);
    }
  }

  // Adjust the tree to contain only one item
  while (pq.Size() > 1) {
    HuffmanNode* left_node = pq.Top();  // Pop the first to be the left
    pq.Pop();
    HuffmanNode* right_node = pq.Top();  // Pop the second to the right
    pq.Pop();
    int freq = left_node->freq() + right_node->freq();
    HuffmanNode* new_node = new HuffmanNode(0, freq, left_node, right_node);
    pq.Push(new_node);  // Push back in the new node
  }

  return pq;
}

// Helper method for preorder traverse
void Huffman::PreOrder(HuffmanNode& n, BinaryOutputStream& output) {
  if (n.data() == 0) {
    output.PutBit(0);  // Write 0 to the file when no character present
  } else {
    output.PutBit(1);  // Write 1 to the file when there is character
    output.PutChar(n.data());
  }

  // check terminating condition
  if (!n.left()) return;
  // if fine, recursion to left
  PreOrder(*(n.left()), output);

  // check terminating condition
  if (!n.right()) return;
  // if fine, recursion to right
  PreOrder(*(n.right()), output);
}

// Objective: Write the contents of the tree to the zap file
void Huffman::OutputTree(PQueue<HuffmanNode*,
                         MyClassPtrCompMin<HuffmanNode*>>& pq,
                         BinaryOutputStream& output) {
  PreOrder(*(pq.Top()), output);
}

// Objective: Write the number of characters to the zap file
void Huffman::OutputNumChar(std::vector<size_t>& input,
                            BinaryOutputStream& output) {
  int num_total = 0;
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] != 0)
      num_total += input[i];
  }
  output.PutInt(num_total);
}

// Objective: Get the string encoding corresponding to each character
std::vector<std::string> Huffman::BuildTable(HuffmanNode& n) {
  // Create a vector of 128 empty string to fill the encoding string
  std::vector<std::string> code_table(128);
  std::vector<char> encoding;  // A vector of char to store the current encoding

  HelperBuildTable(n, code_table, encoding);
  return code_table;
}

// Objective: Recursive helper method to traverse through the huffman tree
//            to obtain the encoding
void Huffman::HelperBuildTable(HuffmanNode& n,
                               std::vector<std::string>& code_table,
                               std::vector<char>& encoding) {
  if (n.data() != 0) {
    std::string temp;

    //  Copy the current state of encoding string to temp
    for (auto itr : encoding)
      temp.push_back(itr);

    code_table[n.data()] = temp;  // Assign temp to the corresponding character
    encoding.pop_back();  // Pop when reaching a leaf node
    return;
  }

  encoding.push_back(48);  // Push 0 in if goes left
  HelperBuildTable(*(n.left()), code_table, encoding);

  encoding.push_back(49);  // Push 1 in if goes right
  HelperBuildTable(*(n.right()), code_table, encoding);

  if (!encoding.empty())
  encoding.pop_back();  // Pop when we already go through two children
}
// Objective: Write the input characters as encoded strings in sequence
void Huffman::OutputChar(std::vector<std::string>& code_table,
                         BinaryOutputStream& output,
                         std::vector<char>& vec_input_file) {
  char cur_char;
  // Iterate through every character
  for (size_t i = 0; i < vec_input_file.size(); i++) {
    cur_char = vec_input_file[i];
    // Iterate through the encoded string bit by bit
    for (size_t j = 0; j  < code_table[cur_char].size(); j++) {
      if (code_table[cur_char][j] == 48)  // Check if the bit is 0
        output.PutBit(0);
      else                               // The bit is else 1
        output.PutBit(1);
    }
  }
}


void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  std::vector<char> vec_input_file;

  std::vector<size_t> vec_char_freq = CountInputFreq(ifs, vec_input_file);
  PQueue<HuffmanNode*, MyClassPtrCompMin<HuffmanNode*>> input_pq =
                                                 BuildTree(vec_char_freq);
  std::vector<std::string> code_table = BuildTable(*(input_pq.Top()));

  BinaryOutputStream output_tree(ofs);

  OutputTree(input_pq, output_tree);
  OutputNumChar(vec_char_freq, output_tree);
  OutputChar(code_table, output_tree, vec_input_file);
}

// Objective: Recursive recreate the huffman tree
HuffmanNode* Huffman::ReBuildTree(BinaryInputStream& input_stream) {
  int cur_bit = input_stream.GetBit();
  // Create a HuffmanNode with char based on the input binary string
  if (cur_bit == 1) {
    HuffmanNode* char_node_ = new HuffmanNode(input_stream.GetChar(), 0);
    return char_node_;
  }

  // Initilize the left and right node
  HuffmanNode* left_subtree = new HuffmanNode(0, 0);
  HuffmanNode* right_subtree = new HuffmanNode(0, 0);

  left_subtree = std::move(ReBuildTree(input_stream));
  right_subtree = std::move(ReBuildTree(input_stream));

  // Build the parent node
  HuffmanNode* cur_node_ = new HuffmanNode(0, 0, left_subtree, right_subtree);
  return cur_node_;
}

// Objective: Write the char to unzap file in the correct sequence
void Huffman::ReEncodeString(PQueue<HuffmanNode*,
                             MyClassPtrCompMin<HuffmanNode*>> &pq,
                             BinaryInputStream& input,
                             BinaryOutputStream& output) {
  int num_char = input.GetInt();  // Get the number of char input
  bool cur_bit;

  for (int i = 0; i < num_char; i++) {
    HuffmanNode* n = pq.Top();
    while (n->data() == 0) {
      cur_bit = input.GetBit();
      // Go to right if the bit is 1
      if (cur_bit)
        n = std::move(n->right());
      // Go to left if the bit is 0
      else
        n = std::move(n->left());
    }
    output.PutChar(n->data());
  }
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  BinaryInputStream input_stream(ifs);
  BinaryOutputStream output_stream(ofs);

  PQueue<HuffmanNode*, MyClassPtrCompMin<HuffmanNode*>> pq;

  pq.Push(ReBuildTree(input_stream));
  ReEncodeString(pq, input_stream, output_stream);
}

#endif  // HUFFMAN_H_
