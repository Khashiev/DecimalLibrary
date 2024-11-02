#include "s21_decimal.h"

// Проверка мантиссы на 0
bool s21_is_zero(s21_decimal src) {
  return (src.bits[0] == 0 && src.bits[1] == 0 && src.bits[2] == 0) ? 1 : 0;
}
// Проверка экспоненты
bool s21_check_exp(s21_decimal src) { return (s21_get_exp(src) > 28) ? 1 : 0; }
// Получение exp (scale to 10) в десятичном виде
int s21_get_exp(s21_decimal value) { return (value.bits[3] & EXP) >> 16; }
// Установка exp
void s21_set_exp(s21_decimal *value, uint16_t exp) {
  value->bits[3] |= (exp << 16);
}
// Получение знака
int s21_get_sign(s21_decimal value) { return (value.bits[3] & 1u << 31) != 0; }
// Установка знака
void s21_set_sign(s21_decimal *src, bool sign) {
  src->bits[3] |= sign ? MINUS : 0;
}
// Получение бита по индексу от 0 до 31
bool s21_get_bit(unsigned int src, unsigned int index) {
  return (src >> index) & 1;
}
// Найти бит
int find_bit(s21_decimal src, int index) {
  int mask = 1u << (index % 32);
  return (src.bits[index / 32] & mask) != 0;
}
// Конвертирование s21_decimal в s21_big_decimal
s21_big_decimal s21_transfer_to_big(s21_decimal value) {
  s21_big_decimal res = {};
  uint16_t exp = s21_get_exp(value);
  for (int i = 0; i < 3; i++) res.bits[i] = value.bits[i] & UINT_MAX;
  res.exp = exp;
  return res;
}
// Конвертирование s21_big_decimal в s21_decimal
s21_decimal s21_transfer_to_decimal(s21_big_decimal value) {
  s21_decimal res = {0};
  for (int i = 0; i < 3; i++) res.bits[i] = value.bits[i] & UINT_MAX;
  s21_set_exp(&res, value.exp);
  return res;
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
// Сдвиг влево (exp++), т.е. домножение на 10, не меняя числа.
// При переполнении возвращает 1.
bool s21_point_left(s21_big_decimal *value) {
  bool overflow = 0;
  s21_big_decimal copy = *value;
  for (int i = 0; i < 7; i++) copy.bits[i] *= 10;
  copy.exp++;
  overflow = s21_check_overflow(&copy);
  if (!overflow && value->exp < 28) *value = copy;
  return overflow;
}
// Сдвиг вправо (exp--), т.е. деление на 10, не меняя числа.
uint32_t s21_point_right(s21_big_decimal *src) {
  uint32_t remainder = 0;
  if (s21_get_exp(s21_transfer_to_decimal(*src)) > 0) {
    s21_big_decimal copy = *src;
    for (int i = 6; i >= 0; i--) {
      copy.bits[i] += (uint64_t)remainder << 32;
      remainder = copy.bits[i] % 10;
      copy.bits[i] /= 10;
    }
    copy.exp--;
    *src = copy;
  }
  return remainder;
}
// Проверка переполнения
bool s21_check_overflow(s21_big_decimal *value) {
  uint32_t overflow = 0;
  for (int i = 0; i < 7; i++) {
    value->bits[i] += overflow;
    overflow = value->bits[i] >> 32;
    value->bits[i] &= UINT_MAX;
  }
  return s21_is_overflow(*value);
}
// Прогоняет биты на переполнение
bool s21_is_overflow(s21_big_decimal value) {
  bool res = 0;
  for (int i = 3; i < 7 && !res; i++) res = value.bits[i] ? 1 : 0;
  return res;
}
// Сложение big decimal
bool s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result) {
  bool res = 0;
  s21_big_decimal copy = {{0}, 0};
  if (value_1.exp >= value_2.exp) {
    copy.exp = value_1.exp;
  } else {
    copy.exp = value_2.exp;
  }

  int rem = 0, ovf = 0;
  for (int i = 0; i < 256; i++) {
    rem = s21_get_bit_big(value_1, i) + s21_get_bit_big(value_2, i) + ovf;
    ovf = rem / 2;
    s21_set_bit_big(&copy, i, rem % 2);
  }
  res = s21_check_overflow(&copy);
  if (res) res = s21_big_okrug(&copy);
  *result = copy;

  return res;
}
// Вычитание big decimal
void s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result) {
  s21_big_decimal copy = value_1;
  bool save = 0;

  for (int i = 0; i < 3; i++) {
    copy.bits[i] += save ? ~(value_2.bits[i] + 1) + 1 : ~value_2.bits[i] + 1;
    save = 0;
    if (value_1.bits[i] < value_2.bits[i]) save = 1;
  }
  *result = copy;
}
// Если есть заполненные биты старше 3 и exp != 0, то делим на 10
// Если поделить не получилось, то return 1
bool s21_big_okrug(s21_big_decimal *src) {
  bool res = 0;
  s21_big_decimal copy = *src;
  uint16_t remainders[28] = {0};
  uint16_t i = 0;
  while ((copy.exp && s21_is_overflow(copy)) || copy.exp > 28)
    remainders[i++] = s21_point_right(&copy);
  if (copy.exp == 0 && s21_is_overflow(copy)) {
    res = 1;
  } else {
    copy.bits[0] = s21_bank_round(copy.bits[0], remainders, i);
    *src = copy;
  }
  return res;
}
// Банковское округление
uint32_t s21_bank_round(uint32_t src, uint16_t num[], uint16_t index) {
  uint32_t res = 0;
  if (num[index] > 5) {
    res = src + 1;
  } else if (num[index] < 5) {
    res = src;
  } else {
    while (index)
      if (num[--index]) res = src + 1;
    if (src & 0)  // 1 если четное
      res = src;
    else
      res = src + 1;
  }
  return res;
}
// Установка бита в большом децимале
void s21_set_bit_big(s21_big_decimal *dec, int pos, int n) {
  if (n == 1) {
    dec->bits[pos / 32] |= 1u << (pos % 32);
  } else {
    dec->bits[pos / 32] &= ~(1u << (pos % 32));
  }
}
// Получение бита в большом децимале
int s21_get_bit_big(s21_big_decimal dec, int pos) {
  return ((dec.bits[pos / 32] >> (pos % 32)) & 1u);
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

int get_bit(int num, int ind) { return (num & (1 << ind)) ? 1 : 0; }

int get_sign(int num) { return get_bit(num, 31) ? 1 : 0; }

int set_bit_1(int num, int ind) { return (num | (1 << ind)); }

int set_bit_0(int num, int ind) { return (num & ~(1 << ind)); }

int get_scaling_factor(int value) {
  int result = value & SCALE_MASK;  // маска для коэффа масштабирования
  result = result >> 16;  // убрали 16 нулей лишних
  return result;
}

void divide_10(s21_decimal *value) {
  long long unsigned int mod = 0;
  long long int temp;
  for (int i = 2; i >= 0; i--) {
    temp = (mod << 32) + value->bits[i];
    mod = temp % 10;
    value->bits[i] = temp / 10;
  }
}

void rewrite_decimal(s21_decimal value, s21_decimal *result) {
  result->bits[0] = value.bits[0];
  result->bits[1] = value.bits[1];
  result->bits[2] = value.bits[2];
  result->bits[3] = value.bits[3];
}

void set_scale(unsigned int *scale, int scale_to_set) {
  int sign = get_sign(*scale);
  *scale = scale_to_set;
  *scale <<= 16;
  if (sign) {
    *scale = set_bit_1(*scale, 31);
  } else {
    *scale = set_bit_0(*scale, 31);
  }
}

int get_num(s21_decimal value, int num_to_get) {
  set_scale(&value.bits[3], num_to_get - 1);
  s21_truncate(value, &value);
  return value.bits[0] % 10;
}

int is_round_up(s21_decimal value) {
  int pow = get_scaling_factor(value.bits[3]);
  int save_value = 0;
  if (get_num(value, pow) >= 5) {  // получаем определённую цифру из числа
    save_value += 1;
  }
  return save_value;
}

int is_inverse(s21_decimal value_1, s21_decimal value_2) {
  value_2.bits[3] += MINUS;
  int res_1 = s21_is_less(value_1, value_2) && value_1.bits[3] / MINUS == 0 &&
              value_2.bits[3] / MINUS == 0;
  value_2.bits[3] -= MINUS;
  value_1.bits[3] += MINUS;
  int res_2 = s21_is_less(value_2, value_1) && value_1.bits[3] / MINUS == 1 &&
              value_2.bits[3] / MINUS == 1;
  value_1.bits[3] -= MINUS;
  return (res_1 || res_2);
}

int num_of_degree(s21_decimal value) {
  return (value.bits[3] - MINUS * (value.bits[3] / MINUS)) / pow(2, 16);
}

long value_of_compare_bits(s21_decimal value, int bits_part, int exponent) {
  int sign = pow(-1, value.bits[3] / MINUS);
  int power = pow(10, ((exponent >= 0) ? exponent : 0));
  return ((long)value.bits[bits_part] * sign * power);
}

void case_of_full(long long *save_value, s21_decimal *result) {
  if ((long)result->bits[1] + 1 >= 2 * MINUS) {
    if ((long)result->bits[2] + 1 >= 2 * MINUS) {
      (*save_value) += 1;
    }
    result->bits[2] += 1;
  }
  result->bits[1] += 1;
}

int error_result(int save_value) {
  int error_result = 0;
  if (save_value >= 1) {
    error_result = 1;
  }
  if (save_value <= -1) {
    error_result = 2;
  }
  return error_result;
}

void get_result_by_ok(long long *save_value, s21_decimal value_1,
                      s21_decimal value_2, s21_decimal *result) {
  while ((*save_value || num_of_degree(*result) > 28) &&
         num_of_degree(*result) > 0) {
    int mod_save_value = *save_value % 10;
    *save_value /= 10;
    for (int bits_part = 2; bits_part != -1; bits_part--) {
      long long sub = result->bits[bits_part] + (mod_save_value)*pow(2, 32);
      if (bits_part == 0 &&
          ((sub % 10 == 5 && ((sub / 10) % 10) % 2 == 1) || sub % 10 > 5)) {
        sub += 10;
      }
      mod_save_value = sub % 10;
      result->bits[bits_part] = sub / 10;
      if (sub / 10 >= 2 * MINUS) {
        case_of_full(save_value, result);
      }
    }
    result->bits[3] -= pow(2, 16);
  }
  if (value_1.bits[3] / MINUS == 1 && value_2.bits[3] / MINUS == 1) {
    if (*save_value >= 0) {
      result->bits[3] += MINUS;
    }
    if (*save_value >= 1) {
      *save_value = -1;
    }
  }
}
