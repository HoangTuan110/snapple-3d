#!/bin/bash
emcc src/main.c -Os -Wall ./libraylib.a -I. -I"./raylib.h" -L. -L"./libraylib.a" -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -DPLATFORM_WEB --preload-file res
zip snapple3d.zip index.html a.out.*
