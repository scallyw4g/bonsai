#! /bin/bash

TESTS_PASSED=0

for test_executable in $(find bin/tests/*); do
  if  $test_executable && [ "$TESTS_PASSED" == "0" ]; then
    TESTS_PASSED=0
  else
    TESTS_PASSED=1
  fi
done


if [ "$TESTS_PASSED" -eq 0 ]; then
  echo ""
  echo "All Tests Passed"
else
  echo ""
  echo "One or more failures.  Inspect log for details."
fi

exit "$TESTS_PASSED"
