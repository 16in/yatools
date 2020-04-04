@call .\build.bat

@mkdir release
@xcopy document\MLT2HTML release\MLT2HTML\ /S /Q /Y
@xcopy document\MLT2Image release\MLT2Image\ /S /Q /Y


@rem ------------------------------------------------
@rem   MLT2HTML
@rem ------------------------------------------------
@rem mlt2html
@copy .\MLT2HTML\bin\Release\*.exe .\release\MLT2HTML\
@copy .\MLT2HTML\bin\Release\*.dll .\release\MLT2HTML\


@rem ------------------------------------------------
@rem   MLT2Image
@rem ------------------------------------------------
@rem mlt2image
@copy .\Mlt2Image\bin\Release\*.exe .\release\MLT2Image\
@copy .\Mlt2Image\bin\Release\*.dll .\release\MLT2Image\

@call .\cleanup.bat
