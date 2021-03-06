#pragma once

/** Z80LdRegPtrIx.h
 *
 * Instruction: LD r, (IX+d)
 *
 * Encoding: 11 011 101  01 rrr 110
 * M Cycles: 5 (DD, OCF, MRB, CPU, MRB)
 * T States: 19
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

bool z80LdRegPtrIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000061;
            return true;

        case 1: // Memory read byte cycle
            cpuProcCycles = 1;
            return true;

        case 2:
            wz.l = iReg.h;
            return false;

        case 3:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 4:
            wz.w += ix.w;
            return false;

        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
            *(reg8[y]) = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
