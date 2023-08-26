# MOS 6502 Emulator

Emulator for the MOS 6502 microprocessor, written in C

## Features

- Full emulation of the legal CPU opcodes
- Emulation of primary system components
- Support for basic input/output

### Prerequisites

- A C 64-bit compiler (ex. GCC, Clang, MSVC)
- Potentially other dependencies (SDL)

### Building

```mingw32-make``` ```make```

### Running

```
./emulator path_to_file
```

## Roadmap

- Implement primary opcodes
- Timings and Interrupts 
- Refine components
- Build on top, other components 

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

