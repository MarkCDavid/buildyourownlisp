#include <check.h>
#include <stdlib.h>

#define PREPARE_SUITE(__name) \
  extern Suite *__name(void); \
  int run_##__name () { \
    Suite *suite = __name(); \
    SRunner *suite_runner = srunner_create(suite); \
    srunner_run_all(suite_runner, CK_NORMAL); \
    int number_failed = srunner_ntests_failed(suite_runner); \
    srunner_free(suite_runner); \
    return number_failed; \
  };


#define RUN_SUITE(__name, __tally) __tally += run_##__name ();


PREPARE_SUITE(lval_suite);

int main(void)
{
  int number_failed = 0;

  RUN_SUITE(lval_suite, number_failed);
  
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}