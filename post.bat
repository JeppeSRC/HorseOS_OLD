@echo off

set file=%~dp0\assign.txt

IF NOT EXIST %file% (
    echo assign.txt doesn't exist.
    goto :eof
)

for /f "tokens=1, 2* delims=," %%A in (%file%) do (
    set letter=%%A
    set vfile=%%B
)

echo > attach.txt select vdisk file=%vfile%
echo >> attach.txt attach vdisk
echo >> attach.txt select partition 2
echo >> attach.txt assign letter=%letter%

echo > detach.txt select vdisk file=%vfile%
echo >> detach.txt detach vdisk

diskpart /s attach.txt

xcopy %~dp0\bin\Release\x64\BOOTX64.EFI %letter%:\EFI\BOOT\ /f /y

diskpart /s detach.txt

del detach.txt
del attach.txt
