/* file: minunit.h */
#define mu_assert(message, test) do { if (!(test)) {return message;} else {asserts_ok++;} } while (0)
#define mu_run_test(test) do { char *message = test(); if (message) {return message;} else {tests_passed++;} } while (0)
extern int tests_run;
