@rem 作業用データ廃棄

@rem @echo off

rd /S /Q bin temp .vs
del /A:-S /S /Q *.user

exit /b
