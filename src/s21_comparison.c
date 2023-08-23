#include "s21_decimal.h"

void s21_comparison_reverse(int *flag) {
  if (*flag == 1) {
    *flag = 2;
  } else if (*flag == 2) {
    *flag = 1;
  }
}

int s21_greater_num(int a, int b) {
  int flag = 0;
  if (a > b) {
    flag = 1;
  } else if (a < b) {
    flag = 2;
  } else if (a == b) {
    flag = 0;
  }
  return flag;
}

int s21_comparison_bits(s21_decimal value_1, s21_decimal value_2) {
  int bit_1 = 0;
  int bit_2 = 0;
  int flag = 0;

  for (int i = 95; i >= 0 && !flag; --i) {
    bit_1 = s21_find_bit(value_1, i);
    bit_2 = s21_find_bit(value_2, i);

    if (bit_1 > bit_2) flag = 1;
    if (bit_1 < bit_2) flag = 2;
  }

  return flag;
}

int s21_comparison(s21_decimal value_1, s21_decimal value_2) {
  int flag = 0;
  int dif_scale = s21_get_scale(value_1) - s21_get_scale(value_2);

  s21_big_decimal copy_1 = s21_transfer_to_big(value_1);
  s21_big_decimal copy_2 = s21_transfer_to_big(value_2);
  s21_normalize(&copy_1, &copy_2);

  value_1 = s21_transfer_to_decimal(copy_1);
  value_2 = s21_transfer_to_decimal(copy_2);

  flag = s21_greater_num(!s21_get_sign(value_1), !s21_get_sign(value_2));

  if (flag == 0) {
    if (dif_scale >= 0) {
      flag = s21_comparison_bits(value_1, value_2);
    } else {
      dif_scale = -dif_scale;
      flag = s21_comparison_bits(value_1, value_2);
      s21_comparison_reverse(&flag);
    }

    if (s21_get_sign(value_1) && s21_get_sign(value_2)) {
      s21_comparison_reverse(&flag);
    }
  } else {
    if (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0 &&
        value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
      flag = 0;
    }
  }

  return flag;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  return (s21_comparison(value_1, value_2) == 2) ? 1 : 0;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res_cmp = s21_comparison(value_1, value_2);
  return (res_cmp == 2 || res_cmp == 0) ? 1 : 0;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return (s21_comparison(value_1, value_2) == 1) ? 1 : 0;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res_cmp = s21_comparison(value_1, value_2);
  return (res_cmp == 1 || res_cmp == 0) ? 1 : 0;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_comparison(value_1, value_2) == 0) ? 1 : 0;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_comparison(value_1, value_2) == 0) ? 0 : 1;
}
