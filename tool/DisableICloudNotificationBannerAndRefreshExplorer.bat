DisableICloudNotificationBanner.exe
if %ERRORLEVEL% equ 0 goto okay
if %ERRORLEVEL% neq 0 goto fail

:okay
taskkill /f /im explorer.exe
start explorer.exe

:fail
exit /b 1