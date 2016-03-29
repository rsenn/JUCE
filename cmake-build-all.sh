#!/bin/sh

MYNAME=`basename "$0" .sh`

[ $# -le 0 ] && set -- */*/*.jucer */*/*/*.jucer

for PROJECT; do

    SOURCEDIR=`dirname "$PROJECT"`
    if grep -q 'projectType="library"' "$PROJECT"; then
        LIBRARY=true
    else
        LIBRARY=false
    fi

   (Introjucer --resave "$PROJECT"
    Introjucer --add-exporter "CMake" "$PROJECT"

    cd "$SOURCEDIR"
    rm -rf $(ls -d Builds/CMake/*|grep -v Lists.txt$ )
    

    build_dir() {
        case "$BUILD_SHARED_LIBS" in
            ON) LIBTYPE=Shared ;;
            OFF) LIBTYPE=Static ;;
        esac
        eval "BUILDDIR=$BUILDDIR"

        mkdir -p "Builds/CMake/$BUILDDIR"

       (cd Builds/CMake/$BUILDDIR
        eval "\${CMAKE-cmake} $ARGS" .. 2>&1  |tee cmake.log
        make)
    }

    CMD='for CONFIG in Debug Release; do
        build_dir
    done'


    if [ "$LIBRARY" = true ]; then
      ARGS='-DCMAKE_VERBOSE_MAKEFILE=TRUE -DCONFIG=$CONFIG -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS'
      BUILDDIR='$CONFIG-$LIBTYPE'
      CMD=" for BUILD_SHARED_LIBS in ON OFF; do $CMD; done"
    else
      ARGS='-DCMAKE_VERBOSE_MAKEFILE=TRUE -DCONFIG=$CONFIG' 
      BUILDDIR='$CONFIG'
    fi

    eval "$CMD")
done 2>&1 | tee "$MYNAME.log"
