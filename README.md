# s21_decimal 

Implementation of your own s21_decimal.h library.

The russian version of the task can be found in the repository.

## Contents

1. [Chapter I](#chapter-i) \
   1.1. [Introduction](#introduction)
2. [Chapter II](#chapter-ii) \
   2.1. [Information](#information)
3. [Chapter III](#chapter-iii) \
   3.1. [Part 1](#part-1-implementation-of-the-decimalh-library-functions)

## Chapter I

Planet Earth, 1990s. 

The world economy is growing exponentially, the stock exchange increases many times year after year, more and more companies go public and their shares start to be quoted. The number of users, the number of transactions, price, commission, interest, calculation of financial technical indicators... It's hard to overestimate the accuracy of all this data, and there are serious problems with the current obsolete data types used in the financial sector.

Due to a calculation error in the usual IEEE 754 (float) millions of dollars are lost every year, which simply absorbed from the system, disappearing forever. 

Together with the FIX (Financial Information eXchange) protocol, which is being developed for data processing in the transfer between the broker and the exchange, one more tool is needed for data transfer and storage.
At the follow-up meeting:

*-- So, gentlemen, please note that our group of specialists, who have already proven themselves in many successful projects, has been tasked by the government to develop a completely new type of data, code-named Decimal. Its task is to allow significantly minimise error for several decades, and in some cases remove it completely in the world's financial transactions. It is required to describe all the necessary logical and arithmetic operations, which would allow to make the necessary calculations quickly and conveniently.*

*- Wow, that's quite an order we got, and from such a customer! We have to keep this client - it promises us big contracts in the future if we do well!*

*-- Yes, you're right, that’s why we need to figure out what functions we need to implement... Any suggestions?*

*-- Sum and difference....*

*-- Multiplication and division...*

*-- Agreed, but we need more!*

*- Taking the remainder, comparison and conversion operations!*

*-- Mathematical rounding in all directions!*

*-- Yes, I think that’s enough, let’s get to work! We've got no more than a couple of days, don't let us down!*

## Introduction

In this project you will implement the s21_decimal.h library in the C programming language. This library should add the ability to work with the "decimal" type, which is not in the language standard. Nevertheless, this type is critically important. For financial calculations, for example, where errors of calculations characteristic of types with floating point are unacceptable. As part of the project you will work with the tasks of processing financial information, dive into the issues of internal representation of different types of data, and solidify knowledge of structured programming.


## Chapter II

## Information

The Decimal value type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335. The default value of a Decimal is 0. The Decimal value type is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors. The Decimal type does not eliminate the need for rounding. Rather, it minimizes errors due to rounding.

When the result of the division and multiplication is passed to the Round method, the result suffers no loss of precision.

A decimal number is a floating-point value that consists of a sign, a numeric value where each digit in the value ranges from 0 to 9, and a scaling factor that indicates the position of a floating decimal point that separates the integral and fractional parts of the numeric value.

The binary representation of a Decimal value consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the 96-bit integer and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28. Therefore, the binary representation of a Decimal value the form, ((-296 to 296) / 10(0 to 28)), where -(296-1) is equal to MinValue, and 296-1 is equal to MaxValue.

The scaling factor also preserves any trailing zeros in a Decimal number. Trailing zeros do not affect the value of a Decimal number in arithmetic or comparison operations. However, trailing zeros might be revealed by the ToString method if an appropriate format string is applied.

### Binary representation

The binary representation of a Decimal number consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the integer number and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28.

Decimal number can be implemented as a four-element long array of 32-bit signed integers (`int bits[4];`).

`bits[0]`, `bits[1]`, and `bits[2]` contain the low, middle, and high 32 bits of the 96-bit integer number.

`bits[3]` contains the scale factor and sign, and consists of following parts:

Bits 0 to 15, the lower word, are unused and must be zero.

Bits 16 to 23 must contain an exponent between 0 and 28, which indicates the power of 10 to divide the integer number.

Bits 24 to 30 are unused and must be zero.

Bit 31 contains the sign; 0 meaning positive, and 1 meaning negative.

Example:
```c
typedef struct
{
    bits[4];
} s21_decimal;
```


### Arithmetic Operators

| Operator name | Operators  | Function | 
| ------ | ------ | ------ |
| Addition | + | s21_decimal add(s21_decimal, s21_decimal) |
| Subtraction | - | s21_decimal sub(s21_decimal, s21_decimal) |
| Multiplication | * | s21_decimal mul(s21_decimal, s21_decimal) | 
| Division | / | s21_decimal div(s21_decimal, s21_decimal) |
| Modulo | Mod | s21_decimal mod(s21_decimal, s21_decimal) |

### Comparison Operators

| Operator name | Operators  | Function | 
| ------ | ------ | ------ |
| Less than | < | s21_int is_less(s21_decimal, s21_decimal) |
| Less than or equal to | <= | int s21_is_less_or_equal(s21_decimal, s21_decimal) | 
| Greater than | > |  int s21_is_greater(s21_decimal, s21_decimal) |
| Greater than or equal to | >= | int s21_is_greater_or_equal(s21_decimal, s21_decimal) | 
| Equal to | == |  int s21_is_equal(s21_decimal, s21_decimal) |
| Not equal to | != |  int s21_is_not_equal(s21_decimal, s21_decimal) |

Return value:
- 0 - TRUE
- 1 - FALSE

### Convertors and parsers
| Convertor/parser | Function | 
| ------ | ------ |
| From int  | int s21_from_int_to_decimal(int src, s21_decimal *dst) |
| From float  | int s21_from_float_to_decimal(float src, s21_decimal *dst) |
| To int  | int s21_from_decimal_to_int(s21_decimal src, int *dst) |
| To float  | int s21_from_decimal_to_float(s21_decimal src, float *dst) |

Return value - code error:
- 0 - SUCCESS
- 1 - CONVERTING ERROR

### Another functions
| Description | Function | 
| ------ | ------ |
| Rounds a specified Decimal number to the closest integer toward negative infinity. | s21_decimal floor(s21_decimal) |	
| Rounds a decimal value to the nearest integer. | s21_decimal round(s21_decimal) |
| Returns the integral digits of the specified Decimal; any fractional digits are discarded. | s21_decimal truncate(s21_decimal) |
| Returns the result of multiplying the specified Decimal value by negative one. | s21_decimal negate(s21_decimal) |

## Chapter III

## Part 1. Implementation of the decimal.h library functions

The functions of the decimal.h library described [above](#information) must be implemented:
- The library must be developed in C language of C11 standard using gcc compiler
- The library code must be located in the src folder
- Make it as a static library (with the s21_decimal.h header file)
- The library must be developed according to the principles of structured programming;
- Use prefix s21_ before each function
- Prepare full coverage of library functions code with unit-tests using the Check library
- Provide a Makefile for building the library and tests (with targets all, clean, test, s21_decimal.a)
- The internal implementation of the decimal type can be arbitrary
- The defined type must support numbers from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335.
