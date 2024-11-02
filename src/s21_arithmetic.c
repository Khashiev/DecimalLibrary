#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  init_decimal(result);
  if (is_inverse(value_1, value_2)) {
    value_1.bits[3] += MINUS;
    value_2.bits[3] += MINUS;
    result->bits[3] = MINUS;
  }
  int exponent = num_of_degree(value_1) - num_of_degree(value_2);
  long long save_value = 0;
  for (int bits_part = 0; bits_part != 3; bits_part++) {
    long value1 = value_of_compare_bits(value_1, bits_part, -exponent);
    long value2 = value_of_compare_bits(value_2, bits_part, exponent);
    long long sum_value =
        (value1 + value2) *
            pow(-1, (value_2.bits[3] / MINUS + value_1.bits[3] / MINUS) / 2) +
        save_value;
    result->bits[bits_part] = (unsigned)(sum_value);
    save_value = 0;
    if (sum_value < 0) {
      save_value = -1;
    }
    if (sum_value >= 2 * MINUS) {
      save_value = sum_value / (2 * MINUS);
    }
  }
  result->bits[3] += (num_of_degree(value_1) > num_of_degree(value_2))
                         ? (num_of_degree(value_1) * 65536)
                         : (num_of_degree(value_2) * 65536);
  get_result_by_ok(&save_value, value_1, value_2, result);
  return error_result(save_value);
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  value_2.bits[3] += MINUS;
  return s21_add(value_1, value_2, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
  s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
  s21_decimal_alt alt_result;
  s21_null_decimal_alt(&alt_result);
  res = s21_mul_alt(alt_value_1, alt_value_2, &alt_result);
  *result = s21_convert_alt_to_std(alt_result);
  return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    res = 3;
  } else if (value_1.bits[0] == 0 && value_1.bits[1] == 0 &&
             value_1.bits[2] == 0) {
    init_decimal(result);
  } else {
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
    s21_decimal_alt alt_result;
    s21_null_decimal_alt(&alt_result);
    while (alt_value_1.bits[0] == 0 && alt_value_2.bits[0] == 0) {
      s21_right_shift(&alt_value_1);
      s21_right_shift(&alt_value_2);
    }
    res = s21_div_alt(alt_value_1, alt_value_2, &alt_result);
    *result = s21_convert_alt_to_std(alt_result);
  }
  return res;
}
