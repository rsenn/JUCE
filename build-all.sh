#!/bin/sh

[ $# -le 0 ] && set -- */*/*.jucer */*/*/*.jucer

for PROJECT; do

    SOURCEDIR=`dirname "$PROJECT"`

    if grep -q 'projectType="library"' "$PROJECT"; then
      BUILDDIR='$CONFIG-$LIBTYPE'
    else
      BUILDDIR='$CONFIG'
    fi

   (Introjucer --resave "$PROJECT"
    Introjucer --add-exporter "CMake" "$PROJECT"

    cd "$SOURCEDIR"
    rm -rf $(ls -d Builds/CMake/*|grep -v Lists.txt$ )
    
    for BUILD_SHARED_LIBS in ON OFF; do

        for CONFIG in Debug Release; do

            case "$BUILD_SHARED_LIBS" in
                ON) LIBTYPE=Shared ;;
                OFF) LIBTYPE=Static ;;
            esac
            eval "BUILDDIR=$BUILDDIR"

            mkdir -p "Builds/CMake/$BUILDDIR"

           (cd Builds/CMake/$BUILDDIR
            cmake  -DCMAKE_VERBOSE_MAKEFILE=TRUE -DCONFIG=$CONFIG  -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS .. 2>&1  |tee cmake.log
            make)
        done
    done)
done
