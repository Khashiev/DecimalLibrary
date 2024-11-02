#include <check.h>
#include <stdlib.h>
#include <time.h>

#include "s21_decimal.h"

// Another functions

START_TEST(s21_negate_test_1) {
  s21_decimal src = {{12, 12, 12, MINUS}};
  s21_decimal dest = {0};
  int ret = s21_negate(src, &dest);
  ck_assert_uint_eq(dest.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_negate_test_2) {
  s21_decimal src = {{12, 12, 12, 0}};
  s21_decimal dest = {0};
  int ret = s21_negate(src, &dest);
  ck_assert_uint_eq(dest.bits[3], MINUS);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_floor_test_positive_number) {
  uint32_t exp = 1 << 16;
  s21_decimal value_1 = {{34, 0, 0, exp}};
  s21_decimal result = {0};
  int ret = s21_floor(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 3);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_negative_number) {
  uint32_t exp = 1 << 16;
  s21_decimal value_1 = {{34, 0, 0, MINUS | exp}};
  s21_decimal result = {0};
  int ret = s21_floor(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 4);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], MINUS);
}
END_TEST

START_TEST(s21_truncate_test) {
  uint32_t exp = 2 << 16;
  s21_decimal value_1 = {{342, 0, 0, MINUS | exp}};
  s21_decimal result = {0};
  int ret = s21_truncate(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 3);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], MINUS);
}
END_TEST

START_TEST(s21_round_test) {
  uint32_t exp = 2 << 16;
  s21_decimal value_1 = {{362, 0, 0, exp}};
  s21_decimal result = {0};
  int ret = s21_round(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 4);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

// Arithmetic functions

START_TEST(s21_add_test_1) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 0);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_2) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{12345, 0, 0, 0}};
  s21_set_exp(&value_1, 1);
  s21_set_exp(&value_2, 3);
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1246845);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], value_2.bits[3]);
}
END_TEST

START_TEST(s21_add_test_3) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{12345, 0, 0, 0}};
  s21_set_exp(&value_1, 1);
  s21_set_exp(&value_2, 3);
  s21_set_sign(&value_2, 1);
  s21_decimal dest = {0};
  unsigned int res_exp = 3 << 16;
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1222155);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], res_exp);
}
END_TEST

START_TEST(s21_add_pos_pos) {  // сложение положительных интов
  s21_decimal var1 = {{50, 0, 0, 0}};
  s21_decimal var2 = {{6, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{56, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_pos_posE) {  // сложение положительных со степенью
  s21_decimal var1 = {{50, 0, 0, 0}};
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50777, 3, 0, 0}};
  ch.bits[3] = 3 << 16;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_posE_posE) {  // сложение положительных со степенью
  s21_decimal var1 = {{8666, 5, 0, 0}};
  var1.bits[3] = 5 << 16;
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 10 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{866600777, 500003, 0, 0}};
  ch.bits[3] = 10 << 16;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_neg_neg) {  // сложение отрицательных интов
  s21_decimal var1 = {{50, 0, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 0, 0, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{100, 0, 0, 0}};
  ch.bits[3] = 1 << 31;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_neg_negE) {  // сложение отрицательных со степенью
  s21_decimal var1 = {{50, 0, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 3 << 16;
  var2.bits[3] |= (1 << 31);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50777, 3, 0, 0}};
  ch.bits[3] = 3 << 16;
  ch.bits[3] |= (1 << 31);
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_negE_negE) {  // сложение отрицательных со степенью
  s21_decimal var1 = {{50, 60, 0, 0}};
  var1.bits[3] = 1 << 31;
  var1.bits[3] |= 3 << 16;
  s21_decimal var2 = {{4, 5555, 333, 0}};
  var2.bits[3] = 7 << 16;
  var2.bits[3] |= (1 << 31);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{500004, 605555, 333, 0}};
  ch.bits[3] = 7 << 16;
  ch.bits[3] |= (1 << 31);
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_pos_neg) {  // сложение положительного и отрицательного
                               //(переход в вычитание)
  s21_decimal var1 = {{50, 50, 0, 0}};
  s21_decimal var2 = {{50, 0, 0, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 50, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}

START_TEST(s21_add_0_0) {  // проверка на сложение 0
  s21_decimal var1 = {{0, 0, 0, 0}};
  s21_decimal var2 = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_max) {  // проверка на превышение мантиссы
  s21_decimal var1 = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal var2 = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_sub_pos_pos) {  // вычитание положительных чисел с положительным
                               // в ответе
  s21_decimal var1 = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 50, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_pos_pos2) {  // вычитание положительных чисел с отрицательным
                                // в ответе
  s21_decimal var1 = {{0, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50, 0, 0, 0}};
  ch.bits[3] = 1 << 31;
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_pos_posE) {  // вычитание положительных чисел
  s21_decimal var1 = {{30, 00, 00, 0}};
  s21_decimal var2 = {{55555550, 50, 0, 0}};
  var2.bits[3] = 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{55525550, 50, 0, 0}};
  ch.bits[3] = 3 << 16;
  ch.bits[3] |= (1 << 31);
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_posE_posE) {  // вычитание положительных чисел со степенью
  s21_decimal var1 = {{7, 55, 0, 0}};
  var1.bits[3] = 6 << 16;
  s21_decimal var2 = {{30, 50, 0, 0}};
  var2.bits[3] = 7 << 16;
  s21_decimal ch = {{40, 500, 0, 0}};
  ch.bits[3] = 7 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_neg_neg) {  // вычитание отрицательных чисел
  s21_decimal var1 = {{7, 33, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{10, 55, 0, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal ch = {{3, 22, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_negE_negE) {  // вычитание отрицательных чисел со степенью
  s21_decimal var1 = {{70, 55, 0, 0}};
  var1.bits[3] = 1 << 31;
  var1.bits[3] |= (4 << 16);
  s21_decimal var2 = {{30, 50, 0, 0}};
  var2.bits[3] = 1 << 31;
  var2.bits[3] |= (3 << 16);
  s21_decimal ch = {{230, 445, 0, 0}};
  ch.bits[3] |= (4 << 16);
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_pos_neg) {  // переход в сложение
  s21_decimal var1 = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal ch = {{100, 100, 100, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_neg_pos) {  // переход в сложение
  s21_decimal var1 = {{50, 50, 50, 0}};
  var1.bits[3] = 1 << 31;
  var1.bits[3] |= 3 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  s21_decimal ch = {{50050, 50050, 50050, 0}};
  ch.bits[3] = 1 << 31;
  ch.bits[3] |= 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_0_0) {  // проверка на вычитание 0
  s21_decimal var1 = {{0, 0, 0, 0}};
  s21_decimal var2 = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_mul_test_1) {
  s21_decimal var1 = {{50, 0, 0, MINUS}};
  unsigned int n = 1 << 16;
  s21_decimal var2 = {{32, 0, 0, n}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_mul(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 1600);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 2147549184);
}
END_TEST

START_TEST(mul_int_1) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(mul_int_2) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(mul_int_3) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(mul_int_4) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(mul_float_1) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 327680);
}
END_TEST

START_TEST(mul_float_2) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147811328);
}
END_TEST

START_TEST(mul_float_3) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147811328);
}
END_TEST

START_TEST(mul_float_4) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 327680);
}
END_TEST

START_TEST(mul_neg_inf) {
  s21_decimal value_1 = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 2);
}
END_TEST

START_TEST(mul_bank_round_1) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{342, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 24);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 5335);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 1835008);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(mul_bank_round_2) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{342, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 24);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 5335);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 1835008);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(mul_bank_round_4) {
  s21_decimal value_1 = {{155, 0, 0, 0}};
  s21_decimal value_2 = {{355, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 24);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 5502);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2149318656);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(mul_one) {
  s21_decimal value_1 = {{155, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 155);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_div_test_1) {  // проверка деления на 0
  s21_decimal var1 = {{50, 0, 0, MINUS}};
  unsigned int n = 1 << 16;
  s21_decimal var2 = {{0, 0, 0, n}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_div(var1, var2, &result);
  ck_assert_int_eq(ret, 3);
  ck_assert_uint_eq(result.bits[0], 0);
}
END_TEST

START_TEST(div_int_1) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(div_int_2) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(div_int_3) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(div_int_4) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(div_float_1) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 7);
  s21_set_exp(&value_2, 3);
  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 262144);
}
END_TEST

START_TEST(div_float_2) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_set_exp(&value_1, 3);
  s21_set_exp(&value_2, 7);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 50000);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(div_float_3) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 33);
  s21_set_exp(&value_2, 3);
  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 1835008);
}
END_TEST

START_TEST(div_float_4) {
  s21_decimal value_1 = {{3, 0, 0, 0}};
  s21_decimal value_2 = {{15, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;
  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 65536);
}
END_TEST

START_TEST(div_zero) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 25);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(code, 3);
}
END_TEST

START_TEST(div_one) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 156);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(div_one_1) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{156, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(s21_is_equal_pos_pos) {
  s21_decimal var = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_equal_neg_neg) {
  s21_decimal var = {{50, 50, 50, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_equal_er_pos_neg) {
  s21_decimal var = {{50, 50, 50, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_equal_er) {
  s21_decimal var = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 30, 50, 0}};
  int ret = s21_is_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_not_equal_pos_pos_er) {
  s21_decimal var = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_not_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_not_equal_neg_neg_er) {
  s21_decimal var = {{50, 50, 50, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_not_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_not_equal_pos_neg) {
  s21_decimal var = {{50, 50, 50, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_not_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_not_equal_pos_pos) {
  s21_decimal var = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 30, 50, 0}};
  int ret = s21_is_not_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_equal) {
  s21_decimal var = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_less(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_less_pos_neg) {
  s21_decimal var = {{50, 20, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_less(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_less_neg_pos) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_less(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_pos_pos) {
  s21_decimal var = {{50, 20, 50, 0}};
  s21_decimal var2 = {{50, 700, 660, 0}};
  var2.bits[3] = 1 << 16;
  int ret = s21_is_less(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_pos_pos_er) {
  s21_decimal var = {{50, 20, 50, 0}};
  s21_decimal var2 = {{50, 700, 0, 0}};
  int ret = s21_is_less(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_less_neg_neg) {
  s21_decimal var = {{50, 70, 50, 0}};
  var.bits[3] = 1 << 31;
  var.bits[3] |= 2 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  var2.bits[3] |= 5 << 16;
  int ret = s21_is_less(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_neg_neg_er) {
  s21_decimal var = {{50, 70, 5, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{30, 888, 666, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_less(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_less_or_equal_eq) {
  s21_decimal var = {{500, 700, 50, 0}};
  var.bits[3] = 2 << 16;
  s21_decimal var2 = {{50, 70, 5, 0}};
  var2.bits[3] = 1 << 16;
  int ret = s21_is_less_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_pos_neg) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 2 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 7 << 16;
  var2.bits[3] |= 1 << 31;
  int ret = s21_is_less_or_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_less_or_equal_neg_pos) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 1 << 31;
  var.bits[3] |= 2 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 7 << 16;
  int ret = s21_is_less_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_pos_pos) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 5 << 16;
  s21_decimal var2 = {{50, 700, 660, 0}};
  var2.bits[3] = 1 << 16;
  int ret = s21_is_less_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_pos_pos_er) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 5 << 16;
  s21_decimal var2 = {{50, 700, 660, 0}};
  var2.bits[3] = 10 << 16;
  int ret = s21_is_less_or_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_less_or_equal_neg_neg) {
  s21_decimal var = {{50, 70, 50, 0}};
  var.bits[3] = 1 << 31;
  var.bits[3] |= 3 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  var2.bits[3] |= 5 << 16;
  int ret = s21_is_less_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_neg_neg_er) {
  s21_decimal var = {{50, 70, 5, 0}};
  var.bits[3] = 3 << 16;
  var.bits[3] |= 1 << 31;
  s21_decimal var2 = {{30, 888, 666, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_less_or_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_greater_eq) {
  s21_decimal var = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_greater(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_greater_pos_neg) {
  s21_decimal var = {{50, 20, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_greater(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_greater_neg_pos) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  int ret = s21_is_greater(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_greater_pos_pos) {
  s21_decimal var = {{50, 20, 7000, 0}};
  s21_decimal var2 = {{50, 700, 660, 0}};
  var2.bits[3] = 1 << 16;
  int ret = s21_is_greater(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_greater_pos_pos_er) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 5 << 16;
  s21_decimal var2 = {{50, 700, 333, 0}};
  var2.bits[3] = 3 << 16;
  int ret = s21_is_greater(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_greater_neg_neg) {
  s21_decimal var = {{50, 70, 0, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_greater(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_greater_neg_neg_er) {
  s21_decimal var = {{50, 70, 5, 0}};
  var.bits[3] = 1 << 31;
  s21_decimal var2 = {{30, 888, 0, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_greater(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_greater_or_equal_eq) {
  s21_decimal var = {{500, 700, 50, 0}};
  var.bits[3] = 3 << 16;
  s21_decimal var2 = {{50, 70, 5, 0}};
  var2.bits[3] = 3 << 16;
  int ret = s21_is_greater_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal_pos_neg) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 2 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 7 << 16;
  var2.bits[3] |= 1 << 31;
  int ret = s21_is_greater_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal_neg_pos) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 2 << 16;
  var.bits[3] |= 1 << 31;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 7 << 16;
  int ret = s21_is_greater_or_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_greater_or_equal_pos_pos) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 5 << 16;
  s21_decimal var2 = {{50, 700, 660, 0}};
  var2.bits[3] = 10 << 16;
  int ret = s21_is_greater_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal_pos_pos_er) {
  s21_decimal var = {{50, 20, 50, 0}};
  var.bits[3] = 5 << 16;
  s21_decimal var2 = {{50, 7, 6, 0}};
  var2.bits[3] = 1 << 16;
  int ret = s21_is_greater_or_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_greater_or_equal_neg_neg) {
  s21_decimal var = {{50, 70, 5, 0}};
  var.bits[3] = 3 << 16;
  var.bits[3] |= 1 << 31;
  s21_decimal var2 = {{30, 888, 666, 0}};
  var2.bits[3] = 1 << 31;
  int ret = s21_is_greater_or_equal(var, var2);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal_neg_neg_er) {
  s21_decimal var = {{50, 70, 50, 0}};
  var.bits[3] = 1 << 31;
  var.bits[3] |= 1 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  var2.bits[3] |= 5 << 16;
  int ret = s21_is_greater_or_equal(var, var2);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_int_pos) {
  srand(time(NULL));
  int n = rand();
  int dest = 0;
  s21_decimal var = {0};
  int ret1 = s21_from_int_to_decimal(n, &var);
  int ret2 = s21_from_decimal_to_int(var, &dest);
  ck_assert_int_eq(ret1, 0);
  ck_assert_int_eq(ret2, 0);
  ck_assert_int_eq(n, dest);
}
END_TEST

START_TEST(s21_from_decimal_to_int_neg) {
  srand(time(NULL));
  int n = -rand();
  int dest = 0;
  s21_decimal var = {0};
  int ret1 = s21_from_int_to_decimal(n, &var);
  int ret2 = s21_from_decimal_to_int(var, &dest);
  ck_assert_int_eq(ret1, 0);
  ck_assert_int_eq(ret2, 0);
  ck_assert_int_eq(n, dest);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_1) {
  float src = 123.5;
  s21_decimal dest = {0};
  uint32_t exp = 1 << 16;
  int ret = s21_from_float_to_decimal(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1235);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], exp);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_2) {
  float src = 123.45;
  s21_decimal dest = {0};
  uint32_t exp = 2 << 16;
  int ret = s21_from_float_to_decimal(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 12345);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], exp);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_3) {
  double src = -123.45;
  s21_decimal dest = {0};
  uint32_t exp = 2 << 16;
  int ret = s21_from_float_to_decimal(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 12345);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], exp | MINUS);
}
END_TEST

START_TEST(s21_from_decimal_to_float_test_1) {
  uint32_t exp = 2 << 16;
  s21_decimal src = {{12345, 0, 0, exp}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, 123.45);
}
END_TEST

START_TEST(from_int_to_decimal_1) {
  int a = 7;
  s21_decimal b = {{0, 0, 0, MINUS}};
  int result = s21_from_int_to_decimal(a, &b);
  ck_assert_int_eq(result, 0);
}
END_TEST

// Проверка маски минуса
START_TEST(minus_test) {
  unsigned int n = 1 << 31;
  ck_assert_uint_eq(n, MINUS);
}
END_TEST

// Проверка функции получения бита по индексу
START_TEST(s21_get_bit_test_1) {
  unsigned int bit = 1;
  unsigned int n = bit << 31;
  bool res = s21_get_bit(n, 31);
  ck_assert_uint_eq(res, bit);
}
END_TEST

START_TEST(s21_get_bit_test_2) {
  unsigned int bit = 1;
  unsigned int n = bit << 0;
  unsigned int res = s21_get_bit(n, 0);
  ck_assert_uint_eq(res, bit);
}
END_TEST

START_TEST(s21_get_exp_test) {
  unsigned int n = 5;
  unsigned int exp1 = n << 16;
  s21_decimal src = {{0, 0, 0, exp1}};
  unsigned int exp2 = s21_get_exp(src);
  ck_assert_uint_eq(exp2, n);
}
END_TEST

// Проверка функции установки экспаненты в децимал
START_TEST(s21_set_exp_test_1) {
  unsigned int exp1 = 16;
  s21_decimal src = {0};
  s21_set_exp(&src, exp1);
  unsigned int exp2 = s21_get_exp(src);
  ck_assert_uint_eq(exp2, exp1);
}
END_TEST

START_TEST(s21_set_exp_test_2) {
  unsigned int exp1 = 16;
  s21_decimal src = {{0, 0, 0, MINUS}};
  s21_set_exp(&src, exp1);
  int sign = s21_get_sign(src);
  unsigned int exp2 = s21_get_exp(src);
  ck_assert_uint_eq(exp2, exp1);
  ck_assert_uint_eq(sign, 1);
}
END_TEST

// Проверка функции валидации экспаненты что она меньше 28
START_TEST(s21_check_exp_test_1) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_set_exp(&src, 16);
  int ret = s21_check_exp(src);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_check_exp_test_2) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_set_exp(&src, 29);
  int ret = s21_check_exp(src);
  ck_assert_uint_eq(ret, 1);
}
END_TEST

START_TEST(s21_transfer_to_decimal_test_1) {
  s21_big_decimal src = {{12, 13, 45, 0, 0, 0, 0}, 14};
  s21_decimal res = s21_transfer_to_decimal(src);
  ck_assert_uint_eq(res.bits[0], 12);
  ck_assert_uint_eq(res.bits[1], 13);
  ck_assert_uint_eq(res.bits[2], 45);
  ck_assert_uint_eq(res.bits[3], 14 << 16);
}
END_TEST

START_TEST(s21_transfer_to_big_test_1) {
  s21_decimal src = {{12, 13, 45, 12 << 16}};
  s21_big_decimal res = s21_transfer_to_big(src);
  ck_assert_uint_eq(res.bits[0], 12);
  ck_assert_uint_eq(res.bits[1], 13);
  ck_assert_uint_eq(res.bits[2], 45);
  ck_assert_uint_eq(res.exp, 12);
}
END_TEST

START_TEST(s21_check_overflow_test_1) {
  s21_big_decimal src = {};
  src.bits[0] = (uint64_t)UINT_MAX + 1;
  bool ret = s21_check_overflow(&src);
  ck_assert_uint_eq(src.bits[0], 0);
  ck_assert_uint_eq(src.bits[1], 1);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_check_overflow_test_2) {
  s21_big_decimal src = {};
  src.bits[0] = UINT64_MAX;
  bool ret = s21_check_overflow(&src);
  ck_assert_uint_eq(src.bits[0], UINT_MAX);
  ck_assert_uint_eq(src.bits[1], UINT_MAX);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_check_overflow_test_3) {
  s21_big_decimal src = {};
  src.bits[6] = UINT64_MAX;
  bool ret = s21_check_overflow(&src);
  ck_assert_uint_eq(ret, 1);
}
END_TEST

START_TEST(s21_point_left_test_1) {
  s21_big_decimal src = {};
  src.bits[0] = 1;
  src.bits[1] = UINT_MAX;
  bool ret = s21_point_left(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[0], 10);
  ck_assert_uint_eq(src.bits[1], 4294967286);
  ck_assert_uint_eq(src.bits[2], 9);
  ck_assert_uint_eq(src.exp, 1);
}
END_TEST

START_TEST(s21_point_left_test_2) {
  s21_big_decimal src = {};
  src.bits[6] = UINT64_MAX;
  bool ret = s21_point_left(&src);
  ck_assert_uint_eq(ret, 1);
  ck_assert_uint_eq(src.bits[6], UINT64_MAX);
}
END_TEST

START_TEST(s21_point_left_test_3) {
  s21_big_decimal src = {};
  src.bits[0] = 11;
  bool ret = s21_point_left(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[0], 110);
}
END_TEST

START_TEST(s21_point_right_test_1) {
  s21_big_decimal src = {{0}, 1};
  src.bits[0] = 10;
  uint32_t ret = s21_point_right(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[0], 1);
  ck_assert_uint_eq(src.exp, 0);
}
END_TEST

START_TEST(s21_point_right_test_2) {
  s21_big_decimal src = {{0}, 1};
  src.bits[0] = 1234;
  uint32_t ret = s21_point_right(&src);
  ck_assert_uint_eq(ret, 4);
  ck_assert_uint_eq(src.bits[0], 123);
  ck_assert_uint_eq(src.exp, 0);
}
END_TEST

START_TEST(s21_is_overflow_test_1) {
  s21_big_decimal src = {{0}, 10};
  src.bits[2] = 123456789;
  src.bits[3] = 123456789;
  bool ret = s21_is_overflow(src);
  ck_assert_uint_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_overflow_test_2) {
  s21_big_decimal src = {{0}, 10};
  src.bits[2] = 123456789;
  bool ret = s21_is_overflow(src);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_overflow_test_3) {
  s21_big_decimal src = {{0}, 10};
  src.bits[3] = 1;
  bool ret1 = s21_is_overflow(src);
  uint32_t reminder = s21_point_right(&src);
  bool ret2 = s21_is_overflow(src);
  ck_assert_uint_eq(ret1, 1);
  ck_assert_uint_eq(reminder, 6);
  ck_assert_uint_eq(ret2, 0);
}
END_TEST

START_TEST(s21_big_okrug_test_1) {
  s21_big_decimal src = {{0}, 10};
  src.bits[3] = 123456789;
  bool ret = s21_big_okrug(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[3], 0);
  ck_assert_uint_eq(src.exp, 1);
}
END_TEST

START_TEST(s21_big_okrug_test_2) {
  s21_big_decimal src = {{0}, 10};
  src.bits[3] = 12;
  bool ret = s21_big_okrug(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[3], 0);
  ck_assert_uint_eq(src.bits[0], 3951369912);
  ck_assert_uint_eq(src.exp, 8);
}
END_TEST

START_TEST(s21_normalize_test_1) {
  s21_big_decimal value_1 = {{0}, 3};
  s21_big_decimal value_2 = {{0}, 1};
  value_1.bits[0] = 12345;
  value_2.bits[0] = 12345;
  bool ret = s21_normalize(&value_1, &value_2);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(value_1.bits[0], 12345);
  ck_assert_uint_eq(value_2.bits[0], 1234500);
  ck_assert_uint_eq(value_1.exp, 3);
  ck_assert_uint_eq(value_1.exp, 3);
}
END_TEST

START_TEST(s21_big_add_test_1) {
  s21_big_decimal value_1 = {{0}, 1};
  s21_big_decimal value_2 = {{0}, 1};
  s21_big_decimal result = {};
  value_1.bits[0] = 1;
  value_2.bits[0] = 1;
  bool ret = s21_big_add(value_1, value_2, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(value_1.bits[0], 1);
  ck_assert_uint_eq(value_2.bits[0], 1);
}
END_TEST

START_TEST(s21_big_sub_test_1) {
  s21_big_decimal value_1 = {{0}, 0};
  s21_big_decimal value_2 = {{0}, 0};
  s21_big_decimal result = {};
  value_1.bits[0] = 4;
  value_2.bits[0] = 2;
  s21_big_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], 2);
}
END_TEST

START_TEST(s21_big_sub_test_2) {
  s21_big_decimal value_1 = {{0}, 0};
  s21_big_decimal value_2 = {{0}, 0};
  s21_big_decimal result = {};
  value_1.bits[0] = 78327;
  value_2.bits[0] = 695;
  s21_big_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], 77632);
}
END_TEST

START_TEST(s21_big_sub_test_3) {
  s21_big_decimal value_1 = {{0}, 0};
  s21_big_decimal value_2 = {{0}, 0};
  s21_big_decimal result_big = {{0}, 0};
  s21_decimal result = {};
  value_1.bits[1] = 1;
  value_1.bits[0] = 695;
  value_2.bits[0] = 783745;
  s21_big_sub(value_1, value_2, &result_big);
  result = s21_transfer_to_decimal(result_big);
  ck_assert_uint_eq(result.bits[0], 4294184246);
  ck_assert_uint_eq(result.bits[1], 0);
}
END_TEST

Suite *s21_decimal_suite_create(void) {
  Suite *s21_decimal = suite_create("s21_decimal");
  TCase *s21_another_test = tcase_create("another_test");
  TCase *s21_arithmetic_test = tcase_create("arithmetic_test");
  TCase *s21_comparsion_test = tcase_create("comparsion_test");
  TCase *s21_convertors_test = tcase_create("convertors_test");
  TCase *s21_helper_test = tcase_create("helper_test");
  // Another functions

  tcase_add_test(s21_another_test, s21_negate_test_1);
  tcase_add_test(s21_another_test, s21_negate_test_2);
  tcase_add_test(s21_another_test, s21_floor_test_positive_number);
  tcase_add_test(s21_another_test, s21_floor_test_negative_number);
  tcase_add_test(s21_another_test, s21_truncate_test);
  tcase_add_test(s21_another_test, s21_round_test);

  // Arithmetic functions

  tcase_add_test(s21_arithmetic_test, s21_add_test_1);
  tcase_add_test(s21_arithmetic_test, s21_add_test_2);
  tcase_add_test(s21_arithmetic_test, s21_add_test_3);
  // s21_arithmetic_sub
  tcase_add_test(s21_arithmetic_test, s21_sub_pos_pos);
  tcase_add_test(s21_arithmetic_test, s21_sub_pos_pos2);
  tcase_add_test(s21_arithmetic_test, s21_sub_pos_posE);
  tcase_add_test(s21_arithmetic_test, s21_sub_posE_posE);
  tcase_add_test(s21_arithmetic_test, s21_sub_neg_neg);
  tcase_add_test(s21_arithmetic_test, s21_sub_negE_negE);
  tcase_add_test(s21_arithmetic_test, s21_sub_pos_neg);
  tcase_add_test(s21_arithmetic_test, s21_sub_neg_pos);
  tcase_add_test(s21_arithmetic_test, s21_sub_0_0);
  // s21_arithmetic_add
  tcase_add_test(s21_arithmetic_test, s21_add_pos_pos);
  tcase_add_test(s21_arithmetic_test, s21_add_pos_posE);
  tcase_add_test(s21_arithmetic_test, s21_add_posE_posE);
  tcase_add_test(s21_arithmetic_test, s21_add_neg_neg);
  tcase_add_test(s21_arithmetic_test, s21_add_neg_negE);
  tcase_add_test(s21_arithmetic_test, s21_add_negE_negE);
  tcase_add_test(s21_arithmetic_test, s21_add_pos_neg);
  tcase_add_test(s21_arithmetic_test, s21_add_0_0);
  tcase_add_test(s21_arithmetic_test, s21_add_max);
  // s21_arithmetic_mul
  tcase_add_test(s21_arithmetic_test, s21_mul_test_1);
  tcase_add_test(s21_arithmetic_test, mul_int_1);
  tcase_add_test(s21_arithmetic_test, mul_int_2);
  tcase_add_test(s21_arithmetic_test, mul_int_3);
  tcase_add_test(s21_arithmetic_test, mul_int_4);
  tcase_add_test(s21_arithmetic_test, mul_float_1);
  tcase_add_test(s21_arithmetic_test, mul_float_2);
  tcase_add_test(s21_arithmetic_test, mul_float_3);
  tcase_add_test(s21_arithmetic_test, mul_float_4);
  tcase_add_test(s21_arithmetic_test, mul_neg_inf);
  tcase_add_test(s21_arithmetic_test, mul_bank_round_1);
  tcase_add_test(s21_arithmetic_test, mul_bank_round_2);
  tcase_add_test(s21_arithmetic_test, mul_bank_round_4);
  tcase_add_test(s21_arithmetic_test, mul_one);
  // s21_arithmetic_div
  tcase_add_test(s21_arithmetic_test, s21_div_test_1);
  tcase_add_test(s21_arithmetic_test, div_int_1);
  tcase_add_test(s21_arithmetic_test, div_int_2);
  tcase_add_test(s21_arithmetic_test, div_int_3);
  tcase_add_test(s21_arithmetic_test, div_int_4);
  tcase_add_test(s21_arithmetic_test, div_float_1);
  tcase_add_test(s21_arithmetic_test, div_float_2);
  tcase_add_test(s21_arithmetic_test, div_float_3);
  tcase_add_test(s21_arithmetic_test, div_float_4);
  tcase_add_test(s21_arithmetic_test, div_zero);
  tcase_add_test(s21_arithmetic_test, div_one);
  tcase_add_test(s21_arithmetic_test, div_one_1);

  // Comparsion functions

  // s21_is_equal
  tcase_add_test(s21_comparsion_test, s21_is_equal_pos_pos);
  tcase_add_test(s21_comparsion_test, s21_is_equal_neg_neg);
  tcase_add_test(s21_comparsion_test, s21_is_equal_er_pos_neg);
  tcase_add_test(s21_comparsion_test, s21_is_equal_er);
  // s21_is_not_equal
  tcase_add_test(s21_comparsion_test, s21_is_not_equal_pos_pos_er);
  tcase_add_test(s21_comparsion_test, s21_is_not_equal_neg_neg_er);
  tcase_add_test(s21_comparsion_test, s21_is_not_equal_pos_neg);
  tcase_add_test(s21_comparsion_test, s21_is_not_equal_pos_pos);
  // s21_is_less
  tcase_add_test(s21_comparsion_test, s21_is_less_equal);
  tcase_add_test(s21_comparsion_test, s21_is_less_pos_neg);
  tcase_add_test(s21_comparsion_test, s21_is_less_neg_pos);
  tcase_add_test(s21_comparsion_test, s21_is_less_pos_pos);
  tcase_add_test(s21_comparsion_test, s21_is_less_pos_pos_er);
  tcase_add_test(s21_comparsion_test, s21_is_less_neg_neg);
  tcase_add_test(s21_comparsion_test, s21_is_less_neg_neg_er);
  // s21_is_less_or_equal
  tcase_add_test(s21_comparsion_test, s21_is_less_or_equal_eq);
  tcase_add_test(s21_comparsion_test, s21_is_less_or_equal_pos_neg);
  tcase_add_test(s21_comparsion_test, s21_is_less_or_equal_neg_pos);
  tcase_add_test(s21_comparsion_test, s21_is_less_or_equal_pos_pos);
  tcase_add_test(s21_comparsion_test, s21_is_less_or_equal_pos_pos_er);
  tcase_add_test(s21_comparsion_test, s21_is_less_or_equal_neg_neg);
  tcase_add_test(s21_comparsion_test, s21_is_less_or_equal_neg_neg_er);
  // s21_is_greater
  tcase_add_test(s21_comparsion_test, s21_is_greater_eq);
  tcase_add_test(s21_comparsion_test, s21_is_greater_pos_neg);
  tcase_add_test(s21_comparsion_test, s21_is_greater_neg_pos);
  tcase_add_test(s21_comparsion_test, s21_is_greater_pos_pos);
  tcase_add_test(s21_comparsion_test, s21_is_greater_pos_pos_er);
  tcase_add_test(s21_comparsion_test, s21_is_greater_neg_neg);
  tcase_add_test(s21_comparsion_test, s21_is_greater_neg_neg_er);
  // s21_is_greater_or_equal
  tcase_add_test(s21_comparsion_test, s21_is_greater_or_equal_eq);
  tcase_add_test(s21_comparsion_test, s21_is_greater_or_equal_pos_neg);
  tcase_add_test(s21_comparsion_test, s21_is_greater_or_equal_neg_pos);
  tcase_add_test(s21_comparsion_test, s21_is_greater_or_equal_pos_pos);
  tcase_add_test(s21_comparsion_test, s21_is_greater_or_equal_pos_pos_er);
  tcase_add_test(s21_comparsion_test, s21_is_greater_or_equal_neg_neg);
  tcase_add_test(s21_comparsion_test, s21_is_greater_or_equal_neg_neg_er);

  // Convertors functions

  tcase_add_test(s21_convertors_test, s21_from_decimal_to_int_pos);
  tcase_add_test(s21_convertors_test, s21_from_decimal_to_int_neg);
  tcase_add_test(s21_convertors_test, s21_from_float_to_decimal_test_1);
  tcase_add_test(s21_convertors_test, s21_from_float_to_decimal_test_2);
  tcase_add_test(s21_convertors_test, s21_from_float_to_decimal_test_3);
  tcase_add_test(s21_convertors_test, s21_from_decimal_to_float_test_1);
  tcase_add_test(s21_convertors_test, s21_from_decimal_to_float_test_1);
  tcase_add_test(s21_convertors_test, from_int_to_decimal_1);

  // Helper functions

  tcase_add_test(s21_helper_test, minus_test);
  tcase_add_test(s21_helper_test, s21_get_bit_test_1);
  tcase_add_test(s21_helper_test, s21_get_bit_test_2);
  tcase_add_test(s21_helper_test, s21_get_exp_test);
  tcase_add_test(s21_helper_test, s21_set_exp_test_1);
  tcase_add_test(s21_helper_test, s21_set_exp_test_2);
  tcase_add_test(s21_helper_test, s21_check_exp_test_1);
  tcase_add_test(s21_helper_test, s21_check_exp_test_2);
  tcase_add_test(s21_helper_test, s21_transfer_to_decimal_test_1);
  tcase_add_test(s21_helper_test, s21_transfer_to_big_test_1);
  tcase_add_test(s21_helper_test, s21_check_overflow_test_1);
  tcase_add_test(s21_helper_test, s21_check_overflow_test_2);
  tcase_add_test(s21_helper_test, s21_check_overflow_test_3);
  tcase_add_test(s21_helper_test, s21_point_left_test_1);
  tcase_add_test(s21_helper_test, s21_point_left_test_2);
  tcase_add_test(s21_helper_test, s21_point_left_test_3);
  tcase_add_test(s21_helper_test, s21_point_right_test_1);
  tcase_add_test(s21_helper_test, s21_point_right_test_2);
  tcase_add_test(s21_helper_test, s21_is_overflow_test_1);
  tcase_add_test(s21_helper_test, s21_is_overflow_test_2);
  tcase_add_test(s21_helper_test, s21_is_overflow_test_3);
  tcase_add_test(s21_helper_test, s21_big_okrug_test_1);
  tcase_add_test(s21_helper_test, s21_big_okrug_test_2);
  tcase_add_test(s21_helper_test, s21_normalize_test_1);
  tcase_add_test(s21_helper_test, s21_big_add_test_1);
  tcase_add_test(s21_helper_test, s21_big_sub_test_1);
  tcase_add_test(s21_helper_test, s21_big_sub_test_2);
  tcase_add_test(s21_helper_test, s21_big_sub_test_3);

  suite_add_tcase(s21_decimal, s21_another_test);
  suite_add_tcase(s21_decimal, s21_arithmetic_test);
  suite_add_tcase(s21_decimal, s21_comparsion_test);
  suite_add_tcase(s21_decimal, s21_convertors_test);
  suite_add_tcase(s21_decimal, s21_helper_test);

  return s21_decimal;
}

int main(void) {
  Suite *s21_decimal = s21_decimal_suite_create();
  SRunner *s21_decimal_suite_runner = srunner_create(s21_decimal);
  int failed_cnt;
  srunner_run_all(s21_decimal_suite_runner, CK_NORMAL);
  failed_cnt = srunner_ntests_failed(s21_decimal_suite_runner);
  srunner_free(s21_decimal_suite_runner);

  if (failed_cnt != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}