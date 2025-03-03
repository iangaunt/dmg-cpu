#include "opcodes.h"

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

void opcodes::EI() { c->running = false; }

void opcodes::HALT() { c->running = false; }

void opcodes::INC_r(unsigned char* reg) {
    unsigned char sum = *reg + 1;
    c->set_f(sum == 0x0, false, ((*reg & 0xF) + (sum & 0xF) > 0xF), *reg > sum);
    *reg = sum;
    NOP();
}

void opcodes::JP_a16(bool flag, unsigned short a16) {
    if (flag) { 
        c->prog_counter = a16; 
    } else { 
        c->prog_counter++; 
    }
    NOP();
}
void opcodes::JR_s8(bool f, unsigned char s8) { c->prog_counter += (f ? s8 : 1); }

void opcodes::LD_rr(unsigned char* r1, unsigned char* r2) { *r1 = *r2; NOP(); }
void opcodes::LD_rp(unsigned char* reg, unsigned short pair) { *reg = c->mram[pair]; NOP(); }
void opcodes::LD_pr(unsigned short pair, unsigned char* reg) { c->mram[pair] = *reg; NOP(); }
void opcodes::LD_pd16(unsigned char* r1, unsigned char* r2, short d16) {
    *r1 = (unsigned char) (d16 & 0xff00 >> 8);
    *r2 = (unsigned char) (d16 & 0x00ff);
    NOP();
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
        new_pc &= c->stack[c->stack_pointer];
        new_pc <<= 8;
        c->stack_pointer++;
        
        new_pc &= c->stack[c->stack_pointer];
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
        case 0x01: { STOP(); break; }
        case 0x02: { JR_s8(
            !c->f_flags.f_zero, 
            c->mram[c->prog_counter + 1]
        ); break; }
        case 0x03: { JR_s8(
            !c->f_flags.f_carry, 
            c->mram[c->prog_counter + 1]
        ); break; }
        case 0x04: { LD_rr(&c->registers->b, &c->registers->b); break; }
        case 0x05: { LD_rr(&c->registers->d, &c->registers->b); break; }
        case 0x06: { LD_rr(&c->registers->h, &c->registers->d); break; }
        case 0x07: { LD_pr(c->get_hl(), &c->registers->d); break; }
        case 0x08: { ADD_rr(&c->registers->a, &c->registers->b); break; }
        case 0x09: { SUB_r(&c->registers->b); break; }
        case 0x0A: { AND_r(&c->registers->b); break; }
        case 0x0B: { OR(&c->registers->b); break; }
        case 0x0C: { RET(!c->f_flags.f_zero); break; }
        case 0x0D: { RET(!c->f_flags.f_carry); break; }
        case 0x0E: { LD_a8a(c->mram[c->prog_counter + 1]); break; }
        case 0x0F: { LD_aa8(c->mram[c->prog_counter + 1]); break; }

        case 0x10: {
            short d16 = c->mram[c->prog_counter + 1] << 8;
            d16 &= c->mram[c->prog_counter + 2];
            LD_pd16(&c->registers->b, &c->registers->c, d16);
            break;
        };
        case 0x11: {
            short d16 = c->mram[c->prog_counter + 1] << 8;
            d16 &= c->mram[c->prog_counter + 2];
            LD_pd16(&c->registers->d, &c->registers->e, d16);
            break;
        };
        case 0x12: {
            short d16 = c->mram[c->prog_counter + 1] << 8;
            d16 &= c->mram[c->prog_counter + 2];
            LD_pd16(&c->registers->h, &c->registers->l, d16);
            break;
        };
        case 0x13: {
            short d16 = c->mram[c->prog_counter + 1] << 8;
            d16 &= c->mram[c->prog_counter + 2];
            c->stack_pointer = d16;
            break;
        }
        case 0x14: { LD_rr(&c->registers->b, &c->registers->c); break; }
        case 0x15: { LD_rr(&c->registers->d, &c->registers->c); break; }
        case 0x16: { LD_rr(&c->registers->h, &c->registers->c); break; }
    }
}