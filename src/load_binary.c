#include <stdio.h>
#include <config.h>
#include "load_binary.h"
//#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]){


    uchar8_t* file_buffer = load_binary(argc, argv);
    printf("bytes: ");
    ulong64_t read_program = 11;
    for (ulong64_t index = 0; index < read_program; index++) {
        printf("%02X ", file_buffer[index]);
    }

    return EXIT_SUCCESS;

}

// load binary file via the command-line arguements
uchar8_t* load_binary(int argc, char* argv[]){

    //char* fail = 'F';

    if(argc < 2){
        puts("You must provide a file to load");
        //return fail;
    }
    // set filename to first user arguement
    const char* filename = argv[1];
    printf("File selected: %s\n", filename);

    FILE* program_file = fopen(filename, "rb");
    if (!program_file){
        puts("File was not able to be opened");
        //return EXIT_FAILURE;
    }

    fseek(program_file, 0, SEEK_END);
    // ftells the current position, since we're at end of file this can represent the size
    ulong64_t file_size = ftell(program_file);
    // creating buffer of files size
    fseek(program_file, 0, SEEK_SET);

    //uchar8_t file_buffer[file_size]; printf("file size: %i bytes\n", file_size);

    uchar8_t *file_buffer = malloc(file_size);
    if (file_buffer == NULL) {
        puts("Error Empty");
    }
    printf("file size: %i bytes\n", file_size);

    // read binary file and store contents into a buffer - returns total number of elements read
    ulong64_t read_program = fread(file_buffer, sizeof(uchar8_t), sizeof(file_buffer), program_file);
    if(read_program != file_size)
        puts("File may have encountered an error");

    fclose(program_file);

    //printf("file ascii: %s\n", file_buffer);
    printf("bytes: "); 
    for (ulong64_t index = 0; index < read_program; index++) {
        printf("%02X ", file_buffer[index]);
    }

    return &file_buffer[0];
}

