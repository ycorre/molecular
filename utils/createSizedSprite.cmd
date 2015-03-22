@ECHO off

::Set directory
SET CURRENTDIR=%cd%

del %CURRENTDIR%\toto.txt

::Get parameters in param.txt
FOR /F "tokens=1-5 delims=;" %%1 IN (param.txt) DO (
	ECHO Name %%1 
	ECHO Max_Width %%2
	ECHO Max_Height %%3
	ECHO Files Width %%4
	ECHO Files Height %%5
	SET NAME=%%1
	SET MAX_WIDTH=%%2
	SET MAX_HEIGHT=%%3
	SET WIDTH=%%4
	SET HEIGHT=%%5
)

::Write the name of all the image files in one text file
setlocal enableDelayedExpansion
for /f "eol=: delims=" %%F in ('dir /b /a-d %NAME%*.png') do (
	set "tname=%%F"
	echo !tname! >> toto.txt
)
 endlocal

::Get the number of files
for /r %%a in (%NAME%*.png) do (
	set /a numberOfFiles+=1
)
echo Find %numberOfFiles% files to concatenate

::Compute number of tiles in a row
set /a fileRow = %MAX_WIDTH% / %WIDTH%
echo max tiles in a row = %fileRow%

::and in a column
if  NOT %MAX_HEIGHT% == 0 (
	set /a maxColumn = %MAX_HEIGHT% / %HEIGHT%
) else (
	set /a maxColumn = 1
)
echo max tiles in a column = %maxColumn%

::Compute number of horizontal sprite we should do
set /a numberOfHSprite = (%numberOfFiles% / %fileRow%)
if %numberOfHSprite% LSS 0 set /a numberOfHSprite = 0
if %numberOfHSprite% GEQ %numberOfFiles% set /a numberOfHSprite = %numberOfHSprite% - 1
set /a tmp = %numberOfHSprite% * %fileRow%
if %tmp% GEQ %numberOfFiles% set /a numberOfHSprite = %numberOfHSprite% - 1
echo Number of horizontal stripes %numberOfHSprite%

::Compute number of different files we should do
if NOT %MAX_HEIGHT% == 0 (
	set /a numberOfFilesToCreate = %numberOfHSprite% / %maxColumn%  
) else (
	set /a numberOfFilesToCreate = 1 
)
echo Number of final files to create %numberOfFilesToCreate%

echo Creating spritesheet with the images named %CURRENTDIR%\%NAME%*.png in directory %CURRENTDIR%

::Creating the horizontal stripes
setlocal enabledelayedexpansion enableextensions
for /L %%i in (0,1,%numberOfHSprite%) do (
	::Compute the starting tile
	set /a start = %%i * %fileRow%
	::Compute the final tile for this row
	set /a end = !start! + %fileRow% - 1
	::If we are over the number of tiles then modify the end
	if !end! geq %numberOfFiles% (
		set /a end = %numberOfFiles% - 1
	)
	echo start !start!
	echo end !end!
	echo "%CURRENTDIR%\%NAME%%%d.png[!start!, !end!]"
	
	::Write in a file the name of the tiles to be processed
	set /a cpt = 0
	for /F "tokens=*" %%A in (toto.txt) do (
		if !cpt! geq !start! (
			if !cpt! leq !end! (
				echo  %%A >> files.txt 
			)
		)
		set /a cpt = !cpt! + 1
	)
	
	convert "@files.txt" +append "%CURRENTDIR%\tmpSprite_%NAME%%%i.png"
	set /a numberOfCreatedFile = %%i
	del %CURRENTDIR%\files.txt
)
set /a numberOfCreatedFile = %numberOfCreatedFile% + 1
echo Created %numberOfCreatedFile% stripes 

::If we have no limit on the height
if %MAX_HEIGHT% == 0 (
	set /a maxColumn = %numberOfCreatedFile% + 1
	set /a numberOfFilesToCreate = %numberOfFilesToCreate% - 1
)

::Assembling the horizontal stripes vertically
for /L %%j in (0,1,%numberOfFilesToCreate%) do (
	::Compute the starting stripe
	set /a start = %%j * %maxColumn%
	echo rStart !start!
	::Compute the final stripe for this row
	set /a end = !start! + %maxColumn% - 1
	::If we are over the number of stripes then modify the end
	if !end! geq %numberOfCreatedFile% (
		set /a end = %numberOfCreatedFile% - 1
	)
	echo rEnd !end!
	echo "%CURRENTDIR%\tmpSprite_%NAME%%%d.png[!start!, !end!]"
	
	convert "%CURRENTDIR%\tmpSprite_%NAME%%%d.png[!start!, !end!]" -append "%CURRENTDIR%\Sprite_%NAME%%%j.png"
)

del %CURRENTDIR%\tmpSprite_%NAME%*.png
del %CURRENTDIR%\toto.txt

ECHO "Done!"
PAUSE
