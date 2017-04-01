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

TEMPFA=arch_tempfile.txt
#TEMPFB=arch_tempfile2.txt

if [ -e $TEMPFA ]; then echo "Error, file $TEMPFA must not exist (verify it and delete)."; exit 1; fi

if [ ! -f $JOBLIST ]; then
  echo "Error, file $JOBLIST must exist."
  echo "It is standard text file that contains projects names, other directories, and single files"
  echo "to be stored into the DevTree archive. All characters after # in a line are comments."
  exit 1
fi

# construct archive suffix

SUFFIX=`date -Idate | awk -F- '{ print substr($1,3)$2$3 }'`
VERSION=

APREF=$ARCHNAME\_$SUFFIX
TARLIST=`ls $APREF*.tar* 2>/dev/null | egrep -e $APREF'(|_.*).tar(|\.bz2)'`
if [ `echo "$TARLIST" | wc -w` -gt 0 ]; then
   VERSION=_`echo "$TARLIST" | awk -F.tar '{ print $1 }' | awk -F$APREF\_ '{ if (($2 ~ /^[0-9]+$/) && ($2>nn)) nn=$2; }; END{ print nn+1 }'`
fi

# remove comments after #; removes leading and trailing whitespace; removes empty lines

for NM in $JOBLIST `awk -F# '{ gsub(/^[ \t]+|[ \t]+$/, "", $1); if (length($1)>0) print $1 }' $JOBLIST`; do

    if [ -d $NM ]; then echo $NM; if [ -f $NM.sln ]; then echo $NM.sln; fi; fi
    if [ -f $NM ]; then echo $NM; fi

done > $TEMPFA

# verify job file

if [ `cat $TEMPFA | wc -l` -eq 0 ]; then echo "Empty job - no projects, directories, or files to process."; exit 1; fi

# finally, lets get it running

tar cf $ARCHNAME\_$SUFFIX$VERSION.tar -T $TEMPFA -p --exclude=\*.suo --numeric-owner > /dev/null
rm $TEMPFA

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



 
