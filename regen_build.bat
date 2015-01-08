CGGM\buildtools\regen\regenBuild.exe "Bowls" "CGGM"
mkdir ..\bin
mkdir ..\bin\win64
mkdir ..\bin\win64\Debug
mkdir ..\bin\win64\Release
xcopy.exe /y "game\lib\bass.dll" "..\bin\win64\Debug\"
xcopy.exe /y "game\lib\bass.dll" "..\bin\win64\Release\"
python Regen_Regen_Build.py %*