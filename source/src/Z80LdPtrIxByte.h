#pragma once

/** Z80LdPtrIxByte.h
 *
 * Instruction: LD (IX+d), n
 *
 * Encoding: 11 011 101  00 110 110
 * M Cycles: 5 (DD, OCF, MRB, MRB/CPU, MWB)
 * T States: 19
 *
 */

bool z80LdPtrIxByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00000611;
            return true;

        case 1:
            wz.l = iReg.h;
            return true;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += ix.w;
            return false;

        case 4:
            memWrCycles = 1;
            oReg.l = iReg.h;
            return true;

        case 5:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
