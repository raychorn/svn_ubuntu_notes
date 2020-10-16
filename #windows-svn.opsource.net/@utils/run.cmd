@echo off

if exist "F:\@utils\svnHotBackups\bin\@svnHotBackups.exe" move /Y /-Y F:\@utils\svnHotBackups\bin\@svnHotBackups.exe F:\@utils\svnHotBackups\bin\@svnHotBackups.exe

START "svnHotBackups-repo1" /I /HIGH /MIN "F:\@utils\svnHotBackups\run_Repo1.cmd"

START "svnHotBackups-avikohn" /I /HIGH /MIN "F:\@utils\svnHotBackups\run_avikohn.cmd"

START "svnHotBackups-molten" /I /HIGH /MIN "F:\@utils\svnHotBackups\run_molten.cmd"

START "svnHotBackups-robertomariobenitez" /I /HIGH /MIN "F:\@utils\svnHotBackups\run_robertomariobenitez.cmd"
