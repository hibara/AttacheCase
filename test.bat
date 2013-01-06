

@rem ƒo[ƒWƒ‡ƒ“”Ô†‚ğæ“¾‚·‚é
for /F "delims=" %%s in ('tools\getver\getver\bin\Release\getver.exe installer\AttacheCase.exe') do @set NUM=%%s

echo %NUM%


@pause

