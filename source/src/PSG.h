#pragma once

/** PSG
 *
 * AY-3-8912 implementation.
 *
 */

#include <cassert>
#include <cstdint>
// #include <random>
#include <cstdio>

#include "SoundDefs.h"

using namespace std;

class PSG
{
    public:
        uint_fast8_t latch_a;
        uint_fast8_t latch_di;
        uint_fast8_t latch_do;

        uint_fast8_t a;
        uint_fast8_t r[16];

        bool wr;

        int signalA, signalB, signalC;
        int filterA[FILTER_PSG_SIZE];
        int filterB[FILTER_PSG_SIZE];
        int filterC[FILTER_PSG_SIZE];
        int channelA, channelB, channelC;
        int noise, seed;
        int volumeA, volumeB, volumeC;
        int waveA, waveB, waveC;

        int out[16];

        int envSlope, envLevel;
        int envStep;
        bool envHold;
        bool envA, envB, envC;

        uint_fast32_t counterA, counterB, counterC, counterN, counterE;
        uint_fast32_t periodA, periodB, periodC, periodN, periodE;

        bool playSound;

        // random_device rd;
        // mt19937 gen;
        // uniform_int_distribution<> uniform;

        PSG() :
            r{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            wr(false),
            signalA(0), signalB(0), signalC(0),
            channelA(0), channelB(0), channelC(0),
            noise(0), seed(0xFFFF),
            volumeA(0), volumeB(0), volumeC(0),
            waveA(1), waveB(1), waveC(1),
            out{0x000, 0x012, 0x049, 0x0A4, 0x123, 0x1C7, 0x28F, 0x37C,
                0x48D, 0x5C2, 0x71C, 0x89A, 0xA3D, 0xC04, 0xDEF, 0xFFF},
            // out{0x000, 0x020, 0x033, 0x04d, 0x078, 0x0ca, 0x133, 0x239,
            //    0x286, 0x45d, 0x606, 0x76e, 0x97b, 0xb8a, 0xdc5, 0xfff},
            // out{0x000, 0x034, 0x04B, 0x06E, 0x0A3, 0x0F2, 0x151, 0x227,
            //    0x289, 0x414, 0x5B2, 0x726, 0x906, 0xB55, 0xD79, 0xFFF},
            envSlope(1), envLevel(0), envStep(0), envHold(false),
            envA(false), envB(false), envC(false),
            counterA(0), counterB(0), counterC(0), counterN(0), counterE(0),
            periodA(0), periodB(0), periodC(0), periodN(0), periodE(1),
            playSound(true) {}
            // gen(rd()), uniform(0, 1) {} 

        void clock()
        {
            static uint_fast32_t count = 0;
            static uint_fast16_t index = 0;

            ++count;

            a = latch_a;
            latch_do = r[a];

            if (wr)
            {
                // Write registers.
                r[a] = latch_di;
                wr = false;

                switch (a)
                {
                    case 000:
                    case 001:
                        // Update tone period for channel A.
                        periodA = (((r[1] & 0x0F) << 8) + r[0]);
                        break;

                    case 002:
                    case 003:
                        // Update tone period for channel B.
                        periodB = (((r[3] & 0x0F) << 8) + r[2]);
                        break;

                    case 004:
                    case 005:
                        // Update tone period for channel C.
                        periodC = (((r[5] & 0x0F) << 8) + r[4]);
                        break;

                    case 006:
                        // Update noise period.
                        periodN = (r[6] & 0x1F);
                        break;

                    case 010:
                        // Update volume for channel A.
                        volumeA = r[8] & 0x0F;
                        envA = ((r[8] & 0x10) == 0x10);
                        break;

                    case 011:
                        // Update volume for channel B.
                        volumeB = r[9] & 0x0F;
                        envB = ((r[9] & 0x10) == 0x10);
                        break;

                    case 012:
                        // Update volume for channel C.
                        volumeC = r[10] & 0x0F;
                        envC = ((r[10] & 0x10) == 0x10);
                        break;

                    case 013:
                    case 014:
                        // Update period for Envelope generator.
                        periodE = ((r[12] * 0x100) + r[11]);
                        break;

                    case 015:
                        // Start values depend on the attack bit.
                        // Attack = 0: Start at 1111, count down.
                        // Attack = 1: Start at 0000, count up.
                        if (r[13] != 0xFF)
                        {
                            envSlope = ((r[13] & 0x04) == 0x00) ? -1 : 1;
                            envLevel = ((r[13] & 0x04) == 0x00) ? 0x0F : 0x00;
                            restartEnvelope();
                        }
                        break;

                    default:
                        break;
                }
            }

            // Because period means a complete wave cycle (high/low)
            if ((count & 0x07) == 0x00)
            {
                if (counterA-- == 0)
                {
                    waveA = 1 - waveA;
                    counterA = periodA;
                }

                if (counterB-- == 0)
                {
                    waveB = 1 - waveB;
                    counterB = periodB;
                }

                if (counterC-- == 0)
                {
                    waveC = 1 - waveC;
                    counterC = periodC;
                }

                if (counterN-- == 0)
                {
                    noise = generateNoise();
                    counterN = 2 * periodN;
                }

                if (counterE-- == 0)
                {
                    counterE = 2 * periodE;

                    if (envHold == false)
                    {
                        if (++envStep >= 0x10)   // We've finished a cycle.
                        {
                            envStep = 0x00;

                            // Continue = 1: Cycle pattern controlled by Hold.
                            if ((r[13] & 0x08) == 0x08)
                            {
                                // Hold & Alternate
                                if ((r[13] & 0x01) == 0x01)
                                    envHold = true;

                                // If Alternate != Hold, change slope. :)
                                if (((r[13] & 0x02) >> 1) != (r[13] & 0x01))
                                    envSlope = -envSlope;
                            }
                            else
                            {
                                // Continue = 0: Just one cycle, return to 0000.
                                //               Hold.
                                envHold = true;
                                envSlope = 1;
                            }
                        }

                        envLevel = (envSlope > 0) ? envStep : (0x0F - envStep);
                    }
                }
            }

            signalA = signalB = signalC = 1;

            if (playSound)
            {
                if ((r[7] & 0x01) == 0) signalA = waveA;
                if ((r[7] & 0x02) == 0) signalB = waveB;
                if ((r[7] & 0x04) == 0) signalC = waveC;
                if ((r[7] & 0x08) == 0) signalA += noise;
                if ((r[7] & 0x10) == 0) signalB += noise;
                if ((r[7] & 0x20) == 0) signalC += noise;
                signalA *= out[envA ? envLevel : volumeA];
                signalB *= out[envB ? envLevel : volumeB];
                signalC *= out[envC ? envLevel : volumeC];
            }

            filterA[index] = signalA;
            filterB[index] = signalB;
            filterC[index] = signalC;
            index = (index + 1) % FILTER_PSG_SIZE;
        }

        void sample()
        {
            channelA = channelB = channelC = 0;
            for (uint_fast16_t i = 0; i < FILTER_PSG_SIZE; ++i)
            {
                channelA += filterA[i];
                channelB += filterB[i];
                channelC += filterC[i];
            }
            channelA /= FILTER_PSG_SIZE;
            channelB /= FILTER_PSG_SIZE;
            channelC /= FILTER_PSG_SIZE;
        }


        uint_fast8_t read() { return latch_do; }

        void write(uint_fast8_t byte)
        {
            static uint_fast8_t wait = 0;

            if (++wait == 5)
            {
                wait = 0;
                latch_di = byte;
                wr = true;
            }
        }

        void addr(uint_fast8_t byte)
        {
            static uint_fast8_t wait = 0;

            if (++wait == 5)
            {
                wait = 0;
                if ((byte & 0x0F) == byte)
                    latch_a = byte;
            }
        }

        void setVolumeLevels(bool sqr)
        {
            if (sqr)    // Square root (quieter)
            {
                int arr[16] = {
                // These are my values.
                    0x000, 0x012, 0x049, 0x0A4, 0x123, 0x1C7, 0x28F, 0x37C,
                    0x48D, 0x5C2, 0x71C, 0x89A, 0xA3D, 0xC04, 0xDEF, 0xFFF};
                // These values are from Lion 17 and V_Soft.
                // 0x000, 0x020, 0x033, 0x04d, 0x078, 0x0ca, 0x133, 0x239,
                // 0x286, 0x45d, 0x606, 0x76e, 0x97b, 0xb8a, 0xdc5, 0xfff};
                // These are from Hacker Kay
                // 0x000, 0x034, 0x04B, 0x06E, 0x0A3, 0x0F2, 0x151, 0x227,
                // 0x289, 0x414, 0x5B2, 0x726, 0x906, 0xB55, 0xD79, 0xFFF};

                for (uint_fast8_t i = 0; i < 16; ++i)
                    out[i] = arr[i];
            }
            else        // Linear (louder)
            {
                for (uint_fast8_t i = 0; i < 16; ++i)
                    out[i] = static_cast<int>(0x222 * i);
            }
        }

        void reset()
        {
            for (uint_fast8_t i = 0; i < 16; ++i)
                r[i] = 0;

            periodA = periodB = periodC = 0;
            periodE = 0;
            periodN = 0;
            volumeA = volumeB = volumeC = 0;
            seed = 0xFFFF;
        }

        void restartEnvelope()
        {
            envStep = 0;
            envHold = false;
            counterE = 2 * periodE;
        }

        int generateNoise()
        {
            // GenNoise (c) Hacker KAY & Sergey Bulba
            seed = (seed * 2 + 1) ^ (((seed >> 16) ^ (seed >> 13)) & 1);
            return ((seed >> 16) & 1);
        }
};

// vim: et:sw=4:ts=4
