#line 31 "<command-line>"
#include "/usr/include/stdc-predef.h"
#line 14 "bits.c"
#include "btest.h"
#include "/usr/lib/gcc/x86_64-linux-gnu/8/include-fixed/limits.h"
#line 22
team_struct team=
{
#line 28
    "520021910607",

   "TAN Zi Ming",

   "520021910607",
#line 36
   "",

   ""};
#line 177
int bang(int x) {
  int bit_16=(  x >> 16) | x;
  int bit_8=(  bit_16 >> 8) | bit_16;
  int bit_4=(  bit_8 >> 4) | bit_8;
  int bit_2=(  bit_4 >> 2) | bit_4;
  int bit=(  bit_2 >> 1) | bit_2;
  return ((bit & 1) ^ 1);
}
#line 192
int bitCount(int x) {
  int mask=  0x11 +( 0x11 << 8) +( 0x11 << 16) +( 0x11 << 24);
  int sum=(  x & mask) +(( x >> 1) & mask) +(( x >> 2) & mask) +(( x >> 3) & mask);
  sum =( sum >> 16) + sum;
  sum =((( sum >> 12) & 0xF) +(( sum >> 8) & 0xF) +(( sum >> 4) & 0xF) +( sum & 0xF));
  return sum;
}
#line 206
int copyLSB(int x) {

  return (~(1 & x) + 1);
}
#line 218
int divpwr2(int x, int n) {
  int signMask=  x >> 31;
  return ((x >> n) & ~signMask)|(((x +( 1 << n) + ~0) >> n) & signMask);
}
#line 228
int evenBits(void) {
  return 0x55 +( 0x55 << 8) +( 0x55 << 16) +( 0x55 << 24);
}
#line 240
int fitsBits(int x, int n) {
  int front=  33 + ~n;
  return !(((x << front) >> front) ^ x);
}
#line 252
int getByte(int x, int n) {
  return ((x >>( n << 3)) & 0xFF);
}
#line 262
int isGreater(int x, int y) {
  int sameSign=((  x ^ y) &( 1 << 31)) >> 31;
  int differ=  x + ~y + 1;
  int differSign=  !(differ >> 31);
  int isEqual=  !(x ^ y);
  return (~sameSign & differSign & !isEqual)|(sameSign & !(x >> 31));
}
#line 276
int isNonNegative(int x) {
  return !(x >> 31);
}
#line 286
int isNotEqual(int x, int y) {
  return !!(x ^ y);
}
#line 297
int isPower2(int x) {
  int sign=  x >> 31;
  return ~sign & !!x & !(x &( x + ~0));
}
#line 309
int leastBitPos(int x) {
  return ((~x + 1) & x);
}
#line 320
int logicalShift(int x, int n) {
  int mask=  ~((1 << 31) >>( n + ~0));
  return ((x >> n) & mask);
}
#line 334
int satAdd(int x, int y) {
  int result=  x + y;
  int sameSign=((  x ^ y) >> 31);
  int signChange=((  result ^ x) &( result ^ y)) >> 31;
  return (sameSign & result)|(~sameSign & ~signChange & result)|(~sameSign & signChange & ~((x >> 31) +( 1 << 31)));
}
#line 349
int tc2sm(int x) {
  int sign=(  1 << 31) & x;
  int mask=  sign >> 31;
  return ((x & ~mask)|((~x + 1) & mask)| sign);
}
