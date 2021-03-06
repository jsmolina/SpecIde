#pragma once

/** Z80Register
 *
 * Each of the 16-bit groups of two 8-bit registers in the Z80.
 *
 */

#include <cstdint>

using namespace std;

class Z80Register
{
    public:
        Z80Register();

        void setw(uint_fast16_t value);
        uint_fast16_t getw(void);

        // The ordering of the following variables is important.
        // These will be initialized in the order they are declared here.
        // For the sake of clarity, the constructor relies on this.
        uint_fast16_t reg;

        // Using non-fast pointers for correct bitwise operations.
        uint16_t *pw;
        uint8_t *ph;
        uint8_t *pl;

        uint16_t &w;
        uint8_t &h;
        uint8_t &l;
};

// vim: et:sw=4:ts=4
