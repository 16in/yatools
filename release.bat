@call .\build.bat

@mkdir release
@xcopy document\aafile release\aafile\ /S /Q /Y
@xcopy document\MLT2HTML release\MLT2HTML\ /S /Q /Y
@xcopy document\MLT2PNG release\MLT2PNG\ /S /Q /Y

@rem ------------------------------------------------
@rem   AAFileAccesser
@rem ------------------------------------------------
@mkdir .\release\aafile\lib\x86\ .\release\aafile\lib\x64\
@mkdir .\release\aafile\include\
@copy .\bin\Release\x86\yatools.aafile.lib .\release\aafile\lib\x86\yatools.aafile.lib
@copy .\bin\Release\x64\yatools.aafile.lib .\release\aafile\lib\x64\yatools.aafile.lib
@xcopy .\include\aafile .\release\aafile\include\aafile\ /S /Q /Y
@copy .\include\aatype.h .\release\aafile\include\aatype.h


@rem ------------------------------------------------
@rem   MLT2HTML
@rem ------------------------------------------------
@mkdir .\release\MLT2HTML\libm2h\lib\x86\ .\release\MLT2HTML\libm2h\lib\x64\ release\MLT2HTML\libm2h\include\

@rem libm2h
@copy .\bin\Release\x86\libM2H.lib .\release\MLT2HTML\libm2h\lib\x86\libM2H.lib
@copy .\bin\Release\x86\libM2H.dll .\release\MLT2HTML\libm2h\lib\x86\libM2H.dll
@copy .\bin\Release\x64\libM2H.lib .\release\MLT2HTML\libm2h\lib\x64\libM2H.lib
@copy .\bin\Release\x64\libM2H.dll .\release\MLT2HTML\libm2h\lib\x64\libM2H.dll
@copy .\include\m2h.h .\release\MLT2HTML\libm2h\include\m2h.h

@rem mlt2html
@copy .\bin\Release\x86\MLT2HTML.exe .\release\MLT2HTML\MLT2HTML.exe
@copy .\bin\Release\x64\MLT2HTML.exe .\release\MLT2HTML\MLT2HTML.x64.exe


@rem ------------------------------------------------
@rem   MLT2PNG
@rem ------------------------------------------------
@rem mlt2html
@copy .\bin\Release\x86\MLT2PNG.exe .\release\MLT2PNG\MLT2PNG.exe
@copy .\bin\Release\x64\MLT2PNG.exe .\release\MLT2PNG\MLT2PNG.x64.exe

@call .\cleanup.bat
