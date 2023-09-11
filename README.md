# fTNES - 6502 Microprocessor - NES

Currently a 6502 microprocessor but PPU is on the way  
Emulation of NES from Mos 6502, written in C

## Features

- Full emulation of the legal CPU opcodes
- Emulation of primary system components I/O
- Support for NROM games, non memory-mapped

### Prerequisites

- A C 64-bit compiler (ex. GCC, Clang, MSVC)
- Simple DirectMedia Layer (SDL2)
- Basic make and lib linking knowledge

### Building

```mingw32-make``` ```make```

### Running

```
./ftnes path_to_rom
```

![Emulator Test](./img/emulator_test.gif)

## Roadmap

- ~~Implement primary opcodes~~
- Timings and Interrupts
- Debug functions
- Build on top, NES components 
- Refine components
- Release version v0.01
- Streamline build

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change

## License

Licensed under the MIT License - [LICENSE.md](LICENSE.md) 

## Acknowledgments

- nesdev.org - obelisk's guide - Andrew Jacobs
- as65 - assembler by Andrew Jacobs
- masswerk.at - 6502 instruction set reference
- ca65 - Assembler by Ullrich von Bassewitz, Greg King
- hex editor - hexed.it ♡
- NES Documentation - Patrick Diskin 2004 
- mcs6500 family programming manual

