#include "M6502.h"
#include "config.h"
#include "load_binary.h"
#include <stdio.h>
#include <stdlib.h>

// get program size - may be a better way
ushort16_t get_program_size(char* argv[]){

    cString filename = argv[1]; // set filename to first user arguement
    FILE* program_file = fopen(filename, "rb");
    if (!program_file)
        puts("File was not able to be opened");
    fseek(program_file, 0, SEEK_END);
    ushort16_t file_size = ftell(program_file); if(file_size > cpu_max_address) puts("file too big");
    fseek(program_file, 0, SEEK_SET);
    return file_size;
}

// print file name and the bytes contained
void print_program_info(int argc, char* argv[]){
    // set filename to first user arguement
    cString filename = argv[1];
    printf("File selected: %s\n", filename);

    uchar8_t* program = load_program(argc, argv);
    // Section for printing each byte in file
    printf("bytes: ");
    ushort16_t program_size = get_program_size(argv);
    for (ushort16_t index = 0; index < program_size; index++) {
        printf("%02X ", program[index]);
    } puts("");
    free(program), program = NULL;
}

// load binary file via the command-line arguements
uchar8_t* load_program(int argc, char* argv[]){

    if(argc < 2)
        puts("You must provide a file to load");

    cString filename = argv[1]; // set filename to first user arguement

    // file open
    FILE* program_file = fopen(filename, "rb");
    if (!program_file)
        puts("File was not able to be opened");

    // ftells the current position, since we're at end of file this can represent the size
    fseek(program_file, 0, SEEK_END);
    ushort16_t file_size = ftell(program_file); if(file_size > cpu_max_address) puts("file too big");
    fseek(program_file, 0, SEEK_SET);

    // allocate memory for the file
    uchar8_t *file_buffer = NULL;
    file_buffer = malloc(file_size);
    if (file_buffer == NULL)
        puts("Memory did not initialize");

    // read binary file and store contents into a buffer - returns total number of elements read
    ushort16_t read_program = fread(file_buffer, sizeof(uchar8_t), file_size, program_file);
    if(read_program != file_size)
        puts("File may not have read bytes correctly");

    fclose(program_file);

    // same as returning &file_buffer[0] - memory address of first element - returns whole array
    return file_buffer;
}


uchar8_t* load_program_temporary(const char8_t* path){

    cString filename = path; // set filename to first user arguement

    // file open
    FILE* program_file = fopen(filename, "rb");
    if (!program_file)
        puts("File was not able to be opened");

    // ftells the current position, since we're at end of file this can represent the size
    fseek(program_file, 0, SEEK_END);
    ushort16_t file_size = ftell(program_file); if(file_size > cpu_max_address) puts("file too big");
    fseek(program_file, 0, SEEK_SET);

    // allocate memory for the file
    uchar8_t *file_buffer = NULL;
    file_buffer = malloc(file_size);
    if (file_buffer == NULL)
        puts("Memory did not initialize");

    // read binary file and store contents into a buffer - returns total number of elements read
    ushort16_t read_program = fread(file_buffer, sizeof(uchar8_t), file_size, program_file);
    if(read_program != file_size)
        puts("File may not have read bytes correctly");

    fclose(program_file);

    // same as returning &file_buffer[0] - memory address of first element - returns whole array
    return file_buffer;
}

void print_program_info_temporary(const char8_t* path){
    // set filename to first user arguement
    cString filename = path;
    printf("File selected: %s\n", filename);

    uchar8_t* program = load_program_temporary(path);
    // Section for printing each byte in file
    printf("bytes: ");
    ushort16_t program_size = get_program_size_temporary(path);
    for (ushort16_t index = 0; index < program_size; index++) {
        printf("%02X ", program[index]);
    } puts("");
    free(program), program = NULL;
}

ushort16_t get_program_size_temporary(const char8_t* path){

    cString filename = path; // set filename to first user arguement
    FILE* program_file = fopen(filename, "rb");
    if (!program_file)
        puts("File was not able to be opened");
    fseek(program_file, 0, SEEK_END);
    ushort16_t file_size = ftell(program_file); if(file_size > cpu_max_address) puts("file too big");
    fseek(program_file, 0, SEEK_SET);
    return file_size;
}