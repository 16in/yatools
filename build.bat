@echo ���ɍ��킹�ăo�b�`�t�@�C�����w�肵�Ȃ����Ă��������B
@SETLOCAL 
@call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

MSBuild.exe yatools.sln /p:Platform=x86 /p:Configuration=Release
MSBuild.exe yatools.sln /p:Platform=x64 /p:Configuration=Release
