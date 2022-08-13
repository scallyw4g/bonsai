Delimeter="$RED-----------------------------------------------------------$WHITE"
Success="$GREEN  Success  $WHITE"
Building="$BLUE  Building $WHITE"
Failed="$RED Failed $WHITE"

if [ "$Platform" == "Linux" ] ; then
  PLATFORM_LINKER_OPTIONS="-lpthread -lX11 -ldl -lGL"
  PLATFORM_DEFINES=""
  PLATFORM_DEFINES="$PLATFORM_DEFINES -DBONSAI_LINUX"
  PLATFORM_DEFINES="$PLATFORM_DEFINES -DBONSAI_INTERNAL"
  # PLATFORM_DEFINES="$PLATFORM_DEFINES -DBONSAI_SLOW"
  PLATFORM_INCLUDE_DIRS=""
  PLATFORM_CXX_OPTIONS="-ggdb"

  # TODO(Jesse): What does -fPIC acutally do?  I found the option documented,
  # but with no explanation of what it's doing.  Apparently it's unsupported on
  # Windows, so hopefully it's not necessary for anything.
  #
  # Turns out that -fPIC turns on rip-relative addressing (among other things?)
  # such that functions work regardless of where they're loaded into memory.
  # This is important (obviously) for dynamically-loaded libs and ASLR.
  #
  # https://clang.llvm.org/docs/ClangCommandLineReference.html
  # @unsupported_fPIC_flag_windows
  #
  SHARED_LIBRARY_FLAGS="-shared -fPIC"
  PLATFORM_EXE_EXTENSION=""
  PLATFORM_LIB_EXTENSION=".so"

elif [[ "$Platform" == "Windows" ]] ; then

  PLATFORM_LINKER_OPTIONS="-lgdi32 -luser32 -lopengl32 -lglu32"
  PLATFORM_DEFINES="-DBONSAI_WIN32"
  PLATFORM_INCLUDE_DIRS=""
  PLATFORM_CXX_OPTIONS="-g -gcodeview"

  # @unsupported_fPIC_flag_windows
  SHARED_LIBRARY_FLAGS="-shared"

  PLATFORM_EXE_EXTENSION=".exe"
  PLATFORM_LIB_EXTENSION=".dll"
else
  echo "Unsupported Platform ($Platform), exiting." && exit 1
fi
