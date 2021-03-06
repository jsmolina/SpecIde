#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

/** TZXFile.h
 *
 * TZX file format implementation.
 *
 * This class loads a TZX file and generates pulses that will be fed to the
 * Spectrum EAR port.
 */

class TZXFile
{
    public:
        TZXFile() :
            magic { 'Z', 'X', 'T', 'a', 'p', 'e', '!', 0x1A },
                  magicIsOk(false),
                  firstLeadTone(false),
                  majorVersion(0), minorVersion(0),
                  pointer(0), loopStart(0), loopCounter(0) {}

        uint8_t magic[8];
        bool magicIsOk;
        bool firstLeadTone;
        uint8_t majorVersion, minorVersion;

        vector<uint8_t> fileData;
        vector<uint8_t> romData;

        size_t pointer;
        size_t loopStart;
        size_t loopCounter;

        void load(string const& fileName);
        void parse(
                vector<size_t> &pulseData,
                set<size_t> &indexData,
                set<size_t> &stopData,
                set<size_t> &stopIf48K);

        size_t dumpArchiveInfo();
        size_t dumpComment();
};

// vim: et:sw=4:ts=4:
