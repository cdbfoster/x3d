@echo off
tigcc EngineState.c ^
      System.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
tigcc EngineState.o ^
      System.o ^
      -ar -o X3D_System -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
echo Y | del EngineState.o ^
      System.o