    #include <stdio.h>
    #include <config.h>
    #include "load_binary.h"
    
    // function to load a program file - meant to run in main function with argc/argv
    int load_binary(int argc, char* argv[]){

        if(argc < 2){
            printf("You must provide a file to load\n");
            //return EXIT_FAILURE;
        }

        // set filename to first user arguement
        const char* filename = argv[1];
        printf("File selected: %s\n", filename);

        FILE* binary_file = fopen(filename, "rb");
        if (!binary_file){
            printf("File was not able to be opened\n");
            return EXIT_FAILURE;
        }
        
        fseek(binary_file, 0, SEEK_END);

        // ftell returns current position, since we're at end of file this can represent the size
        unsigned int file_size = ftell(binary_file);

        fseek(binary_file, 0, SEEK_SET);
        
        // setting the memory needed - temporary storage buffer
        char file_buffer[file_size];

        printf("file size: %i bytes\n", file_size);

        // read as one chunk
        int res = fread(file_buffer, file_size, 1, binary_file);

        if(res != 1){
            printf("File was not able to be read\n");
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }