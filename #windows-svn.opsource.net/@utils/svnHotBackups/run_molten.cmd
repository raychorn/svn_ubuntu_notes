@echo off

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --archive-type=gz --num-backups=4 --repo-path="F:\#svn\molten" --backup-dir="F:/#svn_backups/molten" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 > run_log_molten.txt

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\molten" --backup-dir="F:/#svn_backups/molten" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_molten.txt 2>&1

F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\molten" --backup-dir="F:/#svn_backups/molten" --carbonite="P:\#svn_backups" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_molten.txt 2>&1


exit
