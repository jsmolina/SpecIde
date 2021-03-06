#pragma once

/** Z80RlReg.h
 *
 * Instruction: RL r
 *
 * Encoding: 11 001 011  00 010 rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 * Flags: SZ503P0C
 * - MSB is copied into CF. CF is copied into LSB.
 *
 */

bool z80RlReg()
{
    acc.w = *reg8[z] << 1;
    acc.w |= af.l & FLAG_C;
    af.l = acc.h & FLAG_C;
    *reg8[z] = acc.h = acc.l;
    acc.h ^= acc.h >> 1;
    acc.h ^= acc.h >> 2;
    acc.h ^= acc.h >> 4;
    af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
    af.l |= (acc.l) ? 0x00 : FLAG_Z;
    af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
