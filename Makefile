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

LIBRARY_DIR =	 -L C:\Programs\SDL\SDL2_image-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2_mixer-2.6.3\x86_64-w64-mingw32\lib \
				 -L C:\Programs\SDL\SDL2-2.26.5-mingw\x86_64-w64-mingw32\lib

LINKED_LIBRARIES = -lSDL2  -lSDL2main -lSDL2_mixer -lSDL2_image

TEST_OBJS = ${BUILD}M6502.o \
			${BUILD}M6502_stack.o \

LOAD_BIN_OBJS = ${BUILD}M6502_memory.o \

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

BASE_NAME = chess_test
# Use ca65 assembler
ca65:
	ca65 -l ${ASM}${BASE_NAME}.lst ${ASM}${BASE_NAME}.asm 
	ld65 -t none ${ASM}${BASE_NAME}.o -o ${ASM}${BASE_NAME}.bin


# # Binary file test section
# make_bin:
# #mingw32-make ${BUILD}M6502_memory.o
# 	${CC} ${INCLUDES} ${SOURCES}load_binary.c -o ${BINARIES}load_bin
# 	${BINARIES}load_bin ${ASM}test_mos.bin
# #${BINARIES}load_bin ${ASM}M6502_decimal_test.bin

# ${BUILD}M6502_memory.o:${SOURCES}M6502_memory.c
# 	${CC} ${FLAGS} ${INCLUDES} -c $< -o $@  
# # Test section
# test: ${TEST_OBJS}
# 	${CC} ${FLAGS} ${INCLUDES} ${TEST_OBJS} ${TESTING}main_test.c -o ${BINARIES}main_test

# # ASM section
# asm_build:
# 	@echo "Running asm"
# 	nasm -f win64 ${ASM}test.asm -o ${ASM}test.obj
# 	${CC} ${ASM}test.obj -o ${ASM}test.bin