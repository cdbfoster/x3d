@echo off
cd line
tigcc X3D_ClipLine.s ^
      X3D_DrawLine.s ^
	  X3D_DrawLine_Clipped.s ^
	  X3D_DrawGrayLine.s ^
	  X3D_DrawGrayLine_Clipped.s ^
	  X3D_DrawHLineWHITE.s ^
	  X3D_DrawHLineBLACK.s ^
	  X3D_DrawGrayHLineWHITE.s ^
	  X3D_DrawGrayHLineLGRAY.s ^
	  X3D_DrawGrayHLineDGRAY.s ^
	  X3D_DrawGrayHLineBLACK.s ^
	  -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd ..\polygon
tigcc X3D_FillConvexPolygon.c ^
      X3D_FillGrayConvexPolygon.c ^
	  -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd ..\screen
tigcc X3D_ClearScreen.s ^
      X3D_FillScreen.s ^
	  X3D_CopyScreen.s ^
	  -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd ..
tigcc line\X3D_ClipLine.o ^
      line\X3D_DrawLine.o ^
	  line\X3D_DrawLine_Clipped.o ^
	  line\X3D_DrawGrayLine.o ^
	  line\X3D_DrawGrayLine_Clipped.o ^
	  line\X3D_DrawHLineWHITE.o ^
	  line\X3D_DrawHLineBLACK.o ^
	  line\X3D_DrawGrayHLineWHITE.o ^
	  line\X3D_DrawGrayHLineLGRAY.o ^
	  line\X3D_DrawGrayHLineDGRAY.o ^
	  line\X3D_DrawGrayHLineBLACK.o ^
	  polygon\X3D_FillConvexPolygon.o ^
      polygon\X3D_FillGrayConvexPolygon.o ^
	  screen\X3D_ClearScreen.o ^
      screen\X3D_FillScreen.o ^
	  screen\X3D_CopyScreen.o ^
	  -ar -o X3D_Draw -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
echo Y | del line\X3D_ClipLine.o ^
      line\X3D_DrawLine.o ^
	  line\X3D_DrawLine_Clipped.o ^
	  line\X3D_DrawGrayLine.o ^
	  line\X3D_DrawGrayLine_Clipped.o ^
	  line\X3D_DrawHLineWHITE.o ^
	  line\X3D_DrawHLineBLACK.o ^
	  line\X3D_DrawGrayHLineWHITE.o ^
	  line\X3D_DrawGrayHLineLGRAY.o ^
	  line\X3D_DrawGrayHLineDGRAY.o ^
	  line\X3D_DrawGrayHLineBLACK.o ^
	  polygon\X3D_FillConvexPolygon.o ^
      polygon\X3D_FillGrayConvexPolygon.o ^
	  screen\X3D_ClearScreen.o ^
      screen\X3D_FillScreen.o ^
	  screen\X3D_CopyScreen.o