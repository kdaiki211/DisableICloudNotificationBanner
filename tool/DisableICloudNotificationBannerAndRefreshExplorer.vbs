Set ws  = CreateObject("Wscript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")
thisDir = fso.getParentFolderName(WScript.ScriptFullName)
command = "cmd /c " & thisDir & "\DisableICloudNotificationBannerAndRefreshExplorer.bat"
ws.run command, vbHide