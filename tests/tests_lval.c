#include <check.h>
#include "../src/lval/lval.h"

START_TEST (when__lval_add__called_count_increases)
{
  // Arrange
  lval *qexpression = lval_qexpression();

  // Act
  lval_add(qexpression, lval_integer(1));

  //Assert
  ck_assert_int_eq(qexpression->count, 1);

  // Clean
  lval_delete(qexpression);
}
END_TEST

START_TEST (when__lval_add__called_cells_contain_added_lval)
{
  // Arrange
  lval *qexpression = lval_qexpression();

  // Act
  lval_add(qexpression, lval_integer(1));

  //Assert
  ck_assert_int_eq(qexpression->cell[0]->integer, 1);

  // Clean
  lval_delete(qexpression);
}
END_TEST

START_TEST (when__lval_pop__called_count_decreases)
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

START_TEST (when__lval_pop__called_returns_lval)
{
  // Arrange
  lval *qexpression = lval_qexpression();
  lval_add(qexpression, lval_integer(1));
  lval_add(qexpression, lval_integer(2));

  // Act
  lval *popped = lval_pop(qexpression, _i);

  //Assert
  ck_assert_int_eq(popped->integer, _i + 1);

  // Clean
  lval_delete(popped);
  lval_delete(qexpression);
}
END_TEST

START_TEST (when__lval_pop__called_on_empty_qexpression_returns_NULL)
{
  // Arrange
  lval *qexpression = lval_qexpression();

  // Act
  lval *popped = lval_pop(qexpression, 0);

  //Assert
  ck_assert_ptr_eq(popped, NULL);

  // Clean
  lval_delete(qexpression);
}
END_TEST

START_TEST (when__lval_take__called_returns_lval)
{
  // Arrange
  lval *qexpression = lval_qexpression();
  lval_add(qexpression, lval_integer(1));
  lval_add(qexpression, lval_integer(2));

  // Act
  lval *taken = lval_take(qexpression, _i);
  
  //Assert
  ck_assert_int_eq(taken->integer, _i + 1);

  // Clean
  lval_delete(taken);
}
END_TEST

START_TEST (when__lval_take__called_on_empty_qexpression_returns_NULL)
{
  // Arrange
  lval *qexpr = lval_qexpression();

  // Act
  lval *taken = lval_take(qexpr, 0);

  //Assert
  ck_assert_ptr_eq(taken, NULL);
}
END_TEST

START_TEST (when__lval_join__called_with_empty_qexpressions_returns_empty_qexpression)
{
  // Arrange
  lval *lhs = lval_qexpression();
  lval *rhs = lval_qexpression();

  // Act
  lval *joined = lval_join(lhs, rhs);

  //Assert
  ck_assert_int_eq(joined->count, 0);

  // Clean
  lval_delete(joined);
}
END_TEST

START_TEST (when__lval_join__called_with_non_empty_qexpressions_returns_non_empty_qexpression)
{
  // Arrange
  lval *lhs = lval_qexpression();
  lval_add(lhs, lval_integer(0));

  lval *rhs = lval_qexpression();
  lval_add(rhs, lval_integer(1));

  // Act
  lval *joined = lval_join(lhs, rhs);

  //Assert
  ck_assert_int_eq(joined->count, 2);

  // Clean
  lval_delete(joined);
}
END_TEST

START_TEST (when__lval_join__called_with_non_empty_qexpressions_returns_cells_in_order_of_appearance)
{
  // Arrange
  lval *lhs = lval_qexpression();
  lval_add(lhs, lval_integer(1));

  lval *rhs = lval_qexpression();
  lval_add(rhs, lval_integer(0));

  // Act
  lval *joined = lval_join(lhs, rhs);

  //Assert
  ck_assert_int_eq(joined->cell[0]->integer, 1);
  ck_assert_int_eq(joined->cell[1]->integer, 0);

  // Clean
  lval_delete(joined);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_integer_valid_copy_is_produced)
{
  // Arrange
  lval *integer = lval_integer(5);

  // Act
  lval *copy = lval_copy(integer);

  //Assert
  ck_assert_int_eq(integer->integer, copy->integer);

  // Clean
  lval_delete(integer);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_decimal_valid_copy_is_produced)
{
  // Arrange
  lval *decimal = lval_decimal(5.5);

  // Act
  lval *copy = lval_copy(decimal);

  //Assert
  ck_assert_int_eq(decimal->decimal, copy->decimal);

  // Clean
  lval_delete(decimal);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_symbol_valid_copy_is_produced)
{
  // Arrange
  lval *symbol = lval_symbol("symbol");

  // Act
  lval *copy = lval_copy(symbol);

  //Assert
  ck_assert_str_eq(symbol->symbol, copy->symbol);

  // Clean
  lval_delete(symbol);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_string_valid_copy_is_produced)
{
  // Arrange
  lval *string = lval_string("string");

  // Act
  lval *copy = lval_copy(string);

  //Assert
  ck_assert_str_eq(string->string, copy->string);

  // Clean
  lval_delete(string);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_function_valid_copy_is_produced)
{
  // Arrange
  lval *function = lval_function((void*)1, "function");

  // Act
  lval *copy = lval_copy(function);

  //Assert
  ck_assert_ptr_eq(function->builtin, copy->builtin);
  ck_assert_str_eq(function->builtin_name, copy->builtin_name);

  // Clean
  lval_delete(function);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_error_valid_copy_is_produced)
{
  // Arrange
  lval *error = lval_error("error %i", 5);

  // Act
  lval *copy = lval_copy(error);

  //Assert
  ck_assert_str_eq(error->error, copy->error);

  // Clean
  lval_delete(error);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_sexpression_valid_copy_is_produced)
{
  // Arrange
  lval *sexpression_inner = lval_sexpression();
  lval_add(sexpression_inner, lval_string("hello"));

  lval *sexpression = lval_sexpression();
  lval_add(sexpression, lval_integer(1));
  lval_add(sexpression,lval_decimal(2.2));
  lval_add(sexpression, sexpression_inner);

  // Act
  lval *copy = lval_copy(sexpression);

  //Assert
  ck_assert_int_eq(sexpression->count, copy->count);
  ck_assert_int_eq(sexpression->cell[0]->integer, copy->cell[0]->integer);
  ck_assert_int_eq(sexpression->cell[1]->decimal, copy->cell[1]->decimal);
  ck_assert_int_eq(sexpression->cell[2]->count, copy->cell[2]->count);
  ck_assert_str_eq(sexpression->cell[2]->cell[0]->string, copy->cell[2]->cell[0]->string);

  // Clean
  lval_delete(sexpression);
  free(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_qexpression_valid_copy_is_produced)
{
  // Arrange
  lval *qexpression_inner = lval_qexpression();
  lval_add(qexpression_inner, lval_string("hello"));

  lval *qexpression = lval_qexpression();
  lval_add(qexpression, lval_integer(1));
  lval_add(qexpression,lval_decimal(2.2));
  lval_add(qexpression, qexpression_inner);

  // Act
  lval *copy = lval_copy(qexpression);

  //Assert
  ck_assert_int_eq(qexpression->count, copy->count);
  ck_assert_int_eq(qexpression->cell[0]->integer, copy->cell[0]->integer);
  ck_assert_int_eq(qexpression->cell[1]->decimal, copy->cell[1]->decimal);
  ck_assert_int_eq(qexpression->cell[2]->count, copy->cell[2]->count);
  ck_assert_str_eq(qexpression->cell[2]->cell[0]->string, copy->cell[2]->cell[0]->string);

  // Clean
  lval_delete(qexpression);
  free(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_lambda_valid_copy_is_produced)
{
  // Arrange
  lval *formals = lval_qexpression();
  lval_add(formals, lval_symbol("a"));
  lval_add(formals, lval_symbol("b"));

  lval *body = lval_qexpression();
  lval_add(body, lval_symbol("+"));
  lval_add(body, lval_symbol("a"));
  lval_add(body, lval_symbol("b"));

  lval *lambda = lval_lambda(formals, body);

  // Act
  lval *copy = lval_copy(lambda);

  // Assert
  ck_assert_str_eq(lambda->formals->cell[0]->symbol, copy->formals->cell[0]->symbol);
  ck_assert_str_eq(lambda->formals->cell[1]->symbol, copy->formals->cell[1]->symbol);
  ck_assert_str_eq(lambda->body->cell[0]->symbol, copy->body->cell[0]->symbol);
  ck_assert_str_eq(lambda->body->cell[1]->symbol, copy->body->cell[1]->symbol);
  ck_assert_str_eq(lambda->body->cell[2]->symbol, copy->body->cell[2]->symbol);

  // Clean
  lval_delete(lambda);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_exit_valid_copy_is_produced)
{
  // Arrange
  lval *exit = lval_exit(1);

  // Act
  lval *copy = lval_copy(exit);

  //Assert
  ck_assert_int_eq(exit->exit_code, copy->exit_code);

  // Clean
  lval_delete(exit);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_file_valid_copy_is_produced)
{
  // Arrange
  lval *file = lval_file((FILE *)0, "filename", "rwar");

  // Act
  lval *copy = lval_copy(file);

  //Assert
  ck_assert_ptr_eq(file->file, copy->file);
  ck_assert_str_eq(file->file_name, copy->file_name);
  ck_assert_str_eq(file->mode, copy->mode);

  // Clean
  lval_delete(file);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_copy__called_with_lval_ok_valid_copy_is_produced)
{
  // Arrange
  lval *ok = lval_ok();

  // Act
  lval *copy = lval_copy(ok);

  //Assert
  ck_assert_int_eq(ok->type, copy->type);

  // Clean
  lval_delete(ok);
  lval_delete(copy);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_integer_returns_true)
{
  // Arrange
  lval *lhs = lval_integer(5);
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_decimal_returns_true)
{
  // Arrange
  lval *lhs = lval_decimal(5.5);
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_symbol_returns_true)
{
  // Arrange
  lval *lhs = lval_symbol("symbol");
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_string_returns_true)
{
  // Arrange
  lval *lhs = lval_string("string");
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, lhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_function_returns_true)
{
  // Arrange
  lval *lhs = lval_function((void*)1, "function");
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_error_returns_true)
{
  // Arrange
  lval *lhs = lval_error("error %i", 5);
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_sexpression_returns_true)
{
  // Arrange
  lval *lhs_inner = lval_sexpression();
  lval_add(lhs_inner, lval_string("hello"));

  lval *lhs = lval_sexpression();
  lval_add(lhs, lval_integer(1));
  lval_add(lhs,lval_decimal(2.2));
  lval_add(lhs, lhs_inner);

  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_qexpression_returns_true)
{
  // Arrange
  lval *lhs_inner = lval_qexpression();
  lval_add(lhs_inner, lval_string("hello"));

  lval *lhs = lval_qexpression();
  lval_add(lhs, lval_integer(1));
  lval_add(lhs,lval_decimal(2.2));
  lval_add(lhs, lhs_inner);

  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_lambda_returns_true)
{
  // Arrange
  lval *formals = lval_qexpression();
  lval_add(formals, lval_symbol("a"));
  lval_add(formals, lval_symbol("b"));

  lval *body = lval_qexpression();
  lval_add(body, lval_symbol("+"));
  lval_add(body, lval_symbol("a"));
  lval_add(body, lval_symbol("b"));

  lval *lhs = lval_lambda(formals, body);
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_exit_returns_true)
{
  // Arrange
  lval *lhs = lval_exit(1);
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_file_returns_true)
{
  // Arrange
  lval *lhs = lval_file((FILE *)0, "filename", "rwar");
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST

START_TEST (when__lval_equal__called_with_same_lval_ok_returns_true)
{
  // Arrange
  lval *lhs = lval_ok();
  lval *rhs = lval_copy(lhs);

  // Act
  int result = lval_equal(lhs, rhs);

  //Assert
  ck_assert_int_eq(result, 1);

  // Clean
  lval_delete(lhs);
  lval_delete(rhs);
}
END_TEST


Suite *lval_suite(void) {
  Suite *suite = suite_create("lval");

  TCase *testcase_lval_add = tcase_create("lval_add");

  tcase_add_test(testcase_lval_add, when__lval_add__called_count_increases);
  tcase_add_test(testcase_lval_add, when__lval_add__called_cells_contain_added_lval);

  suite_add_tcase(suite, testcase_lval_add);


  TCase *testcase_lval_pop = tcase_create("lval_pop");

  tcase_add_test(testcase_lval_pop, when__lval_pop__called_count_decreases);
  tcase_add_test(testcase_lval_pop, when__lval_pop__called_on_empty_qexpression_returns_NULL);
  tcase_add_loop_test(testcase_lval_pop, when__lval_pop__called_returns_lval, 0, 2);

  suite_add_tcase(suite, testcase_lval_pop);


  TCase *testcase_lval_take = tcase_create("lval_take");

  tcase_add_test(testcase_lval_take, when__lval_take__called_on_empty_qexpression_returns_NULL);
  tcase_add_loop_test(testcase_lval_take, when__lval_take__called_returns_lval, 0, 2);

  suite_add_tcase(suite, testcase_lval_take);


  TCase *testcase_lval_join = tcase_create("lval_join");

  tcase_add_test(testcase_lval_join, when__lval_join__called_with_empty_qexpressions_returns_empty_qexpression);
  tcase_add_test(testcase_lval_join, when__lval_join__called_with_non_empty_qexpressions_returns_non_empty_qexpression);
  tcase_add_test(testcase_lval_join, when__lval_join__called_with_non_empty_qexpressions_returns_cells_in_order_of_appearance);

  suite_add_tcase(suite, testcase_lval_join);


  TCase *testcase_lval_copy = tcase_create("lval_copy");

  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_integer_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_decimal_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_symbol_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_string_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_function_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_error_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_sexpression_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_qexpression_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_lambda_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_exit_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_file_valid_copy_is_produced);
  tcase_add_test(testcase_lval_copy, when__lval_copy__called_with_lval_ok_valid_copy_is_produced);
  
  suite_add_tcase(suite, testcase_lval_copy);

  TCase *testcase_lval_equals = tcase_create("lval_equals");\
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_integer_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_decimal_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_symbol_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_string_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_function_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_error_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_sexpression_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_qexpression_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_lambda_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_exit_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_file_returns_true);
  tcase_add_test(testcase_lval_equals, when__lval_equal__called_with_same_lval_ok_returns_true);

  suite_add_tcase(suite, testcase_lval_equals);

  return suite;
}
