@echo ���ɍ��킹�ăo�b�`�t�@�C�����w�肵�Ȃ����Ă��������B
@SETLOCAL 
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"

MSBuild.exe Yatools.net.sln /p:Configuration=Release
MSBuild.exe Yatools.net.sln /p:Configuration=Release
