#pragma once

/** Z80PopIx.h
 *
 * Instruction: POP IX
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80PopIx : public Z80Instruction
{
    public:
        Z80PopIx() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x000000BB;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->ix.w = r->operand.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
