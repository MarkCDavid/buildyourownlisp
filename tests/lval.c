#include <check.h>
#include "../src/lval.h"

START_TEST (when_lval_pop_called_count_decreases)
{
  // Arrange
  lval *qexpression = lval_qexpression();
  lval_add(qexpression, lval_integer(1));
  lval_add(qexpression, lval_integer(2));

  // Act
  lval *popped = lval_pop(qexpression, 0);

  //Assert
  ck_assert_int_eq(qexpression->count, 1);

  // Clean
  lval_delete(qexpression);
  lval_delete(popped);
}
END_TEST

START_TEST (when_lval_pop_called_returns_lval)
{
  // Arrange
  lval *qexpr = lval_qexpression();
  lval_add(qexpr, lval_integer(1));
  lval_add(qexpr, lval_integer(2));

  // Act
  lval *popped = lval_pop(qexpr, _i);

  //Assert
  ck_assert_int_eq(popped->integer, _i + 1);

  // Clean
  lval_delete(qexpr);
}
END_TEST

Suite *lval_suite(void) {
  Suite *suite = suite_create("Lispy Value");
  TCase *testcase_lval_pop = tcase_create("lval_pop");

  tcase_add_test(testcase_lval_pop, when_lval_pop_called_count_decreases);
  tcase_add_loop_test(testcase_lval_pop, when_lval_pop_called_returns_lval, 0, 2);
  suite_add_tcase(suite, testcase_lval_pop);

  return suite;
}

int main(void)
{
  Suite *suite = lval_suite();
  SRunner *suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}