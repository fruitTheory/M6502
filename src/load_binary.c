#include <stdio.h>
#include <config.h>
#include "load_binary.h"
#include "MOS_6502_memory.h"


uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, int location){
    
    MOS_6502_memory_inbounds(location);
    return memory->address[location];

}

ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, int location){
    uchar8_t byte1 = MOS_6502_memory_get(memory, location);
    uchar8_t byte2 = MOS_6502_memory_get(memory, location+1);
    printf("byte1 = %i byte2 = %i\n", byte1, byte2);

    // A char is 8 bits wide, shifting 8 bits left leaves all 0
    return byte1 << 8 | byte2;
}

int main(int argc, char* argv[]){

    load_binary(argc, argv);

    

    return EXIT_SUCCESS;

}

// load binary file via the command-line arguements
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

    // ftells the current position, since we're at end of file this can represent the size
    uchar8_t file_size = ftell(binary_file);
    
    // creating buffer of files size
    char file_buffer[file_size];

    fseek(binary_file, 0, SEEK_SET);


    printf("file size: %i bytes\n", file_size);

    // read entire file as one chunk
    int read_file = fread(file_buffer, file_size, 1, binary_file);

    if(read_file != 1){
        printf("File was not able to be read\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

