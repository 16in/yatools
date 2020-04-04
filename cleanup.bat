@rem 作業用データ廃棄

@rem @echo off

rd /S /Q bin obj .vs
del /A:-S /S /Q *.user

for /d %%d in (*) do (
	pushd %%d
		rd /S /Q bin obj .vs
		del /A:-S /S /Q *.user
	popd
)

exit /b
