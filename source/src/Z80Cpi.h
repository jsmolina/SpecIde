#pragma once

/** Z80Cpi.h
 *
 * Instruction: CPI
 *
 * Encoding: 11 101 101  10 100 001
 * M Cycles: 4 (OCF, OCF, MRB, CPU(5))
 * T States: 16
 *
 * Flags:
 * - Let's subtract from A the value in (HL).
 * - S, Z, H are set by this subtraction.
 * - Now, let's call 'n' the value A - (HL) - H.
 * - 3 is bit 3 of n.
 * - 5 is bit 1 of n.
 * - C is not changed.
 * - N is always set.
 * - P/V is set if BC != 0000h
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Cpi : public Z80Instruction
{
    public:
        Z80Cpi() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->cpuProcCycles = 1;
                    r->memAddrMode = 0x00000002;
                    return true;

                case 1:
                    return true;

                case 2:
                    --r->bc.w;
                    ++r->hl.w;
                    return false;

                case 3:
                    r->tmp.l = r->iReg.h;
                    r->acc.l = r->af.h - r->tmp.l;
                    r->af.l &= FLAG_C;                              // 0000000C
                    r->af.l |= FLAG_N;                              // 000000NC
                    r->af.l |= 
                        (r->af.h ^ r->tmp.l ^ r->acc.l) & FLAG_H;   // 000H00NC
                    r->af.l |= r->acc.l & FLAG_S;                   // S00H00NC
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ0H00NC
                    return false;

                case 4:
                    r->acc.l -= (r->af.l & FLAG_H) >> 4;
                    r->af.l |= (r->acc.l & FLAG_3);                 // SZ0H30NC
                    r->af.l |= (r->acc.l << 4) & FLAG_5;            // SZ5H30NC
                    return false;

                case 5:
                    r->af.l |= (r->bc.w) ? FLAG_PV : 0x00;          // SZ5H3PNC
                    return false;

                case 6:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
