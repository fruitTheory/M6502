INCLUDES = -I ./include/
BUILD = ./build/
SOURCES = ./src/
BINARIES = ./bin/
FLAGS = -g -Wall -fdiagnostics-color=always #-fsanitize=address

OBJECTS =   ${BUILD}check_SDL_version.o \
			${BUILD}MOS_6502.o \
			${BUILD}MOS_6502_stack.o \
			${BUILD}MOS_6502_memory.o \
			${BUILD}MOS_6502_registers.o \
			${BUILD}load_binary.o \

LIBRARY_DIR =	 -L C:\Programs\SDL\SDL2_image-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2_mixer-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2-2.26.5-mingw\x86_64-w64-mingw32\lib

LINKED_LIBRARIES = -lSDL2  -lSDL2main -lSDL2_mixer -lSDL2_image

all: ${OBJECTS} # Include objects program relies on
	gcc  ${FLAGS} ${INCLUDES} ${SOURCES}main.c ${OBJECTS} ${LIBRARY_DIR} ${LINKED_LIBRARIES} -o ${BINARIES}main

# Build all - wildcards
${BUILD}%.o:${SOURCES}%.c
	gcc ${FLAGS} ${INCLUDES} -c $< -o $@  

# Clean build folder if needed
clean:
	del ${BUILD}