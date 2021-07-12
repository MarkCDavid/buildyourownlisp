#include <check.h>
#include <stdlib.h>

#define PREPARE_SUITE(__name, __suite_runner) \
  extern Suite *__name(void); \
  if(__suite_runner) { \
    srunner_add_suite(__suite_runner, __name()); \
  } else { \
    __suite_runner = srunner_create(__name()); \
  } \

int main(void)
{
  SRunner *suite_runner;
  
  PREPARE_SUITE(lval_suite, suite_runner);
  PREPARE_SUITE(lenv_suite, suite_runner); 

  srunner_run_all(suite_runner, CK_VERBOSE);
  int number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner); 

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}