#include "s21_decimal.h"

int s21_get_sign(s21_decimal value) { return (value.bits[3] & 1u << 31) != 0; }

bool s21_is_zero(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 && 1;
}

int s21_get_exp(s21_decimal value) { return (value.bits[3] & EXP) >> 16; }

s21_big_decimal s21_transfer_to_big(s21_decimal value) {
  s21_big_decimal res = {0};
  uint16_t exp = s21_get_exp(value);
  for (int i = 0; i < 3; i++) res.bits[i] = value.bits[i] & UINT_MAX;
  res.exp = exp;
  return res;
}

bool s21_is_overflow(s21_big_decimal value) {
  bool res = 0;
  for (int i = 3; i < 7 && !res; i++) res = value.bits[i] ? 1 : 0;
  return res;
}

bool s21_check_overflow(s21_big_decimal *value) {
  uint32_t overflow = 0;
  for (int i = 0; i < 7; i++) {
    value->bits[i] += overflow;
    overflow = value->bits[i] >> 32;
    value->bits[i] &= UINT_MAX;
  }
  return s21_is_overflow(*value);
}

bool s21_point_left(s21_big_decimal *value) {
  bool overflow = 0;
  s21_big_decimal copy = *value;
  for (int i = 0; i < 7; i++) copy.bits[i] *= 10;
  copy.exp++;
  overflow = s21_check_overflow(&copy);
  if (!overflow && value->exp < 28) *value = copy;
  return overflow;
}

bool s21_normalize(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  bool res = 0;
  if (value_1->exp == value_2->exp) return res;
  if (value_1->exp > value_2->exp)
    while (value_2->exp < value_1->exp) res = s21_point_left(value_2);
  else
    while (value_1->exp < value_2->exp) res = s21_point_left(value_1);
  return res;
}

void s21_set_exp(s21_decimal *value, uint16_t exp) {
  value->bits[3] |= (exp << 16);
}

s21_decimal s21_transfer_to_decimal(s21_big_decimal value) {
  s21_decimal res = {0};
  for (int i = 0; i < 3; i++) res.bits[i] = value.bits[i] & UINT_MAX;
  s21_set_exp(&res, value.exp);
  return res;
}

int s21_mantisa_compare(s21_decimal value_1, s21_decimal value_2) {
  s21_big_decimal copy_1 = s21_transfer_to_big(value_1);
  s21_big_decimal copy_2 = s21_transfer_to_big(value_2);

  s21_normalize(&copy_1, &copy_2);

  value_1 = s21_transfer_to_decimal(copy_1);
  value_2 = s21_transfer_to_decimal(copy_2);

  int res = 3;

  for (int i = 2; i >= 0 && res == i + 1; i--) {
    if (value_1.bits[i] == value_2.bits[i]) {
      res--;
    } else if (value_1.bits[i] > value_2.bits[i]) {
      res = 1;
      break;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      res = 2;
      break;
    }
  }

  return res;
}