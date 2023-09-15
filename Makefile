INCLUDES = -I ./include/ -I C:/Programs/SDL/SDL2-2.26.5-mingw/x86_64-w64-mingw32/include/SDL2 \
			-I C:/Programs/SDL/SDL2_ttf-2.20.2/x86_64-w64-mingw32/include/SDL2
BUILD = ./build/
SOURCES = ./src/
BINARIES = ./bin/
FLAGS = -g -Wall -fdiagnostics-color=always #-fsanitize=address
ASM = ./asm/
TESTING = ./testing/
CC = gcc

OBJECTS =	${BUILD}M6502.o \
			${BUILD}M6502_stack.o \
			${BUILD}M6502_memory.o \
			${BUILD}load_binary.o \
			${BUILD}M6502_instructions.o \
			${BUILD}M6502_flags.o \
			${BUILD}M6502_cpu.o \
			${BUILD}check_SDL_version.o \
			${BUILD}M6502_screen.o \
			${BUILD}M6502_general.o 

LIBRARY_DIR =	-L C:\Programs\SDL\SDL2-2.26.5-mingw\x86_64-w64-mingw32\lib \
				-L C:\Programs\SDL\SDL2_ttf-2.20.2\x86_64-w64-mingw32\lib \
				# -L C:\Programs\SDL\SDL2_image-2.6.3\x86_64-w64-mingw32\lib \
				# -L C:\Programs\SDL\SDL2_mixer-2.6.3\x86_64-w64-mingw32\lib 
				 

LINKED_LIBRARIES = -lSDL2  -lSDL2main -lsdl2_ttf #-lSDL2_mixer -lSDL2_image


# Removed SDL stuff - ${LIBRARY_DIR} ${LINKED_LIBRARIES} 

all: ${OBJECTS} # Include objects program relies on
	${CC} ${FLAGS} ${INCLUDES} ${OBJECTS} ${LIBRARY_DIR} ${LINKED_LIBRARIES} ${SOURCES}main.c -o ${BINARIES}main 

# Build all - wildcards
${BUILD}%.o:${SOURCES}%.c
	${CC} ${FLAGS} ${INCLUDES} -c $< -o $@

# Clean build folder if needed
clean:
	del "${BUILD}"

RUN_FILE = game
run:
	${BINARIES}main ${ASM}${RUN_FILE}.bin
	
# Use as65 assembler
as65:
	as65 ${ASM}6502_functional_test.a65
#as65 ${ASM}6502_decimal_test.a65

BASE_NAME = temp_01
TARGET = none
# Use ca65 assembler
ca65:
	ca65 -l ${ASM}${BASE_NAME}.lst ${ASM}${BASE_NAME}.asm 
	ld65 -C ${ASM}${TARGET}.cfg ${ASM}${BASE_NAME}.o -o ${ASM}${BASE_NAME}.bin
	del -f ${ASM}${BASE_NAME}.o
#none is the config type used -C is config path


TESTING=./_testing/
test:
	gcc ${INCLUDES} ${TESTING}test.c -o ${TESTING}test
	${TESTING}test
