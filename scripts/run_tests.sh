#! /bin/bash

TESTS_PASSED=0

UNAME=$(uname)
if [ "$UNAME" == "Linux" ] ; then
  exe_search_string='bin/tests/*';
elif [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]] ; then
  exe_search_string='bin/tests/*.exe';
fi

for test_executable in $(find $exe_search_string); do
  # echo "$test_executable"
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
