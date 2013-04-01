@echo off
tigcc TrigTables.c ^
	  Vector.c ^
	  Matrix.c ^
	  Transform.c ^
      -c -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
tigcc TrigTables.o ^
	  Vector.o ^
	  Matrix.o ^
	  Transform.o ^
      -ar -o X3D_Math -Os -Wall -W -Wwrite-strings -ffunction-sections -fdata-sections -DMIN_AMS=100
echo Y | del TrigTables.o ^
	  Vector.o ^
	  Matrix.o ^
	  Transform.o