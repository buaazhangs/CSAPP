/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


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

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

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
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~x&~y)&~(x&y);
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
//等号的操作可以直接利用a == b 等价于 !(a ^b)，因为完全相等的自己跟自己异或为0，否则不为0布尔值即为1
int isTmax(int x) {
  return !((~(x+1)^x))&(!!(x+1));//加！变为布尔值,当然最后加个按位且又变为返回int
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int a = (((0xAA << 8 | 0xAA) << 16) | ((0xAA << 8) | 0xAA));
  return !((x & a) ^ a);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  return (!((x + (~(0x30)+1)) >> 31) & 1) & (!((0x39 + (~x+1)) >> 31) & 1);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 *///((!!x - 1) | y)&((!x - 1) | z)
//x为真，需要0000与y按位或，否则1111(-1)与y按位或；x为假，0000与z按位或
int conditional(int x, int y, int z) {
  return ((!!x + (~1+1)) | y)&((!x + (~1+1)) | z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
//注意大于等于0符号位相等,因此转化为 y-x >=0,另外注意被减数为tmin时，-tmin=tmin，要特判
//再次要注意溢出，正溢出情况为y为正，x为负，负溢出情况为y为负，x为正
//y，-x的第二位都是0的负数就会溢出了
int isLessOrEqual(int x, int y) {
  int a = x >> 31;//x<0
  int b = y >> 31;//y<0
  int c = !b & a;//y为正且x为负，c为真一定为真.c为假不一定为假
  int d = b & !a;//y为负且x为正，d为真一定为假，d为假不一定为真
  int e = !((y + (~x+1)) >> 31);//y-x不溢出情况，c,d都为假，e来决定真假
  return c |(!d & e);
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
//x == 0 trans to x >= 0 and x - 1 <= 0
//注意int补码右移为算数右移，继续填充符号位
//~x+1 >> 31 负数和0能得到0，但0本身符号位为0，负数本身符号位为1，因此x | ~x+1
//((x | (~x +1)) >> 31)，0得到0，其余得到11111111，然后再加1即可。
int logicalNeg(int x) {
  return ((x | (~x +1)) >> 31) + 1;
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
//正数，最高位+1，负数，取反+1后，新数的最高位+1
//获取最高位方法：分治，
//先判断高16为是否为0，即!!(x>>16),若不为0，说明至少有16位，因此左移4为，变为16，即10000
//因此b16为！！(x>>16)<<4,
//然后，根据b16情况，如果高位有值分析高位，右移16位，否则分析低位，右移0位。
int howManyBits(int x) {
    int b16,b8,b4,b2,b1,b0;
    int flag=x>>31;
    x=(flag&~x)|(~flag&x); //x为非正数则不变 ,x 为负数 则相当于按位取反
    b16=!!(x>>16) <<4; //如果高16位不为0,则我们让b16=16
    x>>=b16; //如果高16位不为0 则我们右移动16位 来看高16位的情况
    //下面过程基本类似
    b8=!!(x>>8)<<3;
    x >>= b8;
    b4 = !!(x >> 4) << 2;
    x >>= b4;
    b2 = !!(x >> 2) << 1;
    x >>= b2;
    b1 = !!(x >> 1);
    x >>= b1;
    b0 = x;
  return b0+b1+b2+b4+b8+b16+1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
//f = (-1)^s * M * 2^E
//s:1,exp:8,frac:23
//E = exp - bias,M=1.xxxx,xxxx为frac表示的二进制小数
/*
0x1表示的依然不是0x1，尾数表示的是2^-23*1，
指数表示的是1-bias，非规格化数，bias = 2^7 - 1，
因此E = 1 - 2^7 = -126，所以此时表示的是浮点数（2^-126 * 1*2^-23）,
而0x2表示的是（2^-126 * 2*2^-23）
*/
//所以总的做法为，先提取各个部分，然后判断规格化属性，exp全0为非规格化
//非规格化为，E = 1 - bias,规格化为E = exp - bias,
//因此非规格化frac左移 << 1即可,
//另外，exp全1且frac非0为nan，返回uf
//exp全1且frac全0为inf，也直接返回uf
//其他情况,规格化部分为exp项加1
unsigned floatScale2(unsigned uf) {
  int sign = uf >> 31;
  int exp = (uf >> 23) & (0xFF);
  int frac = uf & (0x7FFFFF);
  if (exp == 0x00){
    frac = frac << 1;
    return sign << 31 | exp << 23 | frac;
  }
  else if(exp == 0xFF){
    return uf;
  }
  else{
    return uf + 0x800000;//exp项加1
  }
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
//返回uf的int项，uf使用unsigned表示各个位
//注意int(2.999) = 2
//int(-3.5) = -3
//非normal，为0
//normal,E = exp - bias,bias = 127 
//本身frac部分就是右移多个的数据，我们解出E的值，
//根据值的正负性左移或右移，得出新结果再右移23个后得到的值即为int
int floatFloat2Int(unsigned uf) {
  int sign = uf >> 31;
  int exp = (uf >> 23) & (0xFF);
  int frac = uf & (0x7FFFFF);
  int bias = (0x1 << 7) - 1;
  int res;
  if (exp == 0x00){
    return 0x0;
  }
  else if(exp == 0xFF){
    return 0x80000000;
  }
  else{
    int E = exp - bias;
    if(E > 31)
      return 0x80000000;
    frac = frac + 0x800000;//规格化数
    //注意下面的分类，不能完全的左移右移，
    //完全的左移再右移或右移再左移都会有损失。
    //因此要进行分类讨论
    if ((E - 23) >= 0){
      res = frac << (E-23);
    }
    else if (E >= 0 && E <= 23){
      res = frac >> (23 - E);
    }
    else
      return 0x0;
    if (sign == 1){
      return ~res + 1;
    }
    else{
      return res;
    }
  }
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
//exp范围为1-254，E范围为-126 <= E <= 127
//frac均为0，
unsigned floatPower2(int x) {
  if (x <= -126)
    return 0x0;
  else if (x > 127)
    return 0x0FF << 23;
  else
    return (x + ((0x1 << 7) - 1)) << 23;
}
