/* 
 * CS:APP Data Lab 
 * 
 * Ashley Wu 204612415
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    //get the binary number of y=1, x=0 and the number of x=1, y=0 and then merge them together
    return ~((~(y&(~x)))&(~(x&(~y))));
}
/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
    //negate x if x is negative and +1 if x is negative
    int mask = x>>31;
    return (mask^x)+(!!mask);
}
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    //Tmax+1=Tmin
    int tmin=x+1;
    int y = x+tmin; //if it's tmin will become 11111..., another case will be -1+0
    y=~y;
    return !(!!y+!tmin);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    //first check if signs are the same, if signs are the same, calculate difference
    int diffsign = ((x>>31)^(y>>31)); //all 1 if signs are different
    int a = (diffsign &(x>>31)); //all 1 if x is negative and signs are different
    int b = ((~diffsign)&(~((y+(~x)+1)>>31))); //all 1 if signs are same and y-x>=0
    return !!(a|b);
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    //get the n,mth byte out and replace
    int moven = (n<<3);
    int movem = (m<<3);
    int maskn = (255<<moven);
    int maskm = (255<<movem);
    int xn = (((x&maskn)>>moven)&255)<<movem;
    int xm = (((x&maskm)>>movem)&255)<<moven;
    int ans = (((x&(~maskn))&(~maskm))|xn)|xm;
    return ans;
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
    //take out first n number as a, left shift, put back last n number
    int move = (33+(~n)); //32-n
    int mask = (((~1)>>1)<<move); //only first ns are 1
    int a = (x&mask)>>move; //take value of first n numbers and move back to the end
    int mask2 = ~(((~1)>>1)<<n); //only last ns are 1
    a = a&mask2;
    return (x<<n)|a;
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
    //retuns 1 when there is only one 1, and the first digit is 0, and non-zero
    return !(~ (((~(x>>31))&(~(!x))) & ((x&(~x+1))^(~x)) ));
    
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
    //marker 01010101...., then check pattern
    int masker= (0x55 << 8) + 0x55;
    masker = (masker << 16) + masker;
    return !((x & masker)^masker);
    
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops : ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
    //if b0,b1... are bits then b0^b1^... is 1 when there are odd numbers of 1/0
    int comp1= x^(x<<16); //compare first 16 to last 16 digits
    int comp2= comp1^(comp1<<8); //compare first 8 to next 8
    int comp3= comp2^(comp2<<4); //comapre first 4 to next 4
    int comp4= comp3^(comp3<<2); //compare first 2 to next 2
    int comp5= comp4^(comp4<<1); //compare first 1 to next 1
    return !!(comp5>>31);
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
    //add digits together using masks
    //mask1 01010101....
    //mask2 00110011....
    //mask3 00001111 00001111 00001111 00001111
    
    int mask1=0x55;
    int mask2=0x33;
    int mask3=0x0F;
    mask1 = (mask1<<8)+0x55;
    mask1 = (mask1<<8)+0x55;
    mask1 = (mask1<<8)+0x55;
    mask2 = (mask2<<8)+0x33;
    mask2 = (mask2<<8)+0x33;
    mask2 = (mask2<<8)+0x33;
    mask3 = (mask3<<8)+0x0F;
    mask3 = (mask3<<8)+0x0F;
    mask3 = (mask3<<8)+0x0F;
    
    x= (x&mask1)+((x>>1)&mask1);
    x= (x&mask2)+((x>>2)&mask2);
    x= (x&mask3)+((x>>4)&mask3);
    x= x+ (x>>8);
    x= x+ (x>>16);
    return (x&0x3F);
}
/* 
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n) {
    //make the rightmost term 1 or 0, move to the left most and then move to the right by n-1
    return (!!n<<31)>>(n+(~0));
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    int mask1 = 0x2;  		// 0010
    int mask2 = 0xC;  		// 0110
    int mask4 = 0xF0;			//11110000
    int mask8 = 0xFF<<8;		//0x0000FF00
    int mask16 = (mask8 | 0xFF) << 16; // 0xFFFF0000
    
    int result = 1;
    int y = x^(x>>31);
    
    int bitnum = (!!(y & mask16)) << 4;
    result += bitnum;
    y = y >> bitnum;
    
    bitnum = (!!(y & mask8)) << 3;
    result += bitnum;
    y = y >> bitnum;
    
    bitnum = (!!(y & mask4)) << 2;
    result += bitnum;
    y = y >> bitnum;
    
    bitnum = (!!(y & mask2)) << 1;
    result += bitnum;
    y = y >> bitnum;
    
    bitnum = !!(y & mask1);
    result += bitnum;  
    y = y >> bitnum;
    
    return result + (y&1);
}
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
    return 2;
}
