@cd %~dp0
@echo off

@set APP_PATH=..\build\FortFirewall.exe

@call sign-env-certum.bat

signtool.exe sign /n "%CRT_NAME%" /fd sha256 /tr http://time.certum.pl/ %APP_PATH%
