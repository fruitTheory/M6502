INCLUDES = -I ./include/
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

LIBRARY_DIR =	 -L C:\Programs\SDL\SDL2_image-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2_mixer-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2-2.26.5-mingw\x86_64-w64-mingw32\lib

LINKED_LIBRARIES = -lSDL2  -lSDL2main -lSDL2_mixer -lSDL2_image


# Removed SDL stuff - ${LIBRARY_DIR} ${LINKED_LIBRARIES} 

all: ${OBJECTS} # Include objects program relies on
	${CC} ${FLAGS} ${INCLUDES} ${SOURCES}main.c ${OBJECTS} -o ${BINARIES}main

# Build all - wildcards
${BUILD}%.o:${SOURCES}%.c
	${CC} ${FLAGS} ${INCLUDES} -c $< -o $@

# Clean build folder if needed
clean:
	del "${BUILD}"

RUN_FILE = test_ca65
run:
	${BINARIES}main ${ASM}${RUN_FILE}.bin
	
# Use as65 assembler
as65:
	as65 ${ASM}test_as65.asm
#as65 ${ASM}6502_decimal_test.a65

BASE_NAME = test_ca65
# Use ca65 assembler
ca65:
	ca65 -l ${ASM}${BASE_NAME}.lst ${ASM}${BASE_NAME}.asm 
	ld65 -t none ${ASM}${BASE_NAME}.o -o ${ASM}${BASE_NAME}.bin
#none is the config used
	rm -f ${ASM}${BASE_NAME}.o

TESTING=./_testing/
test:
	gcc ${INCLUDES} ${TESTING}test.c -o ${TESTING}test
	${TESTING}test
