#include "s21_decimal.h"

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.

int s21_floor(s21_decimal value, s21_decimal *result) {
  int return_res = ERROR;
  if (result) {
    return_res = OK;
    init_decimal(result);
    s21_decimal one = {{1, 0, 0, 0}};
    return_res = s21_truncate(value, result);
    int sign = get_sign(result->bits[3]);
    if (sign && s21_is_not_equal(value, *result)) {
      s21_sub(*result, one, result);
    }
    result->bits[3] = 0;
    if (sign) {
      result->bits[3] = set_bit_1(result->bits[3], 31);
    } else {
      result->bits[3] = set_bit_0(result->bits[3], 31);
    }
  }
  return return_res;
}

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result) {
  int return_res = ERROR;
  if (result) {
    init_decimal(result);
    rewrite_decimal(value, result);
    return_res = OK;
    int save_value = 0;
    if (is_round_up(value)) {  // проверяет, нужно ли увеличить число
      save_value = 1;
    }
    s21_truncate(value, &value);
    set_scale(&value.bits[3], 0);
    s21_decimal one = {{save_value, 0, 0, 0}};
    s21_add(value, one, result);
  }
  return return_res;
}

// Возвращает целые цифры указанного Decimal числа.
// любые дробные цифры отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int return_res = ERROR;
  if (result) {
    init_decimal(result);
    int scaling_factor =
        get_scaling_factor(value.bits[3]);  // достали коэфф масштабирования
    int sign = s21_get_sign(value);  // достали знак
    for (int i = scaling_factor; i > 0; i--) {
      divide_10(&value);  // делит на 10 весь децимал
    }
    /* сейчас в value лежит нужный decimal
       который будет перезаписан в result */
    rewrite_decimal(value, result);
    result->bits[3] = 0;
    if (sign)
      result->bits[3] = set_bit_1(result->bits[3], 31);
    else
      result->bits[3] = set_bit_0(result->bits[3], 31);
    return_res = OK;
  }
  return return_res;
}

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result) {
  int return_res = ERROR;
  if (result) {
    return_res = OK;
    init_decimal(result);
    rewrite_decimal(value, result);
    if (result->bits[3] == MINUS)
      result->bits[3] = set_bit_0(result->bits[3], 31);
    else {
      result->bits[3] = set_bit_1(result->bits[3], 31);
    }
  }
  return return_res;
}
