#include "config.h"
#include "M6502.h"
#include "load_binary.h"
#include "M6502_screen.h"
#include <stdio.h>
#include <stdlib.h>

#include "hardware_interrupts.h"
//#include "ppu_general.h"

int main(int argc, char* argv[]){

    // initialized M6502 variable
    struct M6502 computer;
    M6502_init(&computer);
    printf("size of virtual computer is: %lli bytes\n", sizeof(struct M6502));

    // load program into memory - returns array char
    uchar8_t* program = load_program(argc, argv);
    //print_program_info(argc, argv);
    ushort16_t program_size = get_program_size(argv);
    printf("program size: %i bytes\n", program_size);
    cpu_store_program(&computer, program, program_size);
    free(program), program = NULL;

    //draw_screen(&computer, program_size);
    // test_prog(&computer);

    // Run below for nes_demo demo.nes
    for(int i = 0; i < 226; i++)
        execute_instructions(&computer, program_size);
        

    for(int i = 0; i < 64; i++)
        printf("sprite: %02X ", computer.ppu.memory.address[0+i]);

    puts("\n");

    for(int i = 0; i < 32; i++)
        printf("palettes: %02X ", computer.ppu.memory.address[0x3F00+i]);

    puts("\n");
    
    for(int i = 0; i < 6; i++)
        printf("vectors: %02X ", computer.cpu.memory.address[0xFFFA+i]);

    puts("\n");

    non_maskable_interrupt(&computer);
    interrupt_request(&computer);
    external_reset_button(&computer); // system reset if user chooses to

    // FILE *file = fopen("./asm/output.bin", "wb");
    // fwrite(&computer.ppu.memory.address, sizeof(uchar8_t), sizeof(computer.ppu.memory.address), file);
    // fclose(file);

    return EXIT_SUCCESS;
}