#include <check.h>
#include <stdlib.h>
#include "../src/lval/lval.h"
#include "../src/lenv.h"

START_TEST (when__lenv_get__called_with_existing_key_lval_is_returned)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* value = lval_string("letter a");
  lenv* env = lenv_new();
  lenv_put(env, key, value);

  // Act
  lval* returned_value = lenv_get(env, key);

  //Assert
  ck_assert_str_eq(returned_value->string, value->string);

  // Clean
  lval_delete(key);
  lval_delete(value);
  lval_delete(returned_value);
  lenv_delete(env);
}
END_TEST

START_TEST (when__lenv_get__called_with_non_existing_key_error_is_returned)
{
   // Arrange
  lval* key = lval_symbol("a");
  lenv* env = lenv_new();

  // Act
  lval* returned_value = lenv_get(env, key);

  //Assert
  ck_assert_int_eq(returned_value->type, LVAL_ERROR);

  // Clean
  lval_delete(key);
  lval_delete(returned_value);
  lenv_delete(env);
}
END_TEST

START_TEST (when__lenv_get__called_first_searches_in_local_environment)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* local_value = lval_string("local");
  lval* parent_value = lval_string("parent");
  lenv* local = lenv_new();
  lenv* parent = lenv_new();
  
  lenv_put(local, key, local_value);
  lenv_put(parent, key, parent_value);

  local->parent = parent;

  // Act
  lval* returned_value = lenv_get(local, key);

  //Assert
  ck_assert_str_eq(returned_value->string, local_value->string);
  ck_assert_str_ne(returned_value->string, parent_value->string);

  // Clean
  lval_delete(key);
  lval_delete(local_value);
  lval_delete(parent_value);
  lenv_delete(local);
  lenv_delete(parent);
}
END_TEST

START_TEST (when__lenv_get__called_searches_in_parent_environments)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* value = lval_string("value");
  lenv* local = lenv_new();
  lenv* parent0 = lenv_new();
  lenv* parent1 = lenv_new();
  lenv* parent2 = lenv_new();
  
  lenv_put(parent2, key, value);

  local->parent = parent0;
  parent0->parent = parent1;
  parent1->parent = parent2;

  // Act
  lval* returned_value = lenv_get(local, key);

  //Assert
  ck_assert_str_eq(returned_value->string, value->string);

  // Clean
  lval_delete(key);
  lval_delete(value);
  lenv_delete(local);
  lenv_delete(parent0);
  lenv_delete(parent1);
  lenv_delete(parent2);
}
END_TEST

START_TEST (when__lenv_put__called_for_empty_env_lval_is_added)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* value = lval_string("value");
  lenv* env = lenv_new();
  
  // Act
  lenv_put(env, key, value);

  //Assert
  ck_assert_int_eq(env->count, 1);

  // Clean
  lval_delete(key);
  lval_delete(value);
  lenv_delete(env);

}
END_TEST

START_TEST (when__lenv_put__called_with_same_key_only_added_once)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* value = lval_string("value");
  lenv* env = lenv_new();
  
  // Act
  lenv_put(env, key, value);
  lenv_put(env, key, value);

  //Assert
  ck_assert_int_eq(env->count, 1);

  // Clean
  lval_delete(key);
  lval_delete(value);
  lenv_delete(env);

}
END_TEST

START_TEST (when__lenv_put__called_with_same_key_overwrites_old_value)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* old_value = lval_string("old");
  lval* new_value = lval_string("new");
  lenv* env = lenv_new();
  
  // Act
  lenv_put(env, key, old_value);
  lenv_put(env, key, new_value);
  lval *returned_value = lenv_get(env, key);

  //Assert
  ck_assert_str_eq(returned_value->string, new_value->string);

  // Clean
  lval_delete(key);
  lval_delete(old_value);
  lval_delete(new_value);
  lval_delete(returned_value);
  lenv_delete(env);

}
END_TEST

START_TEST (when__lenv_def__called_does_not_overwrite_keys_in_local_lenv)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* value = lval_string("value");
  lenv* local = lenv_new();
  lenv* parent0 = lenv_new();
  lenv* parent1 = lenv_new();
  lenv* parent2 = lenv_new();
  

  local->parent = parent0;
  parent0->parent = parent1;
  parent1->parent = parent2;

  // Act
  lenv_def(local, key, value);
  lval* returned_value = lenv_get(parent2, key);

  //Assert
  ck_assert_str_eq(returned_value->string, value->string);

  // Clean
  lval_delete(key);
  lval_delete(value);
  lenv_delete(local);
  lenv_delete(parent0);
  lenv_delete(parent1);
  lenv_delete(parent2);
}
END_TEST

START_TEST (when__lenv_def__called_lval_is_added_to_last_parent)
{
   // Arrange
  lval* key = lval_symbol("a");
  lval* local_value = lval_string("local");
  lval* parent_value = lval_string("parent");
  lenv* local = lenv_new();
  lenv* parent0 = lenv_new();
  lenv* parent1 = lenv_new();
  lenv* parent2 = lenv_new();

  local->parent = parent0;
  parent0->parent = parent1;
  parent1->parent = parent2;

  lenv_put(local, key, local_value);
  lenv_def(local, key, parent_value);

  // Act
  lval* returned_value = lenv_get(local, key);

  //Assert
  ck_assert_str_eq(returned_value->string, local_value->string);

  // Clean
  lval_delete(key);
  lval_delete(local_value);
  lval_delete(parent_value);
  lenv_delete(local);
  lenv_delete(parent0);
  lenv_delete(parent1);
  lenv_delete(parent2);
}
END_TEST

START_TEST (when__lenv_copy__called_for_empty_env_copy_is_empty)
{
  // Arrange
  lenv* env = lenv_new();

  // Act
  lenv* copy = lenv_copy(env);

  //Assert
  ck_assert_int_eq(copy->count, env->count);

  // Clean
  lenv_delete(copy);
  lenv_delete(env);
}
END_TEST

START_TEST (when__lenv_copy__called_for_non_empty_env_copy_is_non_empty)
{
  // Arrange
  lval* key = lval_symbol("a");
  lval* value = lval_string("letter a");
  lenv* env = lenv_new();
  lenv_put(env, key, value);

  // Act
  lenv* copy = lenv_copy(env);
  lval* copy_value = lenv_get(copy, key);

  //Assert
  ck_assert_int_eq(copy->count, env->count);
  ck_assert_str_eq(copy_value->string, value->string);

  // Clean
  lval_delete(key);
  lval_delete(value);
  lval_delete(copy_value);
  lenv_delete(copy);
  lenv_delete(env);
}
END_TEST


Suite *lenv_suite(void) {
  Suite *suite = suite_create("lenv");

  TCase *testcase_lenv_get = tcase_create("lenv_get");

  tcase_add_test(testcase_lenv_get, when__lenv_get__called_with_existing_key_lval_is_returned);
  tcase_add_test(testcase_lenv_get, when__lenv_get__called_with_non_existing_key_error_is_returned);
  tcase_add_test(testcase_lenv_get, when__lenv_get__called_first_searches_in_local_environment);
  tcase_add_test(testcase_lenv_get, when__lenv_get__called_searches_in_parent_environments);

  suite_add_tcase(suite, testcase_lenv_get);

  TCase *testcase_lenv_put = tcase_create("lenv_put");

  tcase_add_test(testcase_lenv_put, when__lenv_put__called_for_empty_env_lval_is_added);
  tcase_add_test(testcase_lenv_put, when__lenv_put__called_with_same_key_only_added_once);
  tcase_add_test(testcase_lenv_put, when__lenv_put__called_with_same_key_overwrites_old_value);

  suite_add_tcase(suite, testcase_lenv_put);

  TCase *testcase_lenv_def = tcase_create("lenv_def");

  tcase_add_test(testcase_lenv_def, when__lenv_def__called_lval_is_added_to_last_parent);
  tcase_add_test(testcase_lenv_def, when__lenv_def__called_does_not_overwrite_keys_in_local_lenv);

  suite_add_tcase(suite, testcase_lenv_def);

  TCase *testcase_lenv_copy = tcase_create("lenv_copy");

  tcase_add_test(testcase_lenv_copy, when__lenv_copy__called_for_empty_env_copy_is_empty);
  tcase_add_test(testcase_lenv_copy, when__lenv_copy__called_for_non_empty_env_copy_is_non_empty);

  suite_add_tcase(suite, testcase_lenv_copy);

  return suite;
}