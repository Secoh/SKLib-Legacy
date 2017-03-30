@echo off

rem Archiving selected projects, directories, and files into tar+bzip2 archive (Windows CMD)
rem
rem Copyright [2016-2017] Secoh
rem
rem Licensed under the GNU General Public License, Version 3 or later versions (the "License")
rem as published by the Free Software Foundation - see http://www.gnu.org/licenses/
rem You may not use this file except in compliance with the License.
rem Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
rem

set joblist=archive_job_list.txt
set archname=DevTree

rem Saves items listed in text file %joblist%
rem and compresses them into tar+bzip2 archives: %archname%_date[_ver].tar.bz2

set run_awk=..\Exe\awk\bin\awk.exe
set run_tar=..\Exe\wtar\wtar.exe
set run_bzip2=..\Exe\bzip2\bin\bzip2.exe

set tempfa=arch_tempfile1.txt
set tempfb=arch_tempfile2.txt

if exist %tempfa% goto :stopa
if exist %tempfb% goto :stopa
goto :conta

:stopa
echo Error, files %tempfa% and %tempfb% must not exist (verify it and delete).
exit

:conta

if exist %joblist% goto :contb
echo Error, file %joblist% must exist.
echo It is standard DOS/Windows text file that contains
echo projects names, other directories, and single files
echo to be stored into the DevTree archive.
echo All characters after # in a line are comments.
exit

:contb

rem construct archive suffix

date /t | %run_awk% -F- "{ print substr($1,3)$2$3 }" > %tempfb%
set /P suffix=<%tempfb%
set suffix=%suffix:~0,-1%
set version=

if exist %archname%_%suffix%.tar goto :mkver
if exist %archname%_%suffix%.tar.bz2 goto :mkver
if exist %archname%_%suffix%_*.tar goto :mkver
if exist %archname%_%suffix%_*.tar.bz2 goto :mkver
goto :contc

:mkver

dir/b %archname%_%suffix%_*.tar* 2>NUL | %run_awk% -F.tar "{ print $1 }" > %tempfa%
%run_awk% -F_ "{ if (($NF ~ /^[0-9]+$/) && ($NF>nn)) nn=$NF; }; END{ print nn+1 }" %tempfa% > %tempfb%
set /P vvv=<%tempfb%
set version=_%vvv%

:contc

rem removes comments after #; removes leading and trailing whitespace; removes empty lines
rem
%run_awk% -F# "{ gsub(/^[ \t]+|[ \t]+$/, substr($1,0,0), $1); if (length($1)>0) print $1 }" %joblist% > %tempfa%

rem process list and add to tar joblist
rem
del %tempfb%
for /F "tokens=*" %%A in (%tempfa%) do call :sub_add %tempfb% %%A
call :sub_add %tempfb% %joblist%

del %tempfa%
rem for /F %%i in (%tempfb%) do set tfsize=%%~zi
rem if %tfsize% gtr 0 goto :contd
if exist %tempfb% goto :contd
rem del %tempfb%
echo Empty job - no projects, directories, or files to process.
exit

:contd

%run_awk% "{ printf($1\""\0\""); }" %tempfb% > %tempfa%
del %tempfb%

rem finally, lets get it running
rem
%run_tar% cf %archname%_%suffix%%version%.tar -T %tempfa% --null -p --exclude=*.suo --numeric-owner >NUL
del %tempfa%

%run_bzip2% -9 %archname%_%suffix%%version%.tar

rem verify
rem
%run_bzip2% -k -d %archname%_%suffix%%version%.tar.bz2
%run_tar% tvf %archname%_%suffix%%version%.tar > %tempfa%
del %archname%_%suffix%%version%.tar

if exist %archname%_%suffix%%version%.tar.bz2 goto :stats
echo Error, %archname%_%suffix%%version%.tar.bz2 is not created.
echo Verify script and files.
rem exit

:stats

call :sub_wrsize %archname%_%suffix%%version%.tar.bz2 %tempfb%
%run_awk% "{ sum+=$3 } END{ print \"Files total: \" sum }" %tempfa% >> %tempfb%

copy %tempfb% archive_self_test.txt >NUL
%run_awk% -F: "{ if (sw) d2=$2; else d1=$2; sw++; } END{ v1=d1*100/d2; v2=d2/d1; print \"Ratio: \" int(100*v1+0.5)/100 \"%%\"; print \"Compression rate: \" int(100*v2+0.5)/100; print \"\" }" %tempfb% >> archive_self_test.txt
del %tempfb%

%run_awk% "{ pk=4+length($3); printf $4 \" \" $5 \" \" substr(\"          \" $3, (pk<11?pk:11)); for (i=6; i<=NF; i++) printf \" \" $i; print \"\" }" %tempfa% >> archive_self_test.txt
del %tempfa%

exit


:sub_wrsize

echo Archive: %~z1 > %2

exit/b


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



 
