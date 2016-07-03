cd %~dp0\..
DisableICloudNotificationBanner.exe
if %ERRORLEVEL% equ 0 goto okay
if %ERRORLEVEL% neq 0 goto fail

:okay
taskkill /f /im explorer.exe
start explorer.exe
exit /b 0

:fail
exit /b 1