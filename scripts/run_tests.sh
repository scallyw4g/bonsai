#! /bin/bash

. scripts/preamble.sh

EXIT_CODE=0

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
    EXIT_CODE=$(($EXIT_CODE+1))
  fi
done

if [ "$EXIT_CODE" -eq 0 ]; then
  echo ""
  echo "All Tests Passed"
elif [ "$EXIT_CODE" -eq 1 ]; then
  echo ""
  echo "$EXIT_CODE Test suite failed. Inspect log for details."
else
  echo ""
  echo "$EXIT_CODE Test suites failed. Inspect log for details."
fi

exit $EXIT_CODE
