INCLUDES = -I ./include/
BUILD = ./build/
SOURCES = ./src/
BINARIES = ./bin/
FLAGS = -g -Wall -fdiagnostics-color=always #-fsanitize=address
ASM = ./asm/
TESTING = ./testing/
CC = gcc

OBJECTS =   ${BUILD}check_SDL_version.o \
			${BUILD}M6502.o \
			${BUILD}M6502_stack.o \
			${BUILD}M6502_memory.o \
			${BUILD}load_binary.o \
			${BUILD}M6502_instructions.o \

LIBRARY_DIR =	 -L C:\Programs\SDL\SDL2_image-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2_mixer-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2-2.26.5-mingw\x86_64-w64-mingw32\lib

LINKED_LIBRARIES = -lSDL2  -lSDL2main -lSDL2_mixer -lSDL2_image

TEST_OBJS = ${BUILD}M6502.o \
			${BUILD}M6502_stack.o \

LOAD_BIN_OBJS = ${BUILD}M6502_memory.o \


all: ${OBJECTS} # Include objects program relies on
	${CC} ${FLAGS} ${INCLUDES} ${SOURCES}main.c ${OBJECTS} ${LIBRARY_DIR} ${LINKED_LIBRARIES} -o ${BINARIES}main

# Build all - wildcards
${BUILD}%.o:${SOURCES}%.c
	${CC} ${FLAGS} ${INCLUDES} -c $< -o $@

# Clean build folder if needed
clean:
	del "${BUILD}"

run:
	${BINARIES}main ${ASM}test_mos.bin
#${BINARIES}main ${ASM}M6502_decimal_test.bin

# Test section
test: ${TEST_OBJS}
	${CC} ${FLAGS} ${INCLUDES} ${TEST_OBJS} ${TESTING}main_test.c -o ${BINARIES}main_test


# ASM section
asm_build:
	@echo "Running asm"
	nasm -f win64 ${ASM}test.asm -o ${ASM}test.obj
	${CC} ${ASM}test.obj -o ${ASM}test.bin

as65:
	as65 -z ${ASM}test_mos2.asm
#as65 ${ASM}M6502_decimal_test.a65


make_bin:
#mingw32-make ${BUILD}M6502_memory.o
	${CC} ${INCLUDES} ${SOURCES}load_binary.c -o ${BINARIES}load_bin
	${BINARIES}load_bin ${ASM}test_mos.bin
#${BINARIES}load_bin ${ASM}M6502_decimal_test.bin

# Binary file test section
${BUILD}M6502_memory.o:${SOURCES}M6502_memory.c
	${CC} ${FLAGS} ${INCLUDES} -c $< -o $@  