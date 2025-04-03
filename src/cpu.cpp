#include <bitset>
#include <fstream>
#include <iostream>

#include <cpu.h>

#include "systems/regs.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::ios_base;

cpu::cpu() { 
    // Initializes a new registers class.
    registers = new regs();
}

bool cpu::load_rom(const char* rom) {
	ifstream in(rom, ios_base::binary | ios_base::ate);

    // Moves the contents in the rom into the internal memory buffer. 
	if (in.is_open()) {
		std::streampos size = in.tellg();
		char* buffer = new char[size];

		in.seekg(0, ios::beg);
		in.read(buffer, size);
		in.close();

        // Move buffer contents into memory.
		for (int i = 0; i < size; ++i) {
			mram[i] = buffer[i];
		}

        // If the memory fits, then return true.
        return true;
	} else {
        // Any errors are reported to the terminal and the process is aborted.
        cout << "Error: problem loading rom at " << rom << endl;
        return false;
    }
}

unsigned int cpu::read() {
    unsigned int opcode = static_cast<unsigned int>(mram[pc]);
    return opcode;
}

void cpu::print() {
    unsigned int opcode = static_cast<unsigned int>(mram[pc]);
    cout << std::hex << opcode << endl;
}

unsigned short cpu::get_af() {
    unsigned short sh_af = registers->a << 8;
    sh_af |= registers->f;
    return sh_af;
}

void cpu::set_af(unsigned short sh_af) {
    registers->a = static_cast<unsigned char>((sh_af & 0xff00) >> 8);
    registers->f = static_cast<unsigned char>(sh_af & 0x00ff);
}

unsigned short cpu::get_bc() {
    unsigned short sh_bc = registers->b << 8;
    sh_bc |= registers->c;
    return sh_bc;
}

void cpu::set_bc(unsigned short sh_bc) {
    registers->b = static_cast<unsigned char>((sh_bc & 0xff00) >> 8);
    registers->c = static_cast<unsigned char>(sh_bc & 0x00ff);
}

unsigned short cpu::get_de() {
    unsigned short sh_de = registers->d << 8;
    sh_de |= registers->e;
    return sh_de;
}

void cpu::set_de(unsigned short sh_de) {
    registers->d = static_cast<unsigned char>((sh_de & 0xff00) >> 8);
    registers->e = static_cast<unsigned char>(sh_de & 0x00ff);
}

unsigned short cpu::get_hl() {
    unsigned short sh_hl = registers->h << 8;
    sh_hl |= registers->l;
    return sh_hl;
}

void cpu::set_hl(unsigned short sh_hl) {
    registers->h = static_cast<unsigned char>((sh_hl & 0xff00) >> 8);
    registers->l = static_cast<unsigned char>(sh_hl & 0x00ff);
}

signed char cpu::get_s8() { return (signed char) (mram[pc + 1]); }
unsigned char cpu::get_d8() { return (unsigned char) (mram[pc + 1]); }
unsigned short cpu::get_d16() {
    unsigned short d16 = 0x0;
    d16 |= mram[pc + 2];
    d16 <<= 8;
    d16 |= mram[pc + 1];
    return d16;
}

void cpu::set_f(bool fz, bool fs, bool fh, bool fcy) {
    f_zero = fz;
    f_subtract = fs;
    f_half_carry = fh;
    f_carry = fcy;

    build_f();
}

void cpu::build_f() {
    unsigned char new_f = 0b11110000;
    if (!f_zero) new_f -= 128;
    if (!f_subtract) new_f -= 64;
    if (!f_half_carry) new_f -= 32;
    if (!f_carry) new_f -= 16;

    registers->f = new_f;
}