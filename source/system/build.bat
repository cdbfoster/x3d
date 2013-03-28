@echo off
tigcc EngineParameters.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
tigcc EngineParameters.o ^
      -ar -o X3D_System -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
echo Y | del EngineParameters.o