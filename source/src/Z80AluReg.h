#pragma once

/** Z80AluReg.h
 *
 * Instruction: ADD r
 * Instruction: ADC r
 *              
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AluReg : public Z80Instruction
{
    public:
        Z80AluReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    switch (r->y)
                    {
                        case 0:     // ADD
                            r->operand.w =
                                (r->af.h & 0x0F) + (*(r->reg8[r->z]) & 0x0F);
                            break;

                        case 1:     // ADC
                            r->operand.w =
                                (r->af.h & 0x0F) + (*(r->reg8[r->z]) & 0x0F) 
                                + (r->af.l & FLAG_C);
                            break;

                        case 2:     // SUB
                            r->operand.w =
                                (r->af.h & 0x0F) - (*(r->reg8[r->z]) & 0x0F);
                            break;

                        case 3:     // SBC
                            r->operand.w =
                                (r->af.h & 0x0F) - (*(r->reg8[r->z]) & 0x0F)
                                - (r->af.l & FLAG_C);
                            break;

                        default:    // Should not happen
                            assert(false);
                            return true;
                    }

                    r->af.l = (r->operand.l & (FLAG_H | FLAG_3)) 
                        | (r->af.l & FLAG_C);                       // ...H3.0c

                    switch (r->y)
                    {
                        case 0:     // ADD
                            r->operand.w = r->af.h + (*r->reg8[r->z]);
                            break;
                            
                        case 1:     // ADC
                            r->operand.w = r->af.h + (*r->reg8[r->z]) 
                                + (r->af.l & FLAG_C);
                            break;

                        case 2:     // SUB
                            r->operand.w = r->af.h - *(r->reg8[r->z]);
                            break;

                        case 3:     // SBC
                            r->operand.w = r->af.h - (*r->reg8[r->z]) 
                                - (r->af.l & FLAG_C);
                            break;

                        default:    // Should not happen
                            assert(false);
                            return true;
                    }

                    r->af.l &= ~FLAG_C;                             // ...H3.0.
                    r->af.l |= r->operand.l & (FLAG_S | FLAG_5);    // S.5H3.0.
                    r->af.l |= r->operand.h & FLAG_C;               // S.5H3.0C
                    r->af.l |= (((r->af.h ^ r->operand.l)
                                & (*(r->reg8[r->z]) ^ r->operand.l)) >> 5) 
                        & FLAG_PV;                                  // S.5H3P0C
                    r->af.l |= (r->operand.l) ? 0x00 : FLAG_Z;      // SZ5H3V0C
                    r->af.h = r->operand.l;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
