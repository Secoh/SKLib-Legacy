@echo off
del /s /q *.ncb > nul 2>&1
del /q /s /f _tmp\*.* > nul 2>&1
del /q _Debug\*.ilk > nul 2>&1
del /q _Debug\*.pdb > nul 2>&1
del /q _Debug\*.lib > nul 2>&1
del /q _Debug\*.exp > nul 2>&1
del /q _Release\*.ilk > nul 2>&1
del /q _Release\*.pdb > nul 2>&1
del /q _Release\*.lib > nul 2>&1
del /q _Release\*.exp > nul 2>&1
del /q _x64_Debug\*.ilk > nul 2>&1
del /q _x64_Debug\*.pdb > nul 2>&1
del /q _x64_Debug\*.lib > nul 2>&1
del /q _x64_Debug\*.exp > nul 2>&1
del /q _x64_Release\*.ilk > nul 2>&1
del /q _x64_Release\*.pdb > nul 2>&1
del /q _x64_Release\*.lib > nul 2>&1
del /q _x64_Release\*.exp > nul 2>&1
