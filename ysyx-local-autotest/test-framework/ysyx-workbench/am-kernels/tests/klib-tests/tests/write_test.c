#include "klibtest.h"
#define N 32
uint8_t data[N];
uint8_t data1[N];
void reset() {
  int i;
  for (i = 0; i < N; i ++) {
    data[i] = i + 1;
    data1[i] = i + 1;
  }
}

// 检查[l,r)区间中的值是否依次为val, val + 1, val + 2...
void check_seq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val + i - l);
  }
}

// 检查[l,r)区间中的值是否均为val
void check_eq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val);
  }
}
void test_memset() {
  int l, r;
  for (l = 0; l < N; l ++) {
    for (r = l + 1; r <= N; r ++) {
      reset();
      uint8_t val = (l + r) / 2;
      memset(data + l, val, r - l);
      check_seq(0, l, 1);
      check_eq(l, r, val);
      check_seq(r, N, r + 1);
    }
  }
}
void test_memcpy() {
  int l, r;
  for (l = 0; l < N; l ++) {
    for (r = l + 1; r <= N; r ++) {
      reset();
      memcpy(data + l, data1 + l, r - l);
      check_seq(0, l, 1);
      check_seq(l, r, l+1);
      check_seq(r, N, r + 1);
    }
  }

}

int main() {
    test_memset();
    test_memcpy();
  return 0;
}
