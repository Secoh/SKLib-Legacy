#!/bin/bash
# "dos2unix" by means of standard utilities:
# sed 's/\r//g' file1 > file2

# Archiving selected projects, directories, and files into tar+bzip2 archive (Unix)
#
# Copyright [2016-2017] Secoh
#
# Licensed under the GNU General Public License, Version 3 or later versions (the "License")
# as published by the Free Software Foundation - see http://www.gnu.org/licenses/
# You may not use this file except in compliance with the License.
# Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#

JOBLIST=archive_job_list.txt
ARCHNAME=DevTree

# Saves items listed in text file $JOBLIST
# and compresses them into tar+bzip2 archives: $ARCHNAME_date[_ver].tar.bz2

TEMPFA=arch_tempfile1.txt
TEMPFB=arch_tempfile2.txt

if [ -e $TEMPFA -o -e $TEMPFB ]; then echo "Error, files $TEMPFA and $TEMPFB must not exist (verify it and delete)."; exit 1; fi

if [ ! -f $JOBLIST ]; then
  echo "Error, file $JOBLIST must exist."
  echo "It is standard text file that contains projects names, other directories, and single files"
  echo "to be stored into the DevTree archive. All characters after # in a line are comments."
  exit 1
fi

:contb

# construct archive suffix

echo need to change it for Unix date; exit

#date /t | %run_awk% -F- "{ print substr($1,3)$2$3 }" > %tempfb%
#set /P suffix=<%tempfb%
#set suffix=%suffix:~0,-1%
#set version=

if [ `ls $ARCHNAME\_$SUFFIX*.tar* | egrep -e $ARCHNAME\_$SUFFIX'(|\..*).tar(|\.bz2)' | wc -l` -gt 1 ]; then

echo make up correct suffix and version; exit

#dir/b %archname%_%suffix%_*.tar* 2>NUL | %run_awk% -F.tar "{ print $1 }" > %tempfa%
#%run_awk% -F_ "{ if (($NF ~ /^[0-9]+$/) && ($NF>nn)) nn=$NF; }; END{ print nn+1 }" %tempfa% > %tempfb%
#set /P vvv=<%tempfb%
#set version=_%vvv%

fi

# remove comments after #; removes leading and trailing whitespace; removes empty lines

awk -F# '{ gsub(/^[ \t]+|[ \t]+$/, "", $1); if (length($1)>0) print $1 }' $JOBLIST > $TEMPFA

# process list and add to tar joblist

echo replace it for Unix style proc call; exit

#for /F "tokens=*" %%A in (%tempfa%) do call :sub_add %tempfb% %%A
#call :sub_add %tempfb% %joblist%

if [ `cat $TEMPFB | wc -l` -eq 0 ]; then echo "Empty job - no projects, directories, or files to process."; exit 1; fi

# finally, lets get it running

tar cf $ARCHNAME\_$SUFFIX$VERSION.tar -T $TEMPFB -p --exclude=\*.suo --numeric-owner > /dev/null
rm $TEMPFB

bzip2 -9 $ARCHNAME\_$SUFFIX$VERSION.tar

# verify

bzip2 -k -d $ARCHNAME\_$SUFFIX$VERSION.tar.bz2
tar tvf $ARCHNAME\_$SUFFIX$VERSION.tar > $TEMPFA
rm  $ARCHNAME\_$SUFFIX$VERSION.tar

if [ ! -f $ARCHNAME\_$SUFFIX$VERSION.tar.bz2 ]; then
  echo "Error, $ARCHNAME\_$SUFFIX$VERSION.tar.bz2 is not created."
  echo "Verify script and files."
  # exit 1
fi

# collect stats

SIZEARCH=`wc -c < $ARCHNAME\_$SUFFIX$VERSION.tar.bz2`
SIZFILES=`awk '{ sum+=$3 } END{ print sum }' $TEMPFA`

echo "Archive: $SIZEARCH" > archive_self_test.txt
echo "Files Total: $SIZFILES" >> archive_self_test.txt

echo $SIZEARCH $SIZFILES | awk '{ v1=$1*100/$2; v2=$2/$1; print "Ratio: " int(100*v1+0.5)/100 "%"; print "Compression rate: " int(100*v2+0.5)/100 }' >> archive_self_test.txt
echo "" >> archive_self_test.txt

awk '{ pk=4+length($3); printf $4 " " $5 " " substr("          " $3, (pk<11?pk:11)); for (i=6; i<=NF; i++) printf " " $i; print "" }' $TEMPFA >> archive_self_test.txt
rm $TEMPFA

exit




:sub_add

if "%~2" == "" exit/b

for %%i in (%2) do if not exist %%~si\NUL goto :notdir

echo %~2/>> %1

if exist "%~2.sln" echo %~2.sln>> %1
rem if exist "%~2.suo" echo %~2.suo>> %1

exit/b


:notdir
if not exist "%~2" goto :badentry
rem echo Warning: "%2" is a file

echo %~2>> %1

exit/b


:badentry
echo Error, "%~2" is neither project, nor directory, nor file

del %tempfa%
del %tempfb% 2>/NUL

exit



 
