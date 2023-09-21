#include "M6502.h"
#include "M6502_screen.h"
#include "M6502_general.h"
#include "M6502_flags.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>

#include "ppu_general.h"


ushort16_t global_pc = 0;
uchar8_t global_opcode = 0;

// void draw_screen(struct M6502* computer, ushort16_t program_size){

//     const char* title = "M6502 Emulator";

//     SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
//     TTF_Init();

//     SDL_Event event;
//     SDL_Window* window;

//     // Text constants - font, size, and color
//     TTF_Font* font = TTF_OpenFont("./font/DejaVuSans.ttf", 21);
//     SDL_Color text_color = {255, 255, 255, 255};

//     // Dev setup to display on specific window, ignore later - 3840x2160
//     SDL_Rect display_bounds;
//     SDL_GetDisplayBounds(1, &display_bounds); // Get bounds of the first or second display

//     //window = SDL_CreateWindow(title, display_bounds.x+300, display_bounds.y+300, 256*2, 240*2, 0); // other monitor
//     window = SDL_CreateWindow(title, display_bounds.x+1600, display_bounds.y+600, 256*2, 240*2, SDL_WINDOW_ALWAYS_ON_TOP);
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create renderer for specific window

//     // Inititializing
//     ushort16_t initial_program_counter = program_counter;
//     bool running = true;
//     int start_x = 75; // Start x position
//     int start_y = 75; // Start y position
//     int spacing = 25; // Space between lines
//     char cpu_register_info[6][20]; // 6 lines that can have up to 20 characters - 2D string array
//     SDL_SetRenderDrawColor(renderer, 0, 100, 128, 255); // sets color

//     // event loop while program running
//     while(running){
//         while(SDL_PollEvent(&event))
//             if(event.type == SDL_QUIT) running = false;
        
//         // Convert status to binary
//         char8_t* binary_status_register = convert_to_binary(status_register);

//         // setting each line or index to a different cpu info
//         sprintf(cpu_register_info[0], "PC: $%04X", global_pc);
//         sprintf(cpu_register_info[1], "Opcode: 0x%02X", global_opcode);
//         sprintf(cpu_register_info[2], "Accumulator: %02X", accumulator);
//         sprintf(cpu_register_info[3], "Stack: %02X", CPU_address[stack_pointer]);
//         sprintf(cpu_register_info[4], "X: $%02X Y: %02X", x_register, y_register);
//         sprintf(cpu_register_info[5], "SR: %s", binary_status_register);
//         free(binary_status_register), binary_status_register = NULL;

//         SDL_RenderClear(renderer); // clear current target with draw color
        
//         // check on this if statement, could be while? remove inits from running loop
//         if(program_counter - initial_program_counter < program_size){
//             for(int i = 0; i < 6; i++){
//                 // Write on a surce and convert to a texture - use rect to position each line
//                 SDL_Surface* text_surface = TTF_RenderText_Solid(font, cpu_register_info[i], text_color);
//                 SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
//                 SDL_Rect text_rect = {start_x, start_y + i * spacing, text_surface->w, text_surface->h};

//                 // copy text to render target - basically stores each line
//                 SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

//                 // Cleanup from new creation
//                 SDL_FreeSurface(text_surface);
//                 SDL_DestroyTexture(text_texture);
//             }

//             SDL_RenderPresent(renderer); // presents render
//             execute_instructions(computer, program_size);
//         }
//         SDL_Delay(100); // .10 seconds (n milliseconds)
//     }

//     // cleanup
//     TTF_CloseFont(font);
//     SDL_DestroyRenderer(renderer); // destroy renderer and associated textures
//     SDL_DestroyWindow(window);
//     TTF_Quit();
//     SDL_Quit();

// }

void draw_screen(struct M6502* computer, ushort16_t program_size){

    const char* title = "M6502 Emulator";

    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    SDL_Event event;
    SDL_Window* window;
    SDL_Rect bounds;
    SDL_Rect pixel;
    SDL_GetDisplayBounds(1, &bounds); // Get bounds of the first or second display

    //window = SDL_CreateWindow(title, display_bounds.x+300, display_bounds.y+300, 256*2, 240*2, 0); // other monitor
    window = SDL_CreateWindow(title, bounds.x+1600, bounds.y+600, 256*2, 240*2, SDL_WINDOW_ALWAYS_ON_TOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create renderer for specific window

    // Inititializing
    bool running = true;
    int start_x = 0; // Start x position
    int start_y = 0; // Start y position
    int pixel_size = 8; // pixel sqaure size
    SDL_SetRenderDrawColor(renderer, 0, 100, 128, 255); // sets bg color
    SDL_RenderClear(renderer); // clear current target with bg color
    SDL_Color pixel_color = {200, 120, 40, 255};
    SDL_SetRenderDrawColor(renderer, pixel_color.r, pixel_color.g, pixel_color.b, pixel_color.a); // set pixel color

    // setup pixel start
    pixel.x = start_x;
    pixel.y = start_y;
    pixel.w = pixel_size;
    pixel.h = pixel_size;

    get_pattern();
    // event loop while program running
    while(running){
        while(SDL_PollEvent(&event))
            if(event.type == SDL_QUIT) running = false;

        // note that vblank is 241 - 260 so need to extend y and catch if past 240
        // x- 512/8=64 || y - 520/8=65 480/8=60
        for(int y = 0; y < 65; y++){
            if(y < 60){
                clear_bit(7, &PPU_status); // clear status bit 7 if not in vblank range 
                printf("line %i\n", y);
                //printf("ppu status before: %02X\n", PPU_status);
            }
            if(y >= 60){
                puts("vblanky");
                set_bit(7, &PPU_status); // set status bit 7 if in vblank range 
                //printf("ppu status after: %02X\n", PPU_status);
            }
            for(int x = 0; x < 65; x++){
                SDL_RenderPresent(renderer); // presents render
                // accept 2 byte input somehwere to determine the resulting number
                // run function here return_results()
                /*
                Fake function(){
                    result = address[0x0000] | address[0x0001] = 0000 0001 | 0000 0000
                    result = 0x01 = 0000 0001 (row of 8 pixels)
                    store_result(0x01)
                    get_row_result = for(i<8){check_bit(result, i)}
                    row_result = {0,0,0,0, 0,0,0,1} 
                    store_row_result_to_matrix
                }
                fake_function2(){
                    uchar8_t arr[] = NULL;

                }

                0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, my pattern input (8x8 pixels)
                */
                // if bit = 1 draw rect, fill rect, and set to palette index  
                SDL_RenderDrawRect(renderer, &pixel);
                SDL_RenderFillRect(renderer, &pixel);
                SDL_SetRenderDrawColor(renderer, pixel_color.r+(x*pixel_size), pixel_color.g, pixel_color.b, pixel_color.a);
                pixel.x += pixel_size;
                if(x == 64){
                    pixel.y += pixel_size;
                    pixel.x = 0;
                }
            }
        }
        puts("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

        SDL_Delay(100);
    }

    // cleanup
    SDL_DestroyRenderer(renderer); // destroy renderer and associated textures
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

}