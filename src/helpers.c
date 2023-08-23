#include "s21_decimal.h"

int s21_get_bit(unsigned int num, unsigned index) { return (num >> index) & 1; }

int s21_get_sign(s21_decimal num) { return s21_get_bit(num.bits[3], 31); }

int s21_find_bit(s21_decimal num, int index) {
  int mask = 1u << (index % 32);
  return (num.bits[index / 32] & mask) != 0;
}

int s21_get_scale(s21_decimal dst) {
  int mask = 127 << 16;
  int scale = (mask & dst.bits[3]) >> 16;
  return scale;
}

// Проверка экспоненты
bool s21_check_exp(s21_decimal src) { return (s21_get_exp(src) > 28) ? 1 : 0; }

// Получение exp (scale to 10) в десятичном виде
int s21_get_exp(s21_decimal src) { return (src.bits[3] & EXP) >> 16; }

// Установка exp
void s21_set_exp(s21_decimal *src, uint16_t exp) {
  src->bits[3] |= (exp << 16);
}

// Конвертирование s21_decimal в s21_big_decimal
s21_big_decimal s21_transfer_to_big(s21_decimal src) {
  s21_big_decimal res = {0};
  uint16_t exp = s21_get_exp(src);
  for (int i = 0; i < 3; i++) res.bits[i] = src.bits[i] & UINT_MAX;
  res.exp = exp;
  return res;
}

// Конвертирование s21_big_decimal в s21_decimal
s21_decimal s21_transfer_to_decimal(s21_big_decimal src) {
  s21_decimal res = {0};
  for (int i = 0; i < 3; i++) res.bits[i] = src.bits[i] & UINT_MAX;
  s21_set_exp(&res, src.exp);
  return res;
}

// Прогоняет биты на переполнение
bool s21_is_overflow(s21_big_decimal src) {
  bool res = 0;
  for (int i = 3; i < 7 && !res; i++) res = src.bits[i] ? 1 : 0;
  return res;
}

// Проверка переполнения
bool s21_check_overflow(s21_big_decimal *src) {
  uint32_t overflow = 0;
  for (int i = 0; i < 7; i++) {
    src->bits[i] += overflow;
    overflow = src->bits[i] >> 32;
    src->bits[i] &= UINT_MAX;
  }
  return s21_is_overflow(*src);
}

// Сдвиг влево (exp++), т.е. домножение на 10, не меняя числа.
// При переполнении возвращает 1.
bool s21_point_left(s21_big_decimal *src) {
  bool overflow = 0;
  s21_big_decimal copy = *src;
  for (int i = 0; i < 7; i++) copy.bits[i] *= 10;
  copy.exp++;
  overflow = s21_check_overflow(&copy);
  if (!overflow && src->exp < 28) *src = copy;
  return overflow;
}

// Приведение к общему множителю (экспоненте)
bool s21_normalize(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  bool res = 0;
  if (value_1->exp == value_2->exp) return res;
  if (value_1->exp > value_2->exp)
    while (value_2->exp < value_1->exp) res = s21_point_left(value_2);
  else
    while (value_1->exp < value_2->exp) res = s21_point_left(value_1);
  return res;
}

// bool s21_is_zero(s21_decimal value) {
//   return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 && 1;
// }