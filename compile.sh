#!/bin/bash
# "dos2unix" by means of standard utilities:
# sed 's/\r//g' file1 > file2

# Copyright [2016-2017] Secoh
#
# Licensed under the GNU General Public License, Version 3 or later versions (the "License")
# as published by the Free Software Foundation - see <http://www.gnu.org/licenses/>
# You may not use this file except in compliance with the License.
# Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

# compile.sh [-options] target source source ...
# adds up all lib_*, making all names unique
# few exceptions

get_file_path()   # 1 - test name; 2 - current directory; echoes result
{
  if [ -z "$1" -o "${1##*/}" == "$1" ]; then echo $2;
  else echo "${1%/*}"; fi
}

#

if [ -z "$1" -o "$1" == "-HELP" ]; then
  echo Compile project and automatically include libraries
  echo ${0##*/} [-option -option ...] target source source ...
  echo 'Source is directory with *.c, *.cpp, *.h, *.hpp, etc., files'
  echo All files in all sources are compiled and linked into Target
  echo All \"lib_'*'\" subdirectories in ${0##*/} directory
  echo are included into list of Sources with exception of:
  echo lib_zio, lib_matlab"(version-dependent)"
  echo Options are one-word GCC or linker options, except of:
  echo " -HELP    - Show this text"
  echo " -ZIO     - Include lib_zio and local ZLIB support"
  echo " -DLL     - Compile as loadable .so module"
  echo " -MAT     - Support MATLAB MCR and include lib_matlab (not included by default)"
  echo " -OPTOFF  - Exclude optimization (default: for speed)"
  echo " -LINT    - Use extended portability warnings"
  echo " -KEEP    - Do not delete previous compile_history file"
  echo " -NOLIBS  - Do not search for lib directories, use only those expressly given"
  echo " -NOINTEL - Remove CPU-optimization options for Intel-compatible processors"
  exit
fi

# collect options

export CURDIR=`pwd`

cd ${0%/*}/
ROOTDIR=`pwd`
cd $CURDIR

MCRPATH=./_MCR/R2012a/v717/bin

GCCOPT=

COMP_ZIO=
COMP_MAT=
COMP_LINT=
COMP_OPT=yes
COMP_DLL=
COMP_KEEP=
COMP_LIBS=yes
COMP_INTEL=yes

GCCLINT="-pedantic -Wall -W -Wshadow -Wconversion -Wcast-qual"    # extended source check

while [ "${1:0:1}" == "-" ]; do
  if   [ $1 == "-HELP" ]; then echo "Option -HELP must be the only \"parameter\" in command line"; exit 1;
  elif [ $1 == "-ZIO" ]; then COMP_ZIO=yes;
  elif [ $1 == "-MAT" ]; then COMP_MAT=yes;
  elif [ $1 == "-DLL" ]; then COMP_DLL=yes;
  elif [ $1 == "-OPTOFF" ]; then COMP_OPT=;
  elif [ $1 == "-LINT" ]; then COMP_LINT=yes;
  elif [ $1 == "-KEEP" ]; then COMP_KEEP=yes;
  elif [ $1 == "-NOLIBS" ]; then COMP_LIBS=;
  elif [ $1 == "-NOINTEL" ]; then COMP_INTEL=;
  else GCCOPT="$GCCOPT${GCCOPT:+ }$1"; fi
  shift;
  if [ -z "$1" ]; then echo Target name must not be empty; exit 1; fi
done

if [ -n "$COMP_MAT" -a -n "$COMP_DLL" ]; then echo Use of MATLAB support inside loadable module is not supported; exit 1; fi

if [ -n "$COMP_OPT" ]; then GCCOPT="$GCCOPT${GCCOPT:+ }-O3"; fi
if [ -n "$COMP_LINT" ]; then GCCOPT="$GCCOPT${GCCOPT:+ }$GCCLINT"; fi
if [ -n "$COMP_DLL" ]; then GCCOPT="$GCCOPT${GCCOPT:+ }-fPIC"; fi
if [ -n "$COMP_INTEL" ]; then GCCOPT="$GCCOPT${GCCOPT:+ }-msse3"; fi

export GCCOPT

# check Targer

TARGET=$1
shift

if [ -d $TARGET ]; then echo Target is directory name, cannot write file; exit 1; fi
if [ -z "${TARGET##*/}" ]; then echo Target name must not be empty; exit 1; fi
TDIR=`get_file_path $TARGET $CURDIR`
if [ ! -d "$TDIR" ]; then echo Target directory must exist; exit 1; fi
cd $TDIR
TDIR=`pwd`
cd $CURDIR

# collect compilation list

if [ -z "$1" ]; then echo Error, compilation list must not be empty; exit 1; fi

SRCLIST=

for ZZ in $@; do
  if [ ! -d "$ZZ" ]; then echo "Error, Source must be directory"; exit 1; fi
  cd $ZZ
  ZNAME=`pwd`
  cd $CURDIR
  SRCLIST="$SRCLIST${SRCLIST:+ }$ZNAME"
done

if [ -n "$COMP_LIBS" ]; then
  for ZZ in $ROOTDIR/lib_*; do
    LNAME=${ZZ##*/}
    if [ "$LNAME" != lib_zio -a "$LNAME" != lib_matlab ]; then SRCLIST="$SRCLIST${SRCLIST:+ }$ZZ"; fi
  done
fi

if [ -n "$COMP_ZIO" ]; then SRCLIST="$SRCLIST${SRCLIST:+ }$ROOTDIR/lib_zio $ROOTDIR/zlibloc"; fi

if [ -n "$COMP_MAT" ]; then SRCLIST="$SRCLIST${SRCLIST:+ }$ROOTDIR/lib_matlab"; fi

SRCLIST=`for ZZ in $SRCLIST; do echo $ZZ; done | sort | uniq`

VARIPATH=
if [ `for ZZ in $SRCLIST; do get_file_path $ZZ $CURDIR; done | sort | uniq | wc -l` -ne 1 ]; then VARIPATH=yes; fi
export VARIPATH;

# debug
#echo "Root: $ROOTDIR"
#echo "Target: $TARGET; dir: $TDIR"
#echo "VP: $VARIPATH"
#echo $GCCOPT
#echo $SRCLIST
#exit
#

#run compilation
compile_proc ()
{
  CURDIR=`pwd`
  for z in "$@"; do
      if [ -n "$VARIPATH" ]; then echo $z; else echo ${z##*/}; fi

      cd $z
      ( g++ $GCCOPT -c `ls | egrep '.*\.(cpp|c)$'`; if [ $? -ne 0 ]; then date > $TDIR/compile_has_error.txt; fi ) 2>&1 | tee $TDIR/_collect_outp_tmp.txt
      rm stdafx.o 2> /dev/null

      if [ -n "$VARIPATH" ]; then
        for u in `ls *.o`; do mv $u $TDIR/`sed -e 'sQ/Q_Qg' <<< $z`_$u; done
      else
        for u in `ls *.o`; do mv $u $TDIR/${z##*/}\_$u; done
      fi

      cd $CURDIR
      cat $TDIR/_collect_outp_tmp.txt >> $TDIR/compile_history.txt
      rm $TDIR/_collect_outp_tmp.txt
   done
}

#

if [ -z "$COMP_KEEP" ]; then rm $TDIR/compile_history.txt 2> /dev/null
else echo "$@" >> $TDIR/compile_history.txt; fi

rm $TDIR/compile_has_error.txt $TDIR/*.o 2> /dev/null

compile_proc $SRCLIST

if [ -s $TDIR/compile_has_error.txt ]; then
  rm $TDIR/compile_has_error.txt $TDIR/*.o 2> /dev/null
  echo Check compile_history.txt for error log
  exit 1
fi

echo Linking${COMP_MAT:+ w/MCR}...

if [ -n "$COMP_MAT" ]; then
  cd $ROOTDIR/matlab_body_libs
  rm mat_read_2D 2> /dev/null
  $MCRPATH/mex -f $MCRPATH/matopts.sh mat_read_2D.cpp -lpthread $TDIR/*.o 2>&1 | tee $TDIR/compile_history_1.txt
  mv mat_read_2D $TARGET 2> /dev/null
  cd $CURDIR
elif [ -n "$COMP_DLL" ]; then
  TARGETSO=`awk '{ gsub(/\.[^\/\.]*$/,""); print $0".so"; }' <<< $TARGET`
  g++ -shared $GCCOPT -pthread -ldl -Wl,-soname,$TARGETSO.1 -o $TARGETSO $TDIR/*.o 2>&1 | tee $TDIR/compile_history_1.txt
else
  g++ $GCCOPT -rdynamic -pthread -ldl -o $TARGET $TDIR/*.o 2>&1 | tee $TDIR/compile_history_1.txt
fi

cat $TDIR/compile_history_1.txt >> $TDIR/compile_history.txt
rm $TDIR/*.o $TDIR/compile_has_error.txt $TDIR/compile_history_1.txt 2> /dev/null
if [ ! -s $TDIR/compile_history.txt ]; then rm $TDIR/compile_history.txt; else echo Check compile_history.txt for error log; exit 1; fi


