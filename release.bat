@call .\build.bat

@mkdir release
@xcopy document\aafile release\aafile\ /S /Q /Y
@xcopy document\MLT2HTML release\MLT2HTML\ /S /Q /Y

@rem ------------------------------------------------
@rem   AAFileAccesser
@rem ------------------------------------------------
@mkdir .\release\aafile\lib\x86\ .\release\aafile\lib\x64\
@mkdir .\release\aafile\include\
@copy .\bin\Win32\Release\yatools.aafile.lib .\release\aafile\lib\x86\yatools.aafile.lib
@copy .\bin\x64\Release\yatools.aafile.lib .\release\aafile\lib\x64\yatools.aafile.lib
@xcopy .\include\aafile .\release\aafile\include\aafile\ /S /Q /Y
@copy .\include\aatype.h .\release\aafile\include\aatype.h


@rem ------------------------------------------------
@rem   MLT2HTML
@rem ------------------------------------------------
@mkdir .\release\MLT2HTML\libm2h\lib\x86\ .\release\MLT2HTML\libm2h\lib\x64\ release\MLT2HTML\libm2h\include\

@rem libm2h
@copy .\bin\Win32\Release\libM2H.lib .\release\MLT2HTML\libm2h\lib\x86\libM2H.lib
@copy .\bin\Win32\Release\libM2H.dll .\release\MLT2HTML\libm2h\lib\x86\libM2H.dll
@copy .\bin\x64\Release\libM2H.lib .\release\MLT2HTML\libm2h\lib\x64\libM2H.lib
@copy .\bin\x64\Release\libM2H.dll .\release\MLT2HTML\libm2h\lib\x64\libM2H.dll
@copy .\include\m2h.h .\release\MLT2HTML\libm2h\include\m2h.h

@rem mlt2html
@copy .\bin\Win32\Release\MLT2HTML.exe .\release\MLT2HTML\MLT2HTML.exe
@copy .\bin\x64\Release\MLT2HTML.exe .\release\MLT2HTML\MLT2HTML.x64.exe


@call .\cleanup.bat
