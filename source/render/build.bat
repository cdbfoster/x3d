@echo off
tigcc BackfaceCullMode.c ^
      DisplayMode.c ^
      ProjectionMode.c ^
      Render.c ^
      RenderCamera.c ^
      RenderFunctions.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd displaymode
tigcc WireframeDisplayMode.c ^
      PainterDisplayMode.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd ..
cd projectionmode
tigcc PerspectiveProjectionMode.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
cd ..
tigcc BackfaceCullMode.o ^
      DisplayMode.o ^
      ProjectionMode.o ^
      Render.o ^
      RenderCamera.o ^
      RenderFunctions.o ^
      displaymode\WireframeDisplayMode.o ^
      displaymode\PainterDisplayMode.o ^
      projectionmode\PerspectiveProjectionMode.o ^
      -ar -o X3D_Render -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
echo Y | del BackfaceCullMode.o ^
      DisplayMode.o ^
      ProjectionMode.o ^
      Render.o ^
      RenderCamera.o ^
      RenderFunctions.o ^
      displaymode\WireframeDisplayMode.o ^
      displaymode\PainterDisplayMode.o ^
      projectionmode\PerspectiveProjectionMode.o