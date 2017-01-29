#pragma once

/** Z80CpReg.h
 *
 * Instruction: CP r
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80CpReg : public Z80Instruction
{
    public:
        Z80CpReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    // Flags 5 & 3 are copied from the operand.
                    r->af.l = *r->reg8[r->z] & (FLAG_5 | FLAG_3);   // ..5.3...
                    r->af.l |= FLAG_N;                              // ..5.3.N.

                    // Calculate half-carry. This is done by doing a 4-bit
                    // subtraction. Half-carry will be in bit 4.
                    r->acc.w = (r->af.h & 0x0F) - (*r->reg8[r->z] & 0x0F);
                    r->af.l |= (r->acc.l & FLAG_H);                 // ..5H3.N.  

                    // Calculate carry in the bit 7. Overflow flag is
                    // (carry in bit 7) XOR (carry in bit 8).
                    r->acc.w = (r->af.h & 0x7F) - (*r->reg8[r->z] & 0x7F);
                    r->af.l |= (r->acc.w >> 5) & FLAG_PV;

                    // Calculate the result.
                    r->acc.w = r->af.h - *r->reg8[r->z];
                    r->af.l |= r->acc.l & FLAG_S;                   // S.5H3.N.
                    r->af.l |= r->acc.h & FLAG_C;                   // S.5H3.NC
                    r->af.l ^= (r->acc.w >> 6) & FLAG_PV;           // S.5H3VNC
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3VNC
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
