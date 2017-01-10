#pragma once

/** Z80LdPtrDeA.h
 *
 * Instruction: LD (DE), A
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrDeA : public Z80Instruction
{
    public:
        Z80LdPtrDeA() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000004;

                    r->outWord.l = r->af->h;
                    return true;

                case 1:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4