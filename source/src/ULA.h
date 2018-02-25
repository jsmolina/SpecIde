#pragma once

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>
#include <cstddef>

#include "Z80Defs.h"

using namespace std;

class ULA
{
    public:
        ULA();

        void clock();
        void reset();

        void generateVideoControlSignals();
        void generateInterrupt();
        void generateVideoData();
        void tapeEarMic();
        void ioPort();
        void start();

        void setUlaVersion(size_t version);

        size_t hBorderStart;
        size_t hBlankStart;
        size_t hBlankEnd;
        size_t hSyncEnd;
        size_t maxPixel;
        size_t interruptStart;
        size_t interruptEnd;

        size_t vBorderStart;
        size_t vBlankStart;
        size_t vBlankEnd;
        size_t vSyncStart;
        size_t vSyncEnd;
        size_t maxScan;

        int_fast32_t voltage[4];

        static int_fast32_t voltages[3][4];
        static bool delayTable[16];
        static bool idleTable[16];
        static bool hizTable[16];

        static uint32_t colourTable[0x100];
        uint32_t colour0, colour1;

        // These values depend on the model
        size_t ulaVersion = 1;

        // ULA internals
        size_t pixel = 0;
        size_t scan = 0;
        uint_fast8_t flash = 0;
        bool z80Clk = false;

        size_t rdWait = 0;
        uint_fast16_t z80_c_1 = 0xFFFF;
        uint_fast16_t z80_c_2 = 0xFFFF;

        uint_fast16_t dataAddr, attrAddr;

        uint_fast8_t data;
        uint_fast8_t attr;
        uint_fast8_t dataLatch;
        uint_fast8_t attrLatch;

        size_t ear = 0;
        size_t chargeDelay = 0;
        size_t capacitor = 0;

        bool interruptRange = false;

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        bool hiz = true;

        uint_fast16_t z80_a = 0xFFFF;
        uint_fast16_t z80_c = 0xFFFF;
        uint_fast16_t z80_mask = 0xFFFF;
        bool cpuClock = false;
        bool ulaReset = true;

        // Video signals
        uint32_t rgba;
        bool display = true;
        bool idle = true;

        // Useful video signals
        bool hSyncEdge;
        bool vSyncEdge;
        bool blanking;
        bool retrace;

        // Port 0xFE
        uint_fast8_t ioPortIn = 0xFF;
        uint_fast8_t ioPortOut = 0x00;
        uint_fast8_t inMask = 0xBF;
        uint_fast8_t borderAttr = 0x00;
        uint_fast8_t tapeIn = 0x00;

        // Keyboard half rows
        uint_fast8_t keys[8];

        // ULA interrupt
        uint_fast16_t c;
};
// vim: et:sw=4:ts=4
