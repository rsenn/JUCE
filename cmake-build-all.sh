#!/bin/sh


cmake_build_all() {
    MYNAME=`basename "$0" .sh`

    INTROJUCER=$(ls -td */*/*/*/{*,*/*,*/*/*}Introjucer{,.exe} 2>/dev/null |head -n1)
    : ${CXX="g++"}

    HOST=`"$CXX" -dumpmachine`
    IFS="
"
    while :; do 
      case "$1" in
	-G) GENERATOR="$2"; shift 2 ;;
        *=*) eval "${1%%=*}=\"\${1#*=}\""; shift ;;
	*) break ;;
      esac
    done

    case "$HOST":`type -p /bin/sh` in
      *mingw32:/bin/sh | *msys:*) : ${GENERATOR="MSYS Makefiles"} ;;
      *mingw32:*) : ${GENERATOR="MinGW Makefiles"} ;;
      *) : ${GENERATOR="Unix Makefiles"} ;;
    esac

    exec 10>&2

    add_args() {
      IFS=" $IFS"
      ARGS="${ARGS:+$ARGS }$*"
      IFS="${IFS#?}"
    }
    change_dir() {
      echo "Entering $1 ..." 1>&10
      cd "$1"
    }

    add_args '${GENERATOR:+-G${IFS}"$GENERATOR"}'
    add_args '-DCMAKE_VERBOSE_MAKEFILE=TRUE'
    add_args '-DCMAKE_BUILD_TYPE=${CONFIG}'

    [ $# -le 0 ] && set -- */*/*.jucer */*/*/*.jucer

    FILES=

    while [ $# -gt 0 ]; do
      F="${1%/}"; shift
      if [ -d "$F" ]; then
	set -- "$F"/*.jucer "$@"
	continue
      fi
      FILES="${FILES:+$FILES$IFS}$F"
    done
    set -- $FILES

    echo "Projects: $*" >&10

    for PROJECT; do

	SOURCEDIR=`dirname "$PROJECT"`
	if grep -q 'projectType="library"' "$PROJECT"; then
	    LIBRARY=true
	else
	    LIBRARY=false
	fi

       (CMAKBLDDIR="Builds/CMake"
	CMAKELISTS="$CMAKBLDDIR/CMakeLists.txt"
	
	    if ! grep -q "<CMAKE" "$PROJECT"; then    
		    (set -x; "${INTROJUCER:-Introjucer}" --add-exporter "CMake" "$PROJECT")
	    fi
	    
	if [ ! -e "$CMAKELISTS" -o "$PROJECT" -nt "$CMAKELISTS" ]; then
	  (set -x; "${INTROJUCER:-Introjucer}" --resave "$PROJECT")
	fi

	set -e
	change_dir "$SOURCEDIR"
	
	set --  `ls -d "$CMAKBLDDIR"/* | grep -v "$CMAKELISTS\$"`
  [ $# -gt 0 ] && (set -x; rm -rf -- "$@" 2>&10 >&10)

	[ "$LIBRARY" = true ] && 
	    add_args '-DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS'

	build_dir() {
	    case "$BUILD_SHARED_LIBS" in
		ON) LIBTYPE="shared" ;;
		OFF) LIBTYPE="static" ;;
	    esac
	    eval "BUILDDIR=$BUILDDIR"

	    mkdir -p "$CMAKBLDDIR/$BUILDDIR"

	   (
	    eval "(set -x; cd '$CMAKBLDDIR/$BUILDDIR'; \${CMAKE-cmake} $ARGS ..)" 2>&1  |tee cmake.log
	    make -C "$CMAKBLDDIR/$BUILDDIR"
      )
	}

	CMD="for CONFIG in ${CONFIG:-Debug Release}; do
	    build_dir
	done"

	if [ "$LIBRARY" = true ]; then
	  BUILDDIR='$CONFIG-$LIBTYPE'
	  CMD=" for BUILD_SHARED_LIBS in ON OFF; do $CMD; done"
	else
	  BUILDDIR='$CONFIG'
	fi

	eval "$CMD") || { echo "Failed! ($?)" >&10; break; }
    done 2>&1 | tee "$MYNAME.log"
}

cmake_build_all "$@"
