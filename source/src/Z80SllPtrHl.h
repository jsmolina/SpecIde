#pragma once

/** Z80SllPtrHl.h
 *
 * Instruction: SLL (HL)
 *
 * Encoding: 11 001 011  00 110 110
 * M Cycles: 4 (OCF, OCF, MRB(4), MWB)
 * T States: 15
 *
 * Flags: SZ503P0C
 * - 1 is shifted into the LSB.
 *
 */

bool z80SllPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            acc.w = (iReg.h << 1) | 0x01;
            af.l = acc.h & FLAG_C;
            acc.h = acc.l;
            acc.h ^= acc.h >> 1;
            acc.h ^= acc.h >> 2;
            acc.h ^= acc.h >> 4;
            af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= (acc.l) ? 0x00 : FLAG_Z;
            af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;
            return false;

        case 2:
            oReg.l = acc.l;
            return true;

        case 3:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
