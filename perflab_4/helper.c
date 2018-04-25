#include "helper.h"

// Generate a shift index based on the value, shift amount, and mask.
int gen_shift(int val, int shift, int mask) {
  return ((val >> shift) & mask);
}

void move_kvp(kvp* dst, kvp* src, int src_i, int dst_i) {
  dst[dst_i].key = src[src_i].key;
  dst[dst_i].value = src[src_i].value;
}

//return the length of the buckets (2 ^ radix)
int bucket_size(int log_radix) {
  int pow_of_2=1;
  for(int i = 0; i < log_radix; ++i) {
    pow_of_2*=2;
  }
  return pow_of_2;
}

void do_nothing(int a) {
  static int b=2;
  for(int i = 0; i < a; ++i) {
    b*=b;
  }
}
