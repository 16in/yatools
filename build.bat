@echo 環境に合わせてバッチファイルを指定しなおしてください。
@SETLOCAL 
@call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

MSBuild.exe yatools.sln /p:Platform=x86 /p:Configuration=Release
MSBuild.exe yatools.sln /p:Platform=x64 /p:Configuration=Release
