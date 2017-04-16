#pragma once

/** Z80IncRegP.h
 *
 * Instruction: INC rr
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80IncRegP : public Z80Instruction
{
    public:
        Z80IncRegP() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    return false;

                case 1:
                    // Increment operand.
                    ++*r->regp[r->p];
                    return false;

                case 2:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4