@echo off

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --archive-type=gz --num-backups=4 --repo-path="F:\#svn\robertomariobenitez" --backup-dir="F:/#svn_backups/robertomariobenitez" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 > run_log_robertomariobenitez.txt

REM F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\robertomariobenitez" --backup-dir="F:/#svn_backups/robertomariobenitez" --carbonite="F:\#svn_backups(Carbonite)" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_robertomariobenitez.txt 2>&1

F:\@utils\svnHotBackups\bin\svnHotBackups.exe --verbose --archive-type=bzip --num-backups=4 --repo-path="F:\#svn\robertomariobenitez" --backup-dir="F:/#svn_backups/robertomariobenitez" --carbonite="P:\#svn_backups" --carbonite-hours=24 --carbonite-files=4 --carbonite-optimize=1 1> run_log_robertomariobenitez.txt 2>&1


exit
