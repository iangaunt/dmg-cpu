#ifndef OPCODES_H
#define OPCODES_H

#include "cpu.h"

class opcodes {
    public:
        cpu* c;
        opcodes(cpu* c);
        void parse(unsigned int opcode);

        void ADC_rr(unsigned char* r1, unsigned char* r2);
        void ADC_rp(unsigned char* r1, unsigned short pair);

        void ADD_rr(unsigned char* r1, unsigned char* r2);
        void ADD_rp(unsigned char* r1, unsigned short pair);

        void AND_r(unsigned char* reg);

        void CCF();
        void CPL();

        void EI();

        void HALT();

        void INC_r(unsigned char* reg);
        void INC_p(unsigned char* r1, unsigned char* r2);

        void JP_a16(bool flag);
        void JR_s8(bool flag);

        void LD_rr(unsigned char* r1, unsigned char* r2);
        void LD_rp(unsigned char* reg, unsigned short pair);
        void LD_pr(unsigned short pair, unsigned char* reg);

        void LD_pd16(unsigned char* r1, unsigned char* r2, short d16);
        void LD_rrd16(unsigned char* r1, unsigned char* r2);
        void LD_rd8(unsigned char* reg, char d8);
        void LD_a8a(unsigned char a8);
        void LD_aa8(unsigned char a8);

        void NOP();

        void OR(unsigned char* reg);

        void POP(unsigned char* r1, unsigned char* r2);
        void PUSH(unsigned char* r1, unsigned char* r2);

        void RET(bool flag);

        void RST(unsigned short location);

        void STOP();

        void SBC_r(unsigned char* reg);
        void SUB_r(unsigned char* reg);
        void SUB_p(unsigned short pair);
};

#endif