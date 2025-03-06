#include "opcodes.h"

#include <iostream>

opcodes::opcodes(cpu* c) {
    this->c = c;
}

void opcodes::ADC_rr(unsigned char* r1, unsigned char* r2) {
    unsigned char sum = *r1 + *r2;
    if (c->f_flags.f_carry) sum++;

    c->set_f(sum == 0x0, false, ((*r1 & 0xF) + (sum & 0xF) > 0xF), *r1 > sum);
    *r1 = sum;
    NOP();
}

void opcodes::ADC_rp(unsigned char* r1, unsigned short pair) {
    unsigned char sum = *r1 + c->mram[pair];
    if (c->f_flags.f_carry) sum++;
    
    c->set_f(sum == 0x0, false, ((*r1 & 0xF) + (sum & 0xF) > 0xF), *r1 > sum);
    *r1 = sum;
    NOP();
}

void opcodes::ADD_rr(unsigned char* r1, unsigned char* r2) {
    unsigned char sum = *r1 + *r2;
    c->set_f(sum == 0x0, false, ((*r1 & 0xF) + (sum & 0xF) > 0xF), *r1 > sum);
    *r1 = sum;
    NOP();
}

void opcodes::ADD_rp(unsigned char* r1, unsigned short pair) {
    unsigned char sum = *r1 + c->mram[pair];
    c->set_f(sum == 0x0, false, ((*r1 & 0xF) + (sum & 0xF) > 0xF), *r1 > sum);
    *r1 = sum;
    NOP();
}

void opcodes::AND_r(unsigned char* reg) {
    unsigned char res = c->registers->a & *reg;
    c->registers->a = res;
    c->set_f(res == 0x0, false, true, false);
    NOP();
}

void opcodes::CCF() { c->set_f(c->f_flags.f_zero, false, false, !c->f_flags.f_carry); NOP(); }
void opcodes::CPL() { 
    c->registers->a = ~(c->registers->a); 
    c->set_f(c->f_flags.f_zero, true, true, c->f_flags.f_carry);
    NOP();
}

void opcodes::DEC_p(unsigned char* r1, unsigned char* r2) {
    unsigned short pair = *r1;
    pair <<= 8;
    pair |= *r2;
    pair--;

    *r1 = (pair >> 8);
    *r2 = (pair &= 0x00FF);
    NOP();
}

void opcodes::EI() { c->running = false; }

void opcodes::HALT() { c->running = false; }

void opcodes::INC_r(unsigned char* reg) {
    unsigned char sum = *reg + 1;
    c->set_f(sum == 0x0, false, ((*reg & 0xF) + (sum & 0xF) > 0xF), *reg > sum);
    *reg = sum;
    NOP();
}
void opcodes::INC_p(unsigned char* r1, unsigned char* r2) {
    unsigned short pair = *r1;
    pair <<= 8;
    pair |= *r2;
    pair++;

    *r1 = (pair >> 8);
    *r2 = (pair &= 0x00FF);
    NOP();
}

void opcodes::JP_a16(bool flag) {
    if (flag) { 
        c->prog_counter = c->get_d16(); 
    } else { 
        c->prog_counter++; 
    }
    NOP();
}
void opcodes::JR_s8(bool flag) { c->prog_counter += (flag ? c->get_s8() : 1); }

void opcodes::LD_rr(unsigned char* r1, unsigned char* r2) { *r1 = *r2; NOP(); }
void opcodes::LD_rp(unsigned char* reg, unsigned short pair) { *reg = c->mram[pair]; NOP(); }
void opcodes::LD_pr(unsigned short pair, unsigned char* reg) { c->mram[pair] = *reg; NOP(); }
void opcodes::LD_rmr(unsigned char* r1, unsigned char* r2) { c->mram[0xFF00 | *r1] = *r2; NOP(); }
void opcodes::LD_rrm(unsigned char* r1, unsigned char* r2) { *r1 = c->mram[0xFF00 | *r2]; NOP(); }

void opcodes::LD_pd16(unsigned char* r1, unsigned char* r2, short d16) {
    *r1 = (unsigned char) (d16 & 0xff00 >> 8);
    *r2 = (unsigned char) (d16 & 0x00ff);
    NOP();
}
void opcodes::LD_rrd16(unsigned char* r1, unsigned char* r2) {
    short d16 = c->mram[c->prog_counter + 1] << 8;
    d16 |= c->mram[c->prog_counter + 2];
    LD_pd16(r1, r2, d16);
}
void opcodes::LD_rd8(unsigned char* reg, char d8) { *reg = d8; NOP(); }
void opcodes::LD_a8a(unsigned char a8) { c->mram[0xff00 & a8] = c->registers->a; NOP(); }
void opcodes::LD_aa8(unsigned char a8) { c->registers->a = c->mram[0xff00 & a8]; NOP(); }

void opcodes::NOP() { c->prog_counter++; }

void opcodes::OR(unsigned char* reg) {
    unsigned char res = c->registers->a | *reg;
    c->registers->a = res;
    c->set_f(res == 0x0, false, false, false);
    NOP();
}

void opcodes::POP(unsigned char* r1, unsigned char* r2) {
    *r2 = c->stack[c->stack_pointer];
    c->stack_pointer++;
    *r1 = c->stack[c->stack_pointer];
    c->stack_pointer++;
    NOP();
}

void opcodes::PUSH(unsigned char* r1, unsigned char* r2) {
    c->stack[c->stack_pointer] = *r1;
    c->stack_pointer++;
    c->stack[c->stack_pointer] = *r2;
    c->stack_pointer++;
    NOP();
}

void opcodes::RET(bool flag) {
    if (flag) {
        unsigned short new_pc = 0x0;
        new_pc |= c->stack[c->stack_pointer];
        new_pc <<= 8;
        c->stack_pointer++;
        
        new_pc |= c->stack[c->stack_pointer];
    }
    NOP();
}

void opcodes::RST(unsigned short location) {
    c->stack_pointer--;
    c->stack[c->stack_pointer] = c->prog_counter >> 8;
    c->stack_pointer--;
    c->stack[c->stack_pointer] = c->prog_counter & 0x00ff;
    
    c->prog_counter = location;
}

void opcodes::STOP() { c->running = false; }

void opcodes::SBC_r(unsigned char* reg) {
    unsigned char diff = c->registers->a - *reg;
    if (c->f_flags.f_carry) diff--;
    c->set_f(diff == 0x0, true, ((c->registers->a & 0xF) < (*reg & 0xF)), c->registers->a < diff);
    c->registers->a = diff;
    NOP();
}
void opcodes::SUB_r(unsigned char* reg) {
    unsigned char diff = c->registers->a - *reg;
    c->set_f(diff == 0x0, true, ((c->registers->a & 0xF) < (*reg & 0xF)), c->registers->a < diff);
    c->registers->a = diff;
    NOP();
}
void opcodes::SUB_p(unsigned short pair) {
    unsigned char diff = c->registers->a - c->mram[pair];
    c->set_f(diff == 0x0, true, ((c->registers->a & 0xF) < (c->mram[pair] & 0xF)), c->registers->a < diff);
    c->registers->a = diff;
    NOP();
}

void opcodes::parse(unsigned int opcode) {
    switch (opcode) {
        case 0x00: { NOP(); break; }
        case 0x10: { STOP(); break; }
        case 0x20: { JR_s8(!c->f_flags.f_zero); break; }
        case 0x30: { JR_s8(!c->f_flags.f_carry); break; }
        case 0x40: { LD_rr(&c->registers->b, &c->registers->b); break; }
        case 0x50: { LD_rr(&c->registers->d, &c->registers->b); break; }
        case 0x60: { LD_rr(&c->registers->h, &c->registers->d); break; }
        case 0x70: { LD_pr(c->get_hl(), &c->registers->d); break; }
        case 0x80: { ADD_rr(&c->registers->a, &c->registers->b); break; }
        case 0x90: { SUB_r(&c->registers->b); break; }
        case 0xA0: { AND_r(&c->registers->b); break; }
        case 0xB0: { OR(&c->registers->b); break; }
        case 0xC0: { RET(!c->f_flags.f_zero); break; }
        case 0xD0: { RET(!c->f_flags.f_carry); break; }
        case 0xE0: { LD_a8a(c->mram[c->prog_counter + 1]); break; }
        case 0xF0: { LD_aa8(c->mram[c->prog_counter + 1]); break; }

        case 0x01: { LD_rrd16(&c->registers->b, &c->registers->c); break; };
        case 0x11: { LD_rrd16(&c->registers->d, &c->registers->e); break; }
        case 0x21: { LD_rrd16(&c->registers->h, &c->registers->l); break; }
        case 0x31: { c->stack_pointer = c->get_d16(); NOP(); break; }
        case 0x41: { LD_rr(&c->registers->b, &c->registers->c); break; }
        case 0x51: { LD_rr(&c->registers->d, &c->registers->c); break; }
        case 0x61: { LD_rr(&c->registers->h, &c->registers->c); break; }
        case 0x71: { LD_pr(c->get_hl(), &c->registers->c); break; }
        case 0x81: { ADD_rr(&c->registers->a, &c->registers->c); break; }
        case 0x91: { SUB_r(&c->registers->c); break; }
        case 0xA1: { AND_r(&c->registers->c); break; }
        case 0xB1: { OR(&c->registers->c); break; }
        case 0xC1: { POP(&c->registers->b, &c->registers->c); break; }
        case 0xD1: { POP(&c->registers->d, &c->registers->e); break; }
        case 0xE1: { POP(&c->registers->h, &c->registers->l); break; }
        case 0xF1: { POP(&c->registers->a, &c->registers->f); break; }

        case 0x02: { LD_pr(c->get_bc(), &c->registers->a); break; }
        case 0x12: { LD_pr(c->get_de(), &c->registers->a); break; }
        case 0x22: { LD_pr(c->get_hl(), &c->registers->a); INC_p(&c->registers->h, &c->registers->l); break; }
        case 0x32: { LD_pr(c->get_hl(), &c->registers->a); DEC_p(&c->registers->h, &c->registers->l); break; }
        case 0x42: { LD_rr(&c->registers->b, &c->registers->d); break; }
        case 0x52: { LD_rr(&c->registers->d, &c->registers->d); break; }
        case 0x62: { LD_rr(&c->registers->h, &c->registers->d); break; }
        case 0x72: { LD_pr(c->get_hl(), &c->registers->d); break; }
        case 0x82: { ADD_rr(&c->registers->a, &c->registers->d); break; }
        case 0x92: { SUB_r(&c->registers->d); break; }
        case 0xA2: { AND_r(&c->registers->d); break; }
        case 0xB2: { OR(&c->registers->d); break; }
        case 0xC2: { JP_a16(!c->f_flags.f_zero); break; }
        case 0xD2: { JP_a16(!c->f_flags.f_carry); break; }
        case 0xE2: { LD_rmr(&c->registers->c, &c->registers->a); break; }
        case 0xF2: { LD_rrm(&c->registers->a, &c->registers->c); break; }

        case 0x03: { INC_p(&c->registers->b, &c->registers->c); break; }
        case 0x13: { INC_p(&c->registers->d, &c->registers->e); break; }
        case 0x23: { INC_p(&c->registers->h, &c->registers->l); break; }
        case 0x33: { c->stack_pointer++; NOP(); break; }
        case 0x43: { LD_rr(&c->registers->b, &c->registers->e); break; }
        case 0x53: { LD_rr(&c->registers->d, &c->registers->e); break; }
        case 0x63: { LD_rr(&c->registers->h, &c->registers->e); break; }
        case 0x73: { LD_pr(c->get_hl(), &c->registers->e); break; }
        case 0x83: { ADD_rr(&c->registers->a, &c->registers->e); break; }
        case 0x93: { SUB_r(&c->registers->e); break; }
        case 0xA3: { AND_r(&c->registers->e); break; }
        case 0xB3: { OR(&c->registers->e); break; }
        case 0xC3: { JP_a16(true); break; }
        case 0xD3: { NOP(); break; }
        case 0xE3: { NOP(); break; }
        case 0xF3: { NOP(); break; }

        default: {
            std::cout << "Unknown opcode " << std::hex << opcode << "." << std::endl;
            break;
        }
    }
}