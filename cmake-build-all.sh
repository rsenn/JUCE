#!/bin/sh

cmake_build_all() {
    MYNAME=`basename "$0" .sh`

    INTROJUCER=$(ls -td */*/*/*/{*,*/*,*/*/*}Introjucer{,.exe} 2>/dev/null |head -n1)
    : ${CXX="g++"}

    HOST=`"$CXX" -dumpmachine`
    IFS="
"
    trap  'echo "SIGINT exit"; exit 1' INT TERM HUP

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
        -C | --clean) CLEAN="true"; shift ;;
        -v | --verbose) : ${VERBOSE:=0}; VERBOSE=$((VERBOSE+1)); shift ;;
        -f | --force) FORCE="true"; shift ;;
        -j) PARALLEL="$2"; shift 2 ;; -j*)PARALLEL="${1#-j}"; shift ;;
	-G) GENERATOR="$2"; shift 2 ;;
        -D) add_args "-D${2}"; shift 2 ;; -D*) add_args "$1"; shift ;;
        *=*) CMD="${1%%=*}=\"${1#*=}\"";  [ "${VERBOSE:-0}" -gt 0 ] && echo  "$CMD" >&10; eval "$CMD"; shift ;;
	*) break ;;
      esac
    done

    if [ "${VERBOSE:-0}" -gt 1 ]; then
	dump_vars() { 
	    O=; for V in ${@:-BUILDDIR BUILD_TYPE CLEAN CMAKEDIR CMAKELISTS CONFIG FILES FORCE GENERATOR IFS INTROJUCER LIBRARY LIBTYPE PROJECT SOURCEDIR VERBOSE}; do
	       eval 'O="${O:+$O
}$V=\"${'$V'}\""'
	     done; echo "$O" >&10
	}
    else
       dump_vars() { :; }
    fi

    case "$HOST":`type -p /bin/sh` in
      *mingw32:/bin/sh | *msys:*) : ${GENERATOR="MSYS Makefiles"} ;;
      *mingw32:*) : ${GENERATOR="MinGW Makefiles"} ;;
      *) : ${GENERATOR="Unix Makefiles"} ;;
    esac

    [ "${VERBOSE:-0}" -gt 1 ] && add_args '-DCMAKE_VERBOSE_MAKEFILE=TRUE'
    add_args '${GENERATOR:+-G "$GENERATOR"}'
    add_args '${JUCE_LIBRARY:+-DJUCE_LIBRARY="$JUCE_LIBRARY"}'
    add_args '-DCONFIG="${BUILD_TYPE}"'

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

    [ "${VERBOSE:-0}" -gt 0 ] && echo "Projects: $*" >&10

    for PROJECT; do

	grep -q 'projectType="library"' "$PROJECT" && LIBRARY=true || LIBRARY=false
	grep -qi 'install\s*('  "$PROJECT" && INSTALLABLE=true || INSTALLABLE=false

       (SOURCEDIR=`dirname "$PROJECT"`
        CMAKEDIR="$SOURCEDIR/Builds/CMake"
	CMAKELISTS="$CMAKEDIR/CMakeLists.txt"
	
        if [ "$CLEAN" != true ]; then
	    if ! grep -q "<CMAKE" "$PROJECT"; then    
		    exec_cmd "${INTROJUCER:-Introjucer}" --add-exporter "CMake" "$PROJECT"
	    fi

		

            dump_vars FORCE CMAKELISTS PROJECT
	    if [ "$FORCE" = true -o ! -e "$CMAKELISTS" -o "$PROJECT" -nt "$CMAKELISTS" ]; then
	      exec_cmd "${INTROJUCER:-Introjucer}" --resave "$PROJECT"
	    fi

	    set -e
	fi
	
        dump_vars FORCE CLEAN LIBRARY BUILD_SHARED_LIBS 

        if [ "$FORCE" = true -o "$CLEAN" = true ]; then
	    set -- `ls -d "$CMAKEDIR"/* | grep -v "$CMAKELISTS\$"`
	    [ $# -gt 0 ] && exec_cmd rm -rf -- "$@" 
	fi

        [ "$CLEAN" = true ] && exit 0

	
	
	build_dir() {
	    case "$BUILD_SHARED_LIBS" in
		ON) LIBTYPE="shared" ;;
		OFF) LIBTYPE="static" ;;
	    esac
	    case "$STATIC_LINK" in
		OFF) LINKTYPE="shared" ;;
		ON) LINKTYPE="static" ;;
	    esac
	    eval "BUILDDIR=$SUBDIR"
            WORKDIR="$CMAKEDIR/$BUILDDIR"

	    mkdir -p "$WORKDIR"
            dump_vars BUILDDIR  WORKDIR

            # --- configure --------------
	   (change_dir "$WORKDIR";  eval "exec_cmd \${CMAKE-cmake} $ARGS ..")

<<<<<<< 184266e3c26e959bf61a9741f2b41572ed615708
            # --- build ------------------
            set make -C "$WORKDIR" 
	    exec_cmd "$@"

            # --- install ----------------
            if [ "$INSTALLABLE" = true ] ; then
                set -- "$@" install
                 [ -n "$DESTDIR" ] && set -- "$@" DESTDIR="$DESTDIR"

                exec_cmd "$@"
            fi
	}

	CMD="for BUILD_TYPE in ${CONFIG:-Debug Release}; do
=======
	   (eval "(change_dir '$CMAKEDIR/$BUILDDIR'; exec_cmd \${CMAKE-cmake} $ARGS ..)"
	    exec_cmd make ${PARALLEL:+-j$PARALLEL} -C "$CMAKEDIR/$BUILDDIR")
	}

	CMD="for CONFIG in ${CONFIG:-Release Debug}; do
>>>>>>> ...
	    build_dir
	done"

	if [ "$LIBRARY" = true ]; then
	  SUBDIR='$BUILD_TYPE-$LIBTYPE'
	  CMD=" for BUILD_SHARED_LIBS in ON OFF; do $CMD; done"
          add_args '-DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS'
        elif [ "$LIBRARY" != true ]; then
          SUBDIR='$BUILD_TYPE-$LINKTYPE'
	  CMD="for STATIC_LINK in OFF ON; do $CMD; done"
	  add_args '-DSTATIC_LINK=$STATIC_LINK'
        fi
         IFS="$IFS;,+ "
	eval "$CMD") || { echo "Failed! ($?)" >&10; break; }
    done 2>&1 | tee "$MYNAME.log"
}

cmake_build_all "$@"
