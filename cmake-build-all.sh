#!/bin/sh

cmake_build_all() {
    MYNAME=`basename "$0" .sh`

    INTROJUCER=$(ls -td */*/*/*/{*,*/*,*/*/*}Introjucer{,.exe} 2>/dev/null |head -n1)
    : ${CXX="g++"}

    HOST=`"$CXX" -dumpmachine`
    IFS="
"
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
    exec_cmd() {
     (exec  2>&10
      set -x
      "$@")
    }

    while :; do 
      case "$1" in
        -v | --verbose) VERBOSE="true"; shift ;;
        -f | --force) FORCE="true"; shift ;;
	-G) GENERATOR="$2"; shift 2 ;;
        -D) add_args "-D${2}"; shift 2 ;; -D*) add_args "$1"; shift ;;
        *=*) eval "${1%%=*}=\"\${1#*=}\""; shift ;;
	*) break ;;
      esac
    done

    case "$HOST":`type -p /bin/sh` in
      *mingw32:/bin/sh | *msys:*) : ${GENERATOR="MSYS Makefiles"} ;;
      *mingw32:*) : ${GENERATOR="MinGW Makefiles"} ;;
      *) : ${GENERATOR="Unix Makefiles"} ;;
    esac

    [ "$VERBOSE" = true ] && add_args '-DCMAKE_VERBOSE_MAKEFILE=TRUE'
    add_args '${GENERATOR:+-G${IFS}"$GENERATOR"}'
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

    [ "$VERBOSE" = true ] && echo "Projects: $*" >&10

    for PROJECT; do

	if grep -q 'projectType="library"' "$PROJECT"; then
	    LIBRARY=true
	else
	    LIBRARY=false
	fi

       (SOURCEDIR=`dirname "$PROJECT"`
        CMAKEDIR="$SOURCEDIR/Builds/CMake"
	CMAKELISTS="$CMAKEDIR/CMakeLists.txt"
	
	if ! grep -q "<CMAKE" "$PROJECT"; then    
		exec_cmd "${INTROJUCER:-Introjucer}" --add-exporter "CMake" "$PROJECT"
	fi

        [ "$VERBOSE" = true ] && {
	    echo "PROJECT: $PROJECT" >&10
	    echo "CMAKELISTS: $CMAKELISTS" >&10
        }
	    
	if [ "$FORCE" = true -o ! -e "$CMAKELISTS" -o "$PROJECT" -nt "$CMAKELISTS" ]; then
	  exec_cmd "${INTROJUCER:-Introjucer}" --resave "$PROJECT"
	fi

	set -e
	
        if [ "$FORCE" = true ]; then
	    set -- `ls -d "$CMAKEDIR"/* | grep -v "$CMAKELISTS\$"`
	    [ $# -gt 0 ] && exec_cmd rm -rf -- "$@" 
	fi

	[ "$LIBRARY" = true ] && 
	    add_args '-DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS'

	build_dir() {
	    case "$BUILD_SHARED_LIBS" in
		ON) LIBTYPE="shared" ;;
		OFF) LIBTYPE="static" ;;
	    esac
	    eval "BUILDDIR=$SUBDIR"

	    mkdir -p "$CMAKEDIR/$BUILDDIR"

	   (eval "(change_dir '$CMAKEDIR/$BUILDDIR'; exec_cmd \${CMAKE-cmake} $ARGS ..)"
	    exec_cmd make -C "$CMAKEDIR/$BUILDDIR")
	}

	CMD="for CONFIG in ${CONFIG:-Debug Release}; do
	    build_dir
	done"

	if [ "$LIBRARY" = true ]; then
	  SUBDIR='$CONFIG-$LIBTYPE'
	  CMD=" for BUILD_SHARED_LIBS in ON OFF; do $CMD; done"
	else
	  SUBDIR='$CONFIG'
	fi

	eval "$CMD") || { echo "Failed! ($?)" >&10; break; }
    done 2>&1 | tee "$MYNAME.log"
}

cmake_build_all "$@"
