set PATH=C:\msys64\mingw64\bin;%PATH%
set PATH=D:\SDL2\bin;%PATH%

g++ src\main.cpp^
    src\graphics.cpp^
    src\crack.cpp^
    src\input.cpp^
    -std=c++17 -O2 -Wall -g -DWIN32 -Iinclude^
    -ID:/SDL2/include/SDL2^
    -LD:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lwinmm -static-libgcc -static-libstdc++^
    -o patcher.exe
