#! /bin/bash

. scripts/preamble.sh

    # src/poof/preprocessor.cpp                                                                         \
    # src/bonsai_types.h                                                                                           \

    # -D BONSAI_WIN32                                                                                              \
    # -D BONSAI_LINUX                                                                                              \

  LOGLEVEL=LogLevel_Debug
  # LOGLEVEL=LogLevel_Info
  # LOGLEVEL=LogLevel_Error


#   bin/tests/preprocessor \
#     --log-level LogLevel_Error

  # DEBUGFLAG="--do-debug-window"

if [ "$Platform" == "Linux" ] ; then

  bin/preprocessor_current               \
    -D BONSAI_PREPROCESSOR               \
    -D BONSAI_LINUX                      \
    src/poof/preprocessor.cpp \
    -I src

elif [ "$Platform" == "Windows" ] ; then

  bin/preprocessor_current               \
    --log-level $LOGLEVEL                \
    $DEBUGFLAG                           \
    $COLORFLAG                           \
    -D BONSAI_PREPROCESSOR               \
    -D BONSAI_LINUX                      \
    src/poof/preprocessor.cpp \
    -I ./src


else
  echo "Unknown platform"
fi
