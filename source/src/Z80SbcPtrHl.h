#pragma once

/** Z80SbcPtrHl.h
 *
 * Instruction: SBC A, (HL)
 *
 */

bool z80SbcPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            acc.w = af.h - iReg.h;
            acc.w -= af.l & FLAG_C;

            af.l = acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= FLAG_N;
            af.l |= (acc.l ^ iReg.h ^ af.h) & FLAG_H;
            af.l |= (((acc.l ^ iReg.h ^ af.h) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= acc.h & FLAG_C;                   // S.5H3V0C
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0C
            af.h = acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
