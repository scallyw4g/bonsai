#! /bin/bash

. scripts/preamble.sh

TESTS_PASSED=1

if [ "$Platform" == "Linux" ] ; then
  exe_search_string='bin/tests/*';
elif [[ "$Platform" == "Windows" ]] ; then
  exe_search_string='bin/tests/*.exe';
fi

for test_executable in $(find $exe_search_string); do
  # echo "$test_executable"
  # echo "$COLORFLAG"
  if $test_executable $COLORFLAG --log-level LogLevel_Shush == 0; then
    echo -n ""
  else
    TESTS_PASSED=0
  fi
done


if [ "$TESTS_PASSED" -eq 1 ]; then
  echo ""
  echo "All Tests Passed"
else
  echo ""
  echo "One or more failures.  Inspect log for details."
fi

exit "$TESTS_PASSED"
