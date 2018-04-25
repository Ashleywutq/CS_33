#include "defs.h"

// Generate a shift index based on the value, shift amount, and mask.
int gen_shift(int val, int shift, int mask);
void clear_val(unsigned long long* bucket_addr);
void move_kvp(kvp* dst, kvp* src, int i, int out_index);
int bucket_size(int radix);

void do_nothing(int a);
