#include "Z80.h"

Z80::Z80() :
    CPU(),
    state(Z80State::ST_RESET)
{
}

Z80::~Z80()
{
}

void Z80::reset()
{
    c &= ~SIGNAL_RESET_;
}

void Z80::clock()
{
    if (!(c & SIGNAL_RESET_))
        state = Z80State::ST_RESET;

    switch (state)
    {
        case Z80State::ST_RESET:
            start();

            state = Z80State::ST_M1_T0_ADDRWR;
            break;

        case Z80State::ST_M1_T0_ADDRWR:
            a = pc.w;
            c = 0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            if (!(c & SIGNAL_WAIT_))
                state = Z80State::ST_M1_T1_DATARD;
            break;

        case Z80State::ST_M1_T1_DATARD:
            a = pc.w;
            c = 0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            state = Z80State::ST_M1_T2_RFSH1;
            break;

        case Z80State::ST_M1_T2_RFSH1:
            c = 0xFFFF & ~(SIGNAL_MREQ_ | SIGNAL_RFSH_);

            state = Z80State::ST_M1_T3_RFSH2;
            break;

        case Z80State::ST_M1_T3_RFSH2:
            c = 0xFFFF & ~(SIGNAL_RFSH_);

        default:
            break;
    }
}

void Z80::start()
{
    // Clear all registers
    pc.w = 0x0000;

    af[0].w = 0x0000; af[1].w = 0x0000;
    bc[0].w = 0x0000; bc[1].w = 0x0000;
    de[0].w = 0x0000; de[1].w = 0x0000;
    hl[0].w = 0x0000; hl[1].w = 0x0000;

    ri.w = 0x0000;
    sp.w = 0x0000;
    ix.w = 0x0000;
    iy.w = 0x0000;

    a = 0xFFFF;
    c = 0xFFFF;
    d = 0xFF;
}

// vim: et:sw=4:ts=4