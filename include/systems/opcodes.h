#ifndef OPCODES_H
#define OPCODES_H

#include "cpu.h"

class opcodes {
    public:
        // The CPU being modified by the following opcodes.
        cpu* c;

        /**
         * Creates a new opcode handler for controlling the CPU `c`.
         */
        opcodes(cpu* c);
        void parse(unsigned int opcode);

        /**
         * `ADC_rr` -> `ADC r1, r2`
         * 
         * Add the contents of register r1 and the CY flag to the contents 
         * of register r2, and store the results in register r1.
         */
        void ADC_rr(unsigned char* r1, unsigned char* r2);
        void ADC_rp(unsigned char* reg, unsigned short pair);
        void ADC_rd8(unsigned char* reg, unsigned char d8);

        void ADD_rr(unsigned char* r1, unsigned char* r2);
        void ADD_rp(unsigned char* reg, unsigned short pair);
        void ADD_rd8(unsigned char* reg, unsigned char d8);
        void ADD_rs8(unsigned char* reg, signed char s8);
        void ADD_pp(unsigned char* r1, unsigned char* r2, unsigned char* r3, unsigned char* r4);

        void AND_r(unsigned char* reg);
        void AND_p(unsigned short pair);
        void AND_d8(unsigned char d8);

        void BIT_r(bool* flag, unsigned char* reg, char index);
        void BIT_p(bool* flag, unsigned short pair, char index);

        void CCF();

        void CP_rr(unsigned char* r1, unsigned char* r2);
        void CP_rp(unsigned char* reg, unsigned short pair);
        void CP_rd8(unsigned char* reg, unsigned char d8);

        void CPL();

        void DEC_r(unsigned char* reg);
        void DEC_p(unsigned char* r1, unsigned char* r2);

        void EI();

        void HALT();

        void INC_r(unsigned char* reg);
        void INC_p(unsigned char* r1, unsigned char* r2);

        void JP_a16(bool flag);
        void JR_s8(bool flag);

        void LD_rr(unsigned char* r1, unsigned char* r2);
        void LD_rp(unsigned char* reg, unsigned short pair);
        void LD_pr(unsigned short pair, unsigned char* reg);
        void LD_rmr(unsigned char* r1, unsigned char* r2);
        void LD_rrm(unsigned char* r1, unsigned char* r2);

        void LD_pd16(unsigned char* r1, unsigned char* r2, unsigned short d16);
        void LD_rrd16(unsigned char* r1, unsigned char* r2);
        void LD_rd8(unsigned char* reg, char d8);
        void LD_pd8(unsigned short pair, char d8);
        void LD_a8a(unsigned char a8);
        void LD_aa8(unsigned char a8);

        void NOP();

        void OR_r(unsigned char* reg);
        void OR_p(unsigned short pair);
        void OR_d8(unsigned char d8);

        void POP(unsigned char* r1, unsigned char* r2);
        void PUSH(unsigned char* r1, unsigned char* r2);

        void RET(bool flag);

        void RST(unsigned short location);

        void STOP();

        void SBC_r(unsigned char* reg);
        void SUB_r(unsigned char* reg);
        void SUB_p(unsigned short pair);
        void SUB_d8(unsigned char d8);

        void XOR_r(unsigned char* reg);
        void XOR_p(unsigned short pair);
        void XOR_d8(unsigned char d8);
};      

#endif