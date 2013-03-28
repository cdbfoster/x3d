@echo off
tigcc TrigTables.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
tigcc TrigTables.o ^
      -ar -o X3D_Math -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
echo Y | del TrigTables.o