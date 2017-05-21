#pragma once

/** Z80SraPtrIxIy.h
 *
 * Instruction: SRA (IX + d)
 * Instruction: SRA (IX + d), r
 * Instruction: SRA (IY + d)
 * Instruction: SRA (IY + d), r
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  00 101 rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  00 101 rrr
 * M Cycles: 6 (OCF, OCF, MRB(5), MRB, MRB(4), MWB)
 * T States: 23
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  -  110
 *  A  111
 *
 * Flags: SZ503P0C
 * - MSB is shifted into MSB again. (Sign extension).
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80SraPtrIxIy : public Z80Instruction
{
    public:
        Z80SraPtrIxIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                // Previous steps are executed by the prefix.
                case 5:
                    r->acc.l = r->iReg.h;
                    r->acc.h = (r->acc.l & 0x80) ? 0x01 : 0x00;
                    r->af.l = r->acc.l & FLAG_C;
                    r->acc.w >>= 1;
                    r->acc.h = r->acc.l;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;
                    r->af.l |= (r->acc.h & 0x01) ? 0x00 : FLAG_PV;
                    return false;

                case 6:
                    if (r->z != 6)
                        *r->reg8[r->z] = r->acc.l;
                    r->oReg.l = r->acc.l;
                    return true;

                case 7:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4