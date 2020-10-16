@echo off

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --archive-type=gz --num-backups=4 --repo-path="F:\#svn\avikohn" --backup-dir="F:/#svn_backups/avikohn" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 > run_log_avikohn.txt

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --archive-type=bz2 --num-backups=4 --repo-path="F:\#svn\avikohn" --backup-dir="F:/#svn_backups/avikohn" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 > run_log_avikohn.txt

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --archive-type=zip --num-backups=4 --repo-path="F:\#svn\avikohn" --backup-dir="F:/#svn_backups/avikohn" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 > run_log_avikohn.txt

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --archive-type=ezip --num-backups=4 --repo-path="F:\#svn\avikohn" --backup-dir="F:/#svn_backups/avikohn" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 > run_log_avikohn.txt

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\avikohn" --backup-dir="F:/#svn_backups/avikohn" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_avikohn.txt 2>&1

 F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\avikohn" --backup-dir="F:/#svn_backups/avikohn" --carbonite="P:\#svn_backups" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_avikohn.txt 2>&1


REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --restore="F:\#svn_backups\avikohn\avikohn-2.bzip" --repo-path="F:\#svn_backups\avikohn-restore" > run_log_avikohn.txt

exit
