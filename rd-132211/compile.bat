@echo off
cls
mkdir build

set flags=-g -Iinclude -x c -lglfw3 -lz -lopengl32 -lgdi32 -lglu32 -m64
set o=-o build/
set gcc=gcc

echo Building src/java
echo - ArrayList
%gcc% -c src/java/ArrayList.c %flags% %o%ArrayList.o
echo - FloatBuffer
%gcc% -c src/java/FloatBuffer.c %flags% %o%FloatBuffer.o
echo - IntBuffer
%gcc% -c src/java/IntBuffer.c %flags% %o%IntBuffer.o
echo - system
%gcc% -c src/java/system.c %flags% %o%system.o
echo - time
%gcc% -c src/java/time.c %flags% %o%time.o

echo Building src/phys
echo - AABB
%gcc% -c src/phys/AABB.c %flags% %o%AABB.o

echo Building src/level
echo - chunk
%gcc% -c src/level/chunk.c %flags% %o%chunk.o
echo - frustum
%gcc% -c src/level/frustum.c %flags% %o%frustum.o
echo - level
%gcc% -c src/level/level.c %flags% %o%level.o
echo - levelRenderer
%gcc% -c src/level/levelRenderer.c %flags% %o%levelRenderer.o
echo - tesselator
%gcc% -c src/level/tesselator.c %flags% %o%tesselator.o
echo - tile
%gcc% -c src/level/tile.c %flags% %o%tile.o

echo Building src/
echo - hitResult
%gcc% -c src/hitResult.c %flags% %o%hitResult.o
echo - player
%gcc% -c src/player.c %flags% %o%player.o
echo - textures
%gcc% -c src/textures.c %flags% %o%textures.o
echo - timer
%gcc% -c src/timer.c %flags% %o%timer.o
echo - RubyDung
%gcc% -c src/RubyDung.c %flags% %o%RubyDung.o

cd build

echo Compiling
%gcc% -o ../Minecraft.exe ArrayList.o FloatBuffer.o IntBuffer.o system.o time.o AABB.o chunk.o frustum.o level.o levelRenderer.o tesselator.o tile.o hitResult.o player.o textures.o timer.o RubyDung.o %flags%

cd ..