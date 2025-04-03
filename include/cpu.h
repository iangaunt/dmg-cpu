#ifndef CPU_H
#define CPU_H

#include "systems/regs.h"

/**
 * The CPU of the chip. Mainly used for storing values within
 * and throughout the internal memory.
 */
class cpu {
    public:
        /**
         * Creates a new cpu with a new set of registers.
         * All registers `A-L` are initially set to `0x0`.
         */
        cpu();

        // The flag for if the CPU is on.
        bool running = true;
        
        // RAM buffer for the program.
        // also, fuck you
        unsigned char mram[1048560];

        // -- GRAPHICS -- //

        // The width of the display (GameBoy screens are 160 pixels wide).
        unsigned int width = 160;

        // The height of the display (GameBoy screens are 144 pixels tall).
        unsigned int height = 144;

        // Scale factor for the window (passed into SDL2 so the game is playable).
        unsigned int size_modifier = 5;

        // -- PROGRAM NAVIGATION -- //
    
        // The program counter.
        unsigned short pc = 0x0000;
        unsigned short pc_copy = 0x0000;

        // The stack, used for storing and popping subroutines.
        unsigned short stack[256];

        // Stack pointer, used for navigating the stack.
        unsigned short sp = 0x00;

        // -- REGISTERS -- // 

        // Class for handling the registers `A-l`.
        regs* registers;

        // -- F REGISTER HANDLING -- //

        // `Z` - Zero flag. Set if the result of the previous operation is 0.
        bool f_zero = false;

        // `S` - Subtract flag. Set if the previous operation was a subtractive operation.
        bool f_subtract = false;

        // `H` - Half-carry flag. Set if the previous operation caused overflow into the upper bits.
        bool f_half_carry = false;

        // `CY` - Carry flag. Set if the previous caused overflow.
        bool f_carry = false;

        // Sets the `F` flags as a group.
        void set_f(bool fz, bool fs, bool fh, bool fcy);

        // Builds the `F` register based on the current values of the flags.
        void build_f();

        /** Returns a combined 16-bit value containing `A - F`. */
        unsigned short get_af();

        /** Returns a combined 16-bit value containing `B - C`. */
        unsigned short get_bc();

        /** Returns a combined 16-bit value containing `D - E`. */
        unsigned short get_de();

        /** Returns a combined 16-bit value containing `H - L`. */
        unsigned short get_hl();

        /** Sets the value of `AF` (registers `A` and `F`) based on the input 16-bit value. */
        void set_af(unsigned short sh_af);

        /** Sets the value of `BC` (registers `B` and `C`) based on the input 16-bit value. */
        void set_bc(unsigned short sh_bc);

        /** Sets the value of `DE` (registers `D` and `E`) based on the input 16-bit value. */
        void set_de(unsigned short sh_de);

        /** Sets the value of `HL` (registers `H` and `L`) based on the input 16-bit value. */
        void set_hl(unsigned short sh_hl);

        // -- IMMEDIATE VALUES -- //

        /** Fetches the immediate signed value `s8` @ `mram[pc + 1]`. */
        signed char get_s8();

        /** Fetches the immediate unsigned value `d8` @ `mram[pc + 1]`. */
        unsigned char get_d8();

        /** Fetches the immediate signed value `d16` @ `mram[pc + 2], mram[pc + 1]`. 
         * Note this value is little endian, so the immediate value `d8` is loaded into the lower bits.
        */
        unsigned short get_d16();

        // -- INSTRUCTIONS -- // 

        /** Fetches the next instruction in memory from `pc`. */
        unsigned int read();

        /** Outputs the next instruction in memory from `pc`. */
        void print();

        /**
         * Loads the contents of the rom located at `rom` in the system 
         * into the `mram` buffer.
         */
        bool load_rom(const char* rom);

};

#endif