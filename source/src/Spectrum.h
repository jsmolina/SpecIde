#pragma once

/** Spectrum
 *
 * A Spectrum computer
 *
 */

#include "Memory.h"
#include "ULA.h"
#include "Z80.h"
#include "Z80Defs.h"
#include "Buzzer.h"
#include "PSG.h"

#include <fstream>
#include <string>

using namespace std;

class Spectrum
{
    public:
        Spectrum();

        // Required hardware.
        Z80 z80;
        ULA ula;
        PSG psg;
        Buzzer buzzer;

        // Kempston Joystick.
        uint_fast8_t joystick;
        bool kempston;
        bool spectrum128K;
        bool spectrumPlus2;
        bool spectrumPlus2A;
        bool spectrumPlus3;
        uint_fast8_t idle;
        uint_fast16_t paging;

        uint_fast16_t contendedMask;
        bool contendedPage[4];
        bool romPage[4];

        uint_fast8_t ram[2 << 17];
        uint_fast8_t rom[2 << 16];
        uint_fast8_t* map[4];
        uint_fast8_t* scr;

        // This one is going to be called at 7MHz, and is going to:
        // 1. Clock the ULA. This starts the ULA counters.
        // 2. Access memory for the ULA, if the ULA is not high impedance.
        // 3. Clock the Z80, if the ULA says so.
        // 4. Access the memory for the Z80.
        void clock48();
        void clock128();
        void clockPlus3();
        void reset();

        void loadRoms(size_t model);
        void initMems(size_t model);
        void set128K();
        void setPlus2();
        void setPlus2A();
        void setPlus3();
        void updatePage(size_t reg);
        void setPage(
                size_t page, size_t bank,
                bool isRom, bool isContended)
        {
            size_t addr = bank * (2 << 14);
            map[page] = (isRom) ? &rom[addr] : &ram[addr];
            romPage[page] = isRom;
            contendedPage[page] = isContended;
        }

        void setScreen(size_t page)
        {
            scr = &ram[page * (2 << 14)];
        }
};

// vim: et:sw=4:ts=4
