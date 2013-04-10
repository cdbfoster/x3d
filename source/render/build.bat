@echo off
tigcc Render.c ^
	  RenderFunctions.c ^
	  DisplayMode.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd displaymode
tigcc WireframeDisplayMode.c ^
	  -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd ..
tigcc Render.o ^
	  RenderFunctions.o ^
	  DisplayMode.o ^
	  displaymode\WireframeDisplayMode.o ^
      -ar -o X3D_Render -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
echo Y | del Render.o ^
	  RenderFunctions.o ^
	  DisplayMode.o ^
	  displaymode\WireframeDisplayMode.o