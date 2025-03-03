#include <bitset>
#include <fstream>
#include <iostream>

#include <cpu.h>

#include "structs/regs.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::ios_base;

cpu::cpu() { 
    registers = new regs();
}

bool cpu::load_rom(const char* rom) {
	ifstream in(rom, ios_base::binary | ios_base::ate);

	if (in.is_open()) {
		std::streampos size = in.tellg();
		char* buffer = new char[size];

		in.seekg(0, ios::beg);
		in.read(buffer, size);
		in.close();

		for (int i = 0; i < size; ++i) {
			mram[i] = buffer[i];
		}
        return true;
	} else {
        cout << "Error: problem loading rom at " << rom << endl;
        return false;
    }

    return false;
}

unsigned int cpu::read() {
    unsigned int opcode = static_cast<unsigned int>(mram[prog_counter]);
    return opcode;
}

void cpu::print() {
    unsigned int opcode = static_cast<unsigned int>(mram[prog_counter]);
    cout << std::hex << opcode << endl;
}

short cpu::get_af() {
    unsigned short sh_af = registers->a << 8;
    sh_af |= registers->f;
    return sh_af;
}

void cpu::set_af(unsigned short sh_af) {
    registers->a = static_cast<unsigned char>((sh_af & 0xff00) >> 8);
    registers->f = static_cast<unsigned char>(sh_af & 0x00ff);
}

short cpu::get_bc() {
    unsigned short sh_bc = registers->b << 8;
    sh_bc |= registers->c;
    return sh_bc;
}

void cpu::set_bc(unsigned short sh_bc) {
    registers->b = static_cast<unsigned char>((sh_bc & 0xff00) >> 8);
    registers->c = static_cast<unsigned char>(sh_bc & 0x00ff);
}

short cpu::get_de() {
    unsigned short sh_de = registers->d << 8;
    sh_de |= registers->e;
    return sh_de;
}

void cpu::set_de(unsigned short sh_de) {
    registers->d = static_cast<unsigned char>((sh_de & 0xff00) >> 8);
    registers->e = static_cast<unsigned char>(sh_de & 0x00ff);
}

short cpu::get_hl() {
    unsigned short sh_hl = registers->h << 8;
    sh_hl |= registers->l;
    return sh_hl;
}

void cpu::set_hl(unsigned short sh_hl) {
    registers->h = static_cast<unsigned char>((sh_hl & 0xff00) >> 8);
    registers->l = static_cast<unsigned char>(sh_hl & 0x00ff);
}

void cpu::set_f(bool fz, bool fs, bool fh, bool fcy) {
    f_flags.f_zero = fz;
    f_flags.f_subtract = fs;
    f_flags.f_half_carry = fh;
    f_flags.f_carry = fcy;

    build_f();
}

void cpu::build_f() {
    unsigned char new_f = 0b11110000;
    if (!f_flags.f_zero) new_f -= 128;
    if (!f_flags.f_subtract) new_f -= 64;
    if (!f_flags.f_half_carry) new_f -= 32;
    if (!f_flags.f_carry) new_f -= 16;

    registers->f = new_f;
}