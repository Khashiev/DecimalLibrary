#include "s21_decimal_test.h"

int main(void) {
  int no_failed;
  Suite *s;
  SRunner *runner;

  s = comparison_suite();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return no_failed == 0 ? 0 : 1;
}