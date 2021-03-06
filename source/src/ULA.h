#pragma once

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>
#include <cstddef>
#include <vector>

#include "Z80Defs.h"

using namespace std;

class ULA
{
    public:
        ULA();

        void clock();
        void reset();
        void paint();

        void generateVideoControlSignals();
        void generateInterrupt();
        void generateVideoDataUla();
        void generateVideoDataGa();
        void tapeEarMic();
        void ioPort();
        void start();

        void setUlaVersion(uint_fast8_t version);

        uint32_t averageColour(uint32_t last, uint32_t curr);

        uint_fast16_t videoStart;
        uint_fast16_t videoEnd;
        uint_fast16_t hBorderStart;
        uint_fast16_t hBlankStart;
        uint_fast16_t hBlankEnd;
        uint_fast16_t hSyncEnd;
        uint_fast16_t maxPixel;

        uint_fast16_t vBorderStart;
        uint_fast16_t vBlankStart;
        uint_fast16_t vBlankEnd;
        uint_fast16_t vSyncStart;
        uint_fast16_t vSyncEnd;
        uint_fast16_t maxScan;

        uint_fast32_t voltage[4];

        static uint_fast32_t voltages[4][4];
        static bool delayTable[16];
        static bool idleTable[16];
        static bool memTable[16];

        static uint32_t colourTable[0x100];
        uint32_t colour[2];
        std::vector<uint32_t> pixelsX1;
        std::vector<uint32_t> pixelsX2;
        uint_fast32_t xSize, ySize;
        uint_fast32_t xPos = 0;
        uint_fast32_t yPos = 0;
        uint_fast32_t yInc = 1;
        uint_fast32_t frame = 0;

        // These values depend on the model
        uint_fast8_t ulaVersion = 1;

        // ULA internals
        uint_fast16_t pixel = 0;
        uint_fast16_t scan = 0;
        uint_fast8_t flash = 0;
        bool z80Clk = false;

        uint_fast8_t ioWait = 0;
        uint_fast16_t z80_c_1 = 0xFFFF;
        uint_fast16_t z80_c_2 = 0xFFFF;

        uint_fast16_t dataAddr, attrAddr;

        uint_fast8_t data;
        uint_fast8_t attr;
        uint_fast8_t dataLatch;
        uint_fast8_t attrLatch;
        uint_fast8_t dataReg;
        uint_fast8_t attrReg;

        uint_fast32_t ear = 0;
        uint_fast32_t chargeDelay = 0;
        uint_fast32_t capacitor = 0;

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        uint_fast8_t io;

        uint_fast16_t z80_a = 0xFFFF;
        uint_fast16_t z80_c = 0xFFFF;
        bool contendedBank = false;
        bool mreq_ = true;
        bool cpuClock = false;
        bool ulaReset = true;
        bool contention = false;

        // Video signals
        uint32_t rgba;
        bool video = true;
        bool border = false;
        bool idle = true;
        bool mem = true;

        // Useful video signals
        bool vSync;
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
        bool interrupt = false;
        uint_fast16_t interruptStart;
        uint_fast16_t interruptEnd;

        // Scanline modes.
        uint_fast32_t scanlines = 0;
};
// vim: et:sw=4:ts=4
