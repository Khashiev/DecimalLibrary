#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int equal = 1;
  int check = 0;

  if (!s21_is_zero(value_1) && !s21_is_zero(value_2)) {
    equal = 0;
  }

  if (equal == 0) {
    check = s21_get_sign(value_1) != s21_get_sign(value_2);
  }

  if (equal == 0 && check == 0) {
    if (s21_mantisa_compare(value_1, value_2) == 0) {
      equal = 1;
    }
  }

  return equal;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int less = 1;

  if (s21_is_equal(value_1, value_2)) {
    less = 0;
  } else if (s21_get_sign(value_1) == 0 && s21_get_sign(value_2) == 1) {
    less = 0;
  } else if (s21_get_sign(value_1) == 1 && s21_get_sign(value_2) == 0) {
    less = 1;
  } else {
    int sign = s21_get_sign(value_1);
    int comp_res = s21_mantisa_compare(value_1, value_2);

    if (sign == 0) {
      less = (comp_res == 2) ? 1 : 0;
    } else {
      less = (comp_res == 1) ? 1 : 0;
    }
  }

  return less;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !(s21_is_equal(value_1, value_2));
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_equal(value_1, value_2) || s21_is_less(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_2, value_1);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_equal(value_1, value_2) || s21_is_greater(value_1, value_2);
}
