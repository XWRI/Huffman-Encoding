#include <cstdio>
#include <fstream>

#include <gtest/gtest.h>

#include "bstream.h"

TEST(BStream, input_1) {
  std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
    0x58, 0x90, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, input_2) {
  std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
    0x42, 0x38, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0100001000111000101010110000100000000000010011101101101101000000
  // ^4  ^2  ^3  ^8  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x08);  // 00001000
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0xe2);  // 11100010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, input_3) {
  std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
    0x33, 0x09, 0xa1, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0011001100001001101000010000100000000000010011101101101101000000
  // ^3  ^3  ^0  ^9  ^a  ^1  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0xcc);  // 11001100
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0x26);  // 00100110
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x8400276);  // 00001000010000000000001001110110
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0xda);   //  11011010
  ifs.close();

  std::remove(filename.c_str());
}


TEST(BStream, output_1) {
  std::string filename{"test_bstream_output"};

  const unsigned char val[] = {
    0x58, 0x90, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  BinaryOutputStream bos(ofs);
  bos.PutChar(val[0]);
  bos.PutChar(val[1]);
  bos.PutBit(val[2] & (1 << 7));
  bos.PutBit(val[2] & (1 << 6));
  bos.PutBit(val[2] & (1 << 5));
  bos.PutBit(val[2] & (1 << 4));
  bos.PutBit(val[2] & (1 << 3));
  bos.PutBit(val[2] & (1 << 2));
  bos.PutBit(val[2] & (1 << 1));
  bos.PutBit(val[2] & (1 << 0));
  bos.PutInt((((val[3] << 24) | (val[4] << 16)) | (val[5] << 8)) | val[6]);
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we writing the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110
  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, output_2) {
  std::string filename{"test_bstream_output"};

  const unsigned char val[] = {
    0x42, 0x38, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0100001000111000101010110000100000000000010011101101101101000000
  // ^4  ^2  ^3  ^8  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  BinaryOutputStream bos(ofs);
  bos.PutChar(val[0]);
  bos.PutChar(val[1]);
  bos.PutBit(val[2] & (1 << 7));
  bos.PutBit(val[2] & (1 << 6));
  bos.PutBit(val[2] & (1 << 5));
  bos.PutBit(val[2] & (1 << 4));
  bos.PutBit(val[2] & (1 << 3));
  bos.PutBit(val[2] & (1 << 2));
  bos.PutBit(val[2] & (1 << 1));
  bos.PutBit(val[2] & (1 << 0));
  bos.PutInt((((val[3] << 24) | (val[4] << 16)) | (val[5] << 8)) | val[6]);
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we writing the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x08);  // 00001000
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0xe2);  // 11100010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110
  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, output_3) {
  std::string filename{"test_bstream_output"};

  const unsigned char val[] = {
    0x33, 0x09, 0xa1, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0011001100001001101000010000100000000000010011101101101101000000
  // ^3  ^3  ^0  ^9  ^a  ^1  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  BinaryOutputStream bos(ofs);
  bos.PutChar(val[0]);
  bos.PutChar(val[1]);
  bos.PutBit(val[2] & (1 << 7));
  bos.PutBit(val[2] & (1 << 6));
  bos.PutBit(val[2] & (1 << 5));
  bos.PutBit(val[2] & (1 << 4));
  bos.PutBit(val[2] & (1 << 3));
  bos.PutBit(val[2] & (1 << 2));
  bos.PutBit(val[2] & (1 << 1));
  bos.PutBit(val[2] & (1 << 0));
  bos.PutInt((((val[3] << 24) | (val[4] << 16)) | (val[5] << 8)) | val[6]);
  bos.PutChar(val[7]);
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we writing the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0xcc);  // 11001100
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0x26);  // 00100110
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x8400276);  // 00001000010000000000001001110110
  EXPECT_EQ((unsigned char)bis.GetChar(), (unsigned char)0xda);  //  11011010
  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, output_) {
  std::string filename{"test_bstream_output"};

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  BinaryOutputStream bos(ofs);
  bos.PutChar('A');
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we writing the binary in the correct order
  EXPECT_EQ(bis.GetChar(), 0x41);  // 01100010
  ifs.close();

  std::remove(filename.c_str());
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
