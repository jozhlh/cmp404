set FBX2SCN_EXE=..\..\..\tools\fbx2scn\bin\win_x64\fbx2scn.exe
set OUTPUT_DIR=..\..\media

set ASSET_NAME=triceratop
set FBX2SCN_FLAGS=-axis-preset mayaZup -strip-texture-path -texture-extension .png
%FBX2SCN_EXE% -o %OUTPUT_DIR%\%ASSET_NAME%.scn %FBX2SCN_FLAGS% %ASSET_NAME%.fbx

pause