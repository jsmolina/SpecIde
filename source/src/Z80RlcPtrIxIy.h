#pragma once

/** Z80RlcPtrIxIy.h
 *
 * Instruction: RLC (IX + d)
 * Instruction: RLC (IX + d), r
 * Instruction: RLC (IY + d)
 * Instruction: RLC (IY + d), r
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  00 000 rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  00 000 rrr
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
 * - MSB is copied into LSB and into CF.
 *
 */

bool z80RlcPtrIxIy()
{
    switch (executionStep)
    {
        // Previous steps are executed by the prefix.
        case 5:
            acc.w = iReg.h << 1;
            acc.h = acc.l = acc.l | acc.h;
            acc.h ^= acc.h >> 1;
            acc.h ^= acc.h >> 2;
            acc.h ^= acc.h >> 4;
            af.l = acc.l & (FLAG_S | FLAG_5 | FLAG_3 | FLAG_C);
            af.l |= (acc.l) ? 0x00 : FLAG_Z;
            af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;
            return false;

        case 6:
            if (z != 6)
                *reg8[z] = acc.l;
            oReg.l = acc.l;
            return true;

        case 7:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
