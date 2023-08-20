INCLUDES = -I ./include/
BUILD = ./build/
SOURCES = ./src/
BINARIES = ./bin/
FLAGS = -g -Wall -fdiagnostics-color=always #-fsanitize=address
ASM = ./asm/
TESTING = ./testing/

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

TEST_OBJS = ${BUILD}MOS_6502.o \
			${BUILD}MOS_6502_stack.o \

LOAD_BIN_OBJS = ${BUILD}MOS_6502_memory.o \


all: ${OBJECTS} # Include objects program relies on
	gcc  ${FLAGS} ${INCLUDES} ${SOURCES}main.c ${OBJECTS} ${LIBRARY_DIR} ${LINKED_LIBRARIES} -o ${BINARIES}main

# Build all - wildcards
${BUILD}%.o:${SOURCES}%.c
	gcc ${FLAGS} ${INCLUDES} -c $< -o $@  

# Clean build folder if needed
clean:
	del ${BUILD}

# Test section
test: ${TEST_OBJS}
	gcc  ${FLAGS} ${INCLUDES} ${TEST_OBJS} ${TESTING}main_test.c -o ${BINARIES}main_test


# ASM section
asm_build:
	@echo "Running asm"
	nasm -f win64 ${ASM}test.asm -o ${ASM}test.obj
	gcc ${ASM}test.obj -o ${ASM}test.bin

as65:
	as65 ${ASM}test_mos.asm
#as65 ${ASM}6502_decimal_test.a65


# Binary file test section
${BUILD}MOS_6502_memory.o:${SOURCES}MOS_6502_memory.c
	gcc ${FLAGS} ${INCLUDES} -c $< -o $@  

load_bin:
	./load_bin ${ASM}test_mos.bin

make_bin:
#mingw32-make ${BUILD}MOS_6502_memory.o
	gcc ${INCLUDES} ${SOURCES}load_binary.c -o load_bin

		 