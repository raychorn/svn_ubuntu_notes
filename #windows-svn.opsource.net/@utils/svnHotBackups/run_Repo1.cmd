@echo off

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --archive-type=gz --num-backups=4 --repo-path="F:\#svn\repo1" --backup-dir="F:/#svn_backups/repo1" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 > run_log_repo1.txt

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\repo1" --backup-dir="F:/#svn_backups/repo1" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_repo1.txt 2>&1

F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\repo1" --backup-dir="F:/#svn_backups/repo1" --carbonite="P:\#svn_backups" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_repo1.txt 2>&1


exit
