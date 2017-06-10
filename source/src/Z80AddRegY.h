#pragma once

/** Z80AddRegY.h
 *
 * Instruction: ADD A, ry
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AddRegY : public Z80Instruction
{
    public:
        Z80AddRegY() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->tmp.l = *r->regy8[r->z];
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
