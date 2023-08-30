#include <stdio.h>
#include <config.h>
#include "load_binary.h"
#include <stdlib.h>


// get program size - may be a better way
ushort16_t get_program_size(char* argv[]){

    const char* filename = argv[1]; // set filename to first user arguement
    FILE* program_file = fopen(filename, "rb");
    if (!program_file)
        puts("File was not able to be opened");
    fseek(program_file, 0, SEEK_END);
    ushort16_t file_size = ftell(program_file); if(file_size > max_address) puts("file too big");
    fseek(program_file, 0, SEEK_SET);
    return file_size;
}

// print file name and the bytes contained
void print_program_info(int argc, char* argv[]){
    // set filename to first user arguement
    const char* filename = argv[1];
    printf("File selected: %s\n", filename);

    uchar8_t* program = load_program(argc, argv);
    printf("bytes: ");
    ushort16_t program_size = get_program_size(argv);
    for (ushort16_t index = 0; index < program_size; index++) {
        printf("%02X ", program[index]);
    } puts("");
    free(program);
}

// load binary file via the command-line arguements
uchar8_t* load_program(int argc, char* argv[]){

    if(argc < 2)
        puts("You must provide a file to load");

    const char* filename = argv[1]; // set filename to first user arguement

    // file open
    FILE* program_file = fopen(filename, "rb");
    if (!program_file)
        puts("File was not able to be opened");

    // ftells the current position, since we're at end of file this can represent the size
    fseek(program_file, 0, SEEK_END);
    ushort16_t file_size = ftell(program_file); if(file_size > max_address) puts("file too big");
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
