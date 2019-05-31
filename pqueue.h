#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>
#include <iostream>

template <typename T, typename C = std::less<T>>
class PQueue {
 public:
  // Constructor
  PQueue() {}
  // Return number of items in priority queue
  size_t Size();
  // Return top of priority queue
  T& Top();
  // Remove top of priority queue
  void Pop();
  // Insert item and sort priority queue
  void Push(const T &item);

 private:
  std::vector<T> items;
  size_t cur_size = 0;
  C cmp;

  // Helper methods for indices
  size_t Root() {
    return 0;
  }
  size_t Parent(size_t n) {
    return (n - 1) / 2;
  }
  size_t LeftChild(size_t n) {
    return 2 * n + 1;
  }
  size_t RightChild(size_t n) {
    return 2 * n + 2;
  }

  // Helper methods for node testing
  bool HasParent(size_t n) {
    return n != Root();
  }
  bool IsNode(size_t n) {
    return n < cur_size;
  }

  // Helper methods for restructuring
  void PercolateUp(size_t n);
  void PercolateDown(size_t n);

  // Node comparison
  bool CompareNodes(size_t i, size_t j);
};

//
// Custom Comparator Class for pointers
//

// Overload () to compare objects of pointers and return
// type T will eventually be MyClass* in test_pqueue.cc
template<typename T>
class MyClassPtrCompMin {
 public:
  bool operator()(T lhs, T rhs) const {
    return *lhs < *rhs;
  }
};

// Custom comparator for max priority queue
// Overload () to compare objects of pointers
// type T will eventually be MyClass* in test_pqueue.cc
template<typename T>
class MyClassPtrCompMax {
 public:
  bool operator()(T lhs, T rhs) const {
    return *lhs > *rhs;
  }
};

//
// Public API
//

// Return number of items in priority queue
template <typename T, typename C>
size_t PQueue<T, C>::Size() {
  return cur_size;
}

// Return top of priority queue
template <typename T, typename C>
T& PQueue<T, C>::Top() {
    // Throw error if the pqueue is empty
    if (!Size())
      throw std::underflow_error("Empty priority queue!");
    return items[Root()];
}

// Remove top of priority queue
template <typename T, typename C>
void PQueue<T, C>::Pop() {
    // Throw error if the pqueue is empty
    if (!Size())
        throw std::underflow_error("Empty priority queue!");

    // Move last item to the root and reduce the heap size
    items[Root()] = std::move(items[cur_size - 1]);
    cur_size--;
    items.pop_back();
    // Percolate the item (currently at root) down the pqueue
    PercolateDown(Root());
  }

// Insert item and sort priority queue
template <typename T, typename C>
void PQueue<T, C>::Push(const T &item) {
  // Insert into the underlying vector
  items.push_back(item);
  // Insert at the end
  items[++cur_size] = std::move(item);
  // Percolate up
  PercolateUp(cur_size - 1);
}

//
// Private Methods
//

// Helper methods for restructuring
template <typename T, typename C>
void PQueue<T, C>::PercolateUp(size_t n) {
  // Iterate until n is root
  // If parent node is less(min) or greater(max) than current node
  // swap the parent and current node, and continue with the parent node
  while (HasParent(n) && CompareNodes(n, Parent(n))) {
    std::swap(items[Parent(n)], items[n]);
    n = Parent(n);
  }
}

// Helper methods for restructuring
template <typename T, typename C>
void PQueue<T, C>::PercolateDown(size_t n) {
  // While node has at least one child
  while (IsNode(LeftChild(n))) {
    // Consider the left child by default
    size_t child = LeftChild(n);
    // If the right child exists and smaller than left child,
    // then consider right child
    if (IsNode(RightChild(n)) && CompareNodes(RightChild(n), LeftChild(n)))
      child = RightChild(n);

    // Exchange the smallest child with node to restore heap-order if necessary
    if (CompareNodes(child, n))
      std::swap(items[child], items[n]);
    else
      break;

    // Do it again
    n = child;
  }
}

// Node comparison
template <typename T, typename C>
bool PQueue<T, C>::CompareNodes(size_t i, size_t j) {
     return cmp(items[i], items[j]);
}


#endif  // PQUEUE_H_
