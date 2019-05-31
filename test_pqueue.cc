#include <functional>
#include <gtest/gtest.h>

#include "pqueue.h"

// Test Push, Top, Size, Pop one element
TEST(PQueue, one_element) {
  PQueue<int> pq;

  pq.Push(42);
  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 1);

  pq.Pop();
  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_EQ(pq.Size(), 0);
}

// Test percolateUp (push increasing order)
TEST(PQueue, percolate_up_increasing) {
  PQueue<int> pq;

  pq.Push(11);
  EXPECT_EQ(pq.Top(), 11);
  EXPECT_EQ(pq.Size(), 1);
  pq.Push(12);
  EXPECT_EQ(pq.Top(), 11);
  EXPECT_EQ(pq.Size(), 2);
  pq.Push(13);
  EXPECT_EQ(pq.Top(), 11);
  EXPECT_EQ(pq.Size(), 3);
}

// Test PercolateUp (push decreasing order)
TEST(PQueue, percolate_up_decreasing) {
  PQueue<int> pq;

  pq.Push(12);
  EXPECT_EQ(pq.Top(), 12);
  EXPECT_EQ(pq.Size(), 1);
  pq.Push(11);
  EXPECT_EQ(pq.Top(), 11);
  EXPECT_EQ(pq.Size(), 2);
  pq.Push(10);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 3);
}

// Test PercolateUp (push random order)
TEST(PQueue, percolate_up_random) {
  PQueue<int> pq;

  pq.Push(10);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 1);
  pq.Push(12);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 2);
  pq.Push(9);
  EXPECT_EQ(pq.Top(), 9);
  EXPECT_EQ(pq.Size(), 3);
}

// Test PercolateDown (push increasing order)
TEST(PQueue, percolate_down_increasing) {
  PQueue<int> pq;

  pq.Push(10);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 1);
  pq.Push(11);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 2);
  pq.Push(12);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 3);

  pq.Pop();
  EXPECT_EQ(pq.Top(), 11);
  EXPECT_EQ(pq.Size(), 2);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 12);
  EXPECT_EQ(pq.Size(), 1);
  pq.Pop();
  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_EQ(pq.Size(), 0);
}

// Test PercolateDown (push decreasing order)
TEST(PQueue, percolate_down_decreasing) {
  PQueue<int> pq;

  pq.Push(12);
  EXPECT_EQ(pq.Top(), 12);
  EXPECT_EQ(pq.Size(), 1);
  pq.Push(11);
  EXPECT_EQ(pq.Top(), 11);
  EXPECT_EQ(pq.Size(), 2);
  pq.Push(10);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 3);

  pq.Pop();
  EXPECT_EQ(pq.Top(), 11);
  EXPECT_EQ(pq.Size(), 2);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 12);
  EXPECT_EQ(pq.Size(), 1);
  pq.Pop();
  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_EQ(pq.Size(), 0);
}

// Test PercolateDown (push random order)
TEST(PQueue, percolate_down_random) {
  PQueue<int> pq;

  pq.Push(10);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 1);
  pq.Push(12);
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 2);
  pq.Push(9);
  EXPECT_EQ(pq.Top(), 9);
  EXPECT_EQ(pq.Size(), 3);

  pq.Pop();
  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 2);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 12);
  EXPECT_EQ(pq.Size(), 1);
  pq.Pop();
  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_EQ(pq.Size(), 0);
}

// Test Exceptions
TEST(PQueue, exceptions) {
  PQueue<int> pq;

  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_THROW(pq.Pop(), std::exception);
}

//  Test the default(std::less) comparator with integers
TEST(PQueue, int_less) {
  PQueue<int> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 2);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
  EXPECT_EQ(pq.Size(), 3);
}

//  Test std::greater comparator with integers
TEST(PQueue, int_great) {
  PQueue<int, std::greater<int>> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
  EXPECT_EQ(pq.Size(), 3);
}

//  Test the default comparator with char
TEST(PQueue, char_less) {
  PQueue<char> pq;

  pq.Push('A');
  pq.Push('B');
  pq.Push('C');
  pq.Push('D');
  pq.Push('E');

  EXPECT_EQ(pq.Top(), 'A');
  EXPECT_EQ(pq.Size(), 5);
  pq.Pop();
  pq.Pop();
  EXPECT_EQ(pq.Top(), 'C');
  EXPECT_EQ(pq.Size(), 3);

  // {(C, 10), (A, 30), (B, 30)}
}

//  Test std::greater comparator with char
TEST(PQueue, char_great) {
  PQueue<char, std::greater<char>> pq;

  pq.Push('A');
  pq.Push('B');
  pq.Push('C');
  pq.Push('D');
  pq.Push('E');

  EXPECT_EQ(pq.Top(), 'E');
  EXPECT_EQ(pq.Size(), 5);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 'D');
  EXPECT_EQ(pq.Size(), 4);
}


class MyClass {
 public:
  explicit MyClass(int n) : n_(n) {}
  bool operator < (const MyClass &mc) const { return n_ < mc.n_; }
  bool operator > (const MyClass &mc) const { return n_ > mc.n_; }
  int n() { return n_; }
 private:
  int n_;
};

//  Test customized class object with default comparator
TEST(PQueue, custom_class_less) {
  std::vector<MyClass> vec{MyClass(42), MyClass(23),
                           MyClass(2), MyClass(34), MyClass(10)};

  PQueue<MyClass> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top().n(), vec[2].n());
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  pq.Push(vec[4]);
  EXPECT_EQ(pq.Top().n(), vec[4].n());
  EXPECT_EQ(pq.Size(), 4);
}

//  Test customized class object with std::greater
TEST(PQueue, custom_class_great) {
  std::vector<MyClass> vec{MyClass(42), MyClass(23), MyClass(2), MyClass(34)};

  PQueue<MyClass, std::greater<MyClass>> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top().n(), vec[0].n());
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top().n(), vec[3].n());
}

//  Test customized pointer object with customized Min comparator
TEST(PQueue, custom_class_pointer_min) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23),
                            new MyClass(2), new MyClass(34),
                            new MyClass(10)};

  PQueue<MyClass*, MyClassPtrCompMin<MyClass*>> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[2]);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[1]);
  EXPECT_EQ(pq.Size(), 3);
  pq.Push(vec[4]);
  EXPECT_EQ(pq.Top(), vec[4]);
  EXPECT_EQ(pq.Size(), 4);
}

//  Test customized pointer object with customized Max comparator
TEST(PQueue, custom_class_pointer_max) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23),
                            new MyClass(2), new MyClass(34)};

  PQueue<MyClass*, MyClassPtrCompMax<MyClass*>> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[0]);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[3]);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
