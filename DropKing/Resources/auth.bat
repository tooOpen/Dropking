CD /D %~dp0
takeown /f * /a /r
icacls * /t /grant:r everyone:f