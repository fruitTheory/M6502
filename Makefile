INCLUDES = -I ./include/
BUILD = ./build/
OBJECTS = ${BUILD}version_check.o
LIBRARY_DIR =	 -L C:\Programs\SDL\SDL2_image-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2_mixer-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2-2.26.5-mingw\x86_64-w64-mingw32\lib
FLAGS = -g -Wall -fdiagnostics-color=always #-fsanitize=address #If need extra debugging use -> -Wall 
SOURCES = ./src/
BINARIES = ./bin/
LINKED_LIBRARIES = -lSDL2  -lSDL2main -lSDL2_mixer -lSDL2_image

all: ${OBJECTS}# we rely on all these object fields to actually build -> ${OBJECTS}
	gcc  ${FLAGS} ${INCLUDES} ${SOURCES}main.c ${OBJECTS} ${LIBRARY_DIR} ${LINKED_LIBRARIES} -o ${BINARIES}main

${BUILD}version_check.o:${SOURCES}version_check.c
	gcc ${FLAGS} ${INCLUDES} -c $< -o $@ 

clean:
	del ${BUILD}