#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINUS 0x80000000
#define EXP 0x00ff0000

typedef struct {
  uint32_t bits[4];
} s21_decimal;

typedef struct {
  uint64_t bits[8];
  uint16_t exp;
} s21_big_decimal;

// helpers
int s21_get_bit(unsigned int num, unsigned index);
int s21_get_sign(s21_decimal num);
int s21_find_bit(s21_decimal num, int index);
int s21_get_scale(s21_decimal dst);
bool s21_normalize(s21_big_decimal *value_1, s21_big_decimal *value_2);
bool s21_check_exp(s21_decimal src);
int s21_get_exp(s21_decimal src);
void s21_set_exp(s21_decimal *src, uint16_t exp);
s21_big_decimal s21_transfer_to_big(s21_decimal src);
s21_decimal s21_transfer_to_decimal(s21_big_decimal src);

// Comparison
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

#endif  // SRC_S21_DECIMAL_H_