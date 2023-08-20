#include <stdio.h>
#include <config.h>
#include "load_binary.h"
//#include "MOS_6502_memory.h"
//#define EXIT_FAIL_CHAR char fail[6]={"Failed"}


int main(int argc, char* argv[]){

    //char* test = 
    load_binary(argc, argv);
    //printf("%s", &test);

    return EXIT_SUCCESS;

}

// load binary file via the command-line arguements
char* load_binary(int argc, char* argv[]){

    //char* fail = 'F';

    if(argc < 2){
        printf("You must provide a file to load\n");
        //return fail;
    }

    // set filename to first user arguement
    const char* filename = argv[1];
    printf("File selected: %s\n", filename);

    FILE* program_file = fopen(filename, "rb");
    if (!program_file){
        printf("File was not able to be opened\n");
        //return EXIT_FAILURE;
    }
    
    fseek(program_file, 0, SEEK_END);

    // ftells the current position, since we're at end of file this can represent the size
    uchar8_t file_size = ftell(program_file);
    
    // creating buffer of files size
    char file_buffer[file_size];

    fseek(program_file, 0, SEEK_SET);


    printf("file size: %i bytes\n", file_size);

    // read binary file and store contents into a buffer
    size_t read_program = fread(file_buffer, 1, sizeof(file_buffer), program_file);

    // if(read_program != 1){
    //     printf("File was not able to be read\n");
    //     //return EXIT_FAILURE;
    // }
    fclose(program_file);

    //printf("%02X", read_file);
    printf("%s\n", file_buffer);
    for (size_t i = 0; i < read_program; i++) {
        printf("%02X ", file_buffer[i]);
    }

    //return file_buffer;
}

