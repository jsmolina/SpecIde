#pragma once

/** Z80OrByte.h
 *
 * Instruction: OR n
 *
 */

bool z80OrByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            // Calculate the result.
            af.l = orFlags[af.h][iReg.h];
            af.h |= iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
