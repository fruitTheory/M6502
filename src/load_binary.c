#include <stdio.h>
#include <config.h>
#include "load_binary.h"
//#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]){

    print_file_info(argc, argv);
    // uchar8_t* file_buffer = load_program(argc, argv);

    ushort16_t address[2]={0x05, 0x06};

    MOS_6502_memory_get_short(address, 0x00);
    
    return EXIT_SUCCESS;

}

uchar8_t MOS_6502_memory_get(ushort16_t* memory, int location){
    
    //MOS_6502_memory_inbounds(location);
    return memory[location];

}

ushort16_t MOS_6502_memory_get_short(ushort16_t* memory, int location){
    uchar8_t byte1 = MOS_6502_memory_get(memory, location);
    uchar8_t byte2 = MOS_6502_memory_get(memory, location+1);
    printf("byte1 = %02X byte2 = %02X\n", byte1, byte2);

    // A char is 8 bits wide, shifting 8 bits left leaves all 0
    return byte1 << 8 | byte2;
}


// prints file name, and bytes contained
void print_file_info(int argc, char* argv[]){
    // set filename to first user arguement
    const char* filename = argv[1];
    printf("File selected: %s\n", filename);

    uchar8_t* file_buffer = load_program(argc, argv);
    printf("bytes: ");
    ushort16_t read_program = 11;
    for (ushort16_t index = 0; index < read_program; index++) {
        printf("%02X ", file_buffer[index]);
    } puts("");
    free(file_buffer);
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
    ushort16_t file_size = ftell(program_file); if(file_size > max_memory) puts("file too big");
    fseek(program_file, 0, SEEK_SET);

    // allocate memory for the file
    uchar8_t *file_buffer = malloc(file_size);
    if (file_buffer == NULL)
        puts("Memory did not initialize");

    // read binary file and store contents into a buffer - returns total number of elements read
    ushort16_t read_program = fread(file_buffer, sizeof(uchar8_t), 11, program_file);
    if(read_program != file_size)
        puts("File may not have read bytes correctly");

    fclose(program_file);

    // returning 
    return &file_buffer[0];
}
