#include "M6502.h"
#include "M6502_screen.h"
#include "M6502_general.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>


ushort16_t global_pc = 0;
uchar8_t global_opcode = 0;

void draw_screen(struct M6502* computer, ushort16_t program_size){

    const char* title = "M6502 Emulator";

    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    SDL_Event event;
    SDL_Window* window;

    // Text constants - font, size, and color
    TTF_Font* font = TTF_OpenFont("./font/DejaVuSans.ttf", 21);
    SDL_Color text_color = {255, 255, 255, 255};

    // Dev setup to display on specific window, ignore later - 3840x2160
    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(1, &display_bounds); // Get bounds of the first or second display

    //window = SDL_CreateWindow(title, display_bounds.x+300, display_bounds.y+300, 256*2, 240*2, 0); // other monitor
    window = SDL_CreateWindow(title, display_bounds.x+1600, display_bounds.y+600, 256*2, 240*2, SDL_WINDOW_ALWAYS_ON_TOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create renderer for specific window

    // Inititializes
    ushort16_t initial_program_counter = program_counter;
    bool running = true;

    // event loop while program running
    while(running){
        while(SDL_PollEvent(&event))
            if(event.type == SDL_QUIT) running = false;
        
        // Convert status to binary
        char8_t* binary_status_register = convert_to_binary(status_register);

        int start_x = 75; // Start x position
        int start_y = 75; // Start y position
        int spacing = 25; // Space between lines
        char cpu_register_info[6][20]; // 6 lines that can have up to 20 characters - 2D string array

        // setting each line or index to a different cpu info
        sprintf(cpu_register_info[0], "PC: $%04X", global_pc);
        sprintf(cpu_register_info[1], "Opcode: 0x%02X", global_opcode);
        sprintf(cpu_register_info[2], "Accumulator: %02X", accumulator);
        sprintf(cpu_register_info[3], "Stack: %02X", memory_address[stack_pointer]);
        sprintf(cpu_register_info[4], "X: $%02X Y: %02X", x_register, y_register);
        sprintf(cpu_register_info[5], "SR: %s", binary_status_register);
        free(binary_status_register), binary_status_register = NULL;

        SDL_SetRenderDrawColor(renderer, 0, 100, 128, 255); // sets color
        SDL_RenderClear(renderer); // clear current target with draw color
        
        // check on this if statement, could be while? remove inits from running loop
        if(program_counter - initial_program_counter < program_size){
            for(int i = 0; i < 6; i++){
                // Write on a surce and convert to a texture - use rect to position each line
                SDL_Surface* text_surface = TTF_RenderText_Solid(font, cpu_register_info[i], text_color);
                SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
                SDL_Rect text_rect = {start_x, start_y + i * spacing, text_surface->w, text_surface->h};

                // copy text to render target - basically stores each line
                SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

                // Cleanup from new creation
                SDL_FreeSurface(text_surface);
                SDL_DestroyTexture(text_texture);
            }

            SDL_RenderPresent(renderer); // presents render
            execute_instructions(computer, program_size);
        }
        SDL_Delay(100); // .11 seconds (n milliseconds)
    }

    // cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer); // destroy renderer and associated textures
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

}
