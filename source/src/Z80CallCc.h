#pragma once

/** Z80CallCc.h
 *
 * Instruction: CALL NC, nn
 * Instruction: CALL C, nn
 * Instruction: CALL NZ, nn
 * Instruction: CALL Z, nn
 * Instruction: CALL PO, nn
 * Instruction: CALL PE, nn
 * Instruction: CALL P, nn
 * Instruction: CALL M, nn
 *
 */

bool z80CallCc()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x0000AA11;
            return true;

        case 1:
            return true;

        case 2:
            wz.w = iReg.w;
            switch (y)
            {
                case 0: return ((af.l & FLAG_Z) == FLAG_Z);
                case 1: return ((af.l & FLAG_Z) == 0x00);
                case 2: return ((af.l & FLAG_C) == FLAG_C);
                case 3: return ((af.l & FLAG_C) == 0x00);
                case 4: return ((af.l & FLAG_PV) == FLAG_PV);
                case 5: return ((af.l & FLAG_PV) == 0x00);
                case 6: return ((af.l & FLAG_S) == FLAG_S);
                case 7: return ((af.l & FLAG_S) == 0x00);
                default: assert(false); return true;
            }

        case 3:
            oReg.l = pc.h;
            oReg.h = pc.l;
            memWrCycles = 2;
            return true;

        case 4:
            return true;

        case 5:
            pc.w = wz.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
