#pragma once

/** Z80AddPtrIy.h
 *
 * Instruction: ADD A, (IY+d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AddPtrIy : public Z80Instruction
{
    public:
        Z80AddPtrIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000071;
                    return true;

                case 1:
                    r->tmp.l = r->iReg.h;
                    return false;

                case 2:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->tmp.w += r->iy.w;
                    return false;

                case 4:
                    return false;

                case 5:
                    r->memRdCycles = 1;
                    return true;

                case 6:
                    r->tmp.l = r->iReg.h;
                    r->acc.w = r->af.h + r->tmp.l;

                    r->af.l = r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l ^ r->tmp.l ^ r->af.h) & FLAG_H;
                    r->af.l |= (((r->acc.l ^ r->tmp.l ^ r->af.h) >> 5) 
                            ^ (r->acc.h << 2)) & FLAG_PV;
                    r->af.l |= r->acc.h & FLAG_C;                   // S.5H3V0C
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0C
                    r->af.h = r->acc.l;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
