/**
 * @file CRCTest.cpp
 * @brief Source file for class CRCTest
 * @date 13/10/2018
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class CRCTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CRC.h"
#include "CRCTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool CRCTest::TestConstructor() {
    using namespace MARTe;
    CRC<uint16> crc;
    crc.ComputeTable(0x0);
    return true;
}

bool CRCTest::TestComputeTable_0x3D65() {
    using namespace MARTe;
    MARTe::uint16 expectedCRC[] = { 0x4841, 0xFC81, 0x9EC9, 0x90D0, 0x397E, 0xEF87, 0xBC97, 0xE5AA };
    return TestComputeCRC<uint16>(0x3D65, 0, false, expectedCRC);
}

bool CRCTest::TestComputeTable_0x3D65_inverted() {
    using namespace MARTe;
    MARTe::uint16 expectedCRC[] = { 0x8E2D, 0x6826, 0x94EE, 0x8DF4, 0x272D, 0x708D, 0xEE98, 0xF4D8 };
    return TestComputeCRC<uint16>(0x3D65, 0, true, expectedCRC);
}

bool CRCTest::TestComputeTable_0x3D65_initialCRC() {
    using namespace MARTe;
    MARTe::uint16 expectedCRC[] = { 0xAF28, 0x1BE8, 0x79A0, 0x77B9, 0x44B4, 0x924D, 0xC15D, 0x9860 };
    return TestComputeCRC<uint16>(0x3D65, 0xFFEE, false, expectedCRC);
}

bool CRCTest::TestComputeTable_0x3D65_inverted_initialCRC() {
    using namespace MARTe;
    MARTe::uint16 expectedCRC[] = { 0x7068, 0x9663, 0x6AAB, 0x73B1, 0x5C09, 0xBA9, 0x95BC, 0x8FFC };
    return TestComputeCRC<uint16>(0x3D65, 0xAABB, true, expectedCRC);
}


bool CRCTest::TestCompute_uint16_CRC_0() {
    using namespace MARTe;
    uint8 data1[16] = { 0xF, 0x0, 0xF, 0x0, 0xAA, 0x55, 0xAA, 0x55, 0xBB, 0xDD, 0xFF, 0xEE, 0xCC, 0xDD, 0xEE, 0xFF };
    uint8 data2[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    uint8 data3[16] = { 0x1, 0xFF, 0x2, 0xFE, 0x3, 0xFD, 0x4, 0xFC, 0x5, 0xFB, 0x6, 0xFA, 0x7, 0xEF, 0x8, 0xEE };
    uint8 data4[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    bool ok = TestComputeCRC0<uint16>(0x589, 0x0, false, &data1[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0, false, &data2[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0, false, &data3[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0, false, &data4[0]);

    return ok;
}

bool CRCTest::TestCompute_uint16_CRC_0_inverted() {
    using namespace MARTe;
    uint8 data1[16] = { 0xF, 0x0, 0xF, 0x0, 0xAA, 0x55, 0xAA, 0x55, 0xBB, 0xDD, 0xFF, 0xEE, 0xCC, 0xDD, 0xEE, 0xFF };
    uint8 data2[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    uint8 data3[16] = { 0x1, 0xFF, 0x2, 0xFE, 0x3, 0xFD, 0x4, 0xFC, 0x5, 0xFB, 0x6, 0xFA, 0x7, 0xEF, 0x8, 0xEE };
    uint8 data4[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    bool ok = TestComputeCRC0<uint16>(0x589, 0x0, true, &data1[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0, true, &data2[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0, true, &data3[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0, true, &data4[0]);

    return ok;
}

bool CRCTest::TestCompute_uint16_CRC_0_initialCRC() {
    using namespace MARTe;
    uint8 data1[16] = { 0xF, 0x0, 0xF, 0x0, 0xAA, 0x55, 0xAA, 0x55, 0xBB, 0xDD, 0xFF, 0xEE, 0xCC, 0xDD, 0xEE, 0xFF };
    uint8 data2[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    uint8 data3[16] = { 0x1, 0xFF, 0x2, 0xFE, 0x3, 0xFD, 0x4, 0xFC, 0x5, 0xFB, 0x6, 0xFA, 0x7, 0xEF, 0x8, 0xEE };
    uint8 data4[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    bool ok = TestComputeCRC0<uint16>(0x589, 0x0A, false, &data1[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0B, false, &data2[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0C, false, &data3[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0x0D, false, &data4[0]);

    return ok;
}

bool CRCTest::TestCompute_uint16_CRC_0_inverted_initialCRC() {
    using namespace MARTe;
    uint8 data1[16] = { 0xF, 0x0, 0xF, 0x0, 0xAA, 0x55, 0xAA, 0x55, 0xBB, 0xDD, 0xFF, 0xEE, 0xCC, 0xDD, 0xEE, 0xFF };
    uint8 data2[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    uint8 data3[16] = { 0x1, 0xFF, 0x2, 0xFE, 0x3, 0xFD, 0x4, 0xFC, 0x5, 0xFB, 0x6, 0xFA, 0x7, 0xEF, 0x8, 0xEE };
    uint8 data4[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    bool ok = TestComputeCRC0<uint16>(0x589, 0xA0, true, &data1[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0xB0, true, &data2[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0xC0, true, &data3[0]);
    ok &= TestComputeCRC0<uint16>(0x589, 0xD0, true, &data4[0]);

    return ok;
}

bool CRCTest::TestCompute_uint8_CRC_0() {
    using namespace MARTe;
    uint8 data1[16] = { 0xF, 0x0, 0xF, 0x0, 0xAA, 0x55, 0xAA, 0x55, 0xBB, 0xDD, 0xFF, 0xEE, 0xCC, 0xDD, 0xEE, 0xFF };
    uint8 data2[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    uint8 data3[16] = { 0x1, 0xFF, 0x2, 0xFE, 0x3, 0xFD, 0x4, 0xFC, 0x5, 0xFB, 0x6, 0xFA, 0x7, 0xEF, 0x8, 0xEE };
    uint8 data4[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    bool ok = TestComputeCRC0<uint8>(0x7, 0x0, false, &data1[0]);
    ok &= TestComputeCRC0<uint8>(0x7, 0x0, false, &data2[0]);
    ok &= TestComputeCRC0<uint8>(0x7, 0x0, false, &data3[0]);
    ok &= TestComputeCRC0<uint8>(0x7, 0x0, false, &data4[0]);

    return ok;
}

bool CRCTest::TestCompute_CRC8_CDMA2000() {
    using namespace MARTe;
    MARTe::uint8 expectedCRC[] = { 0xA5, 0xD8, 0xA1, 0x1F, 0x11, 0xCE, 0xF, 0x8 };
    return TestComputeCRC<uint8>(0x9B, 0xFF, false, expectedCRC);
}

bool CRCTest::TestCompute_CRC8_CDMA2000_inverted() {
    using namespace MARTe;
    MARTe::uint8 expectedCRC[] = { 0xC0, 0x48, 0x15, 0xF8, 0x14, 0x9D, 0x5D, 0x40 };
    return TestComputeCRC<uint8>(0x9B, 0xFF, true, expectedCRC);
}

bool CRCTest::TestCompute_CRC32_MPEG2() {
    using namespace MARTe;
    MARTe::uint32 expectedCRC[] = { 0x13A9D368, 0xA97AFF4D, 0x8B85CF50, 0xA72D88AE, 0x9AE7F9AB, 0x6D6F4403, 0x45F0C92C, 0x903102BF };
    return TestComputeCRC<uint32>(0x4C11DB7, 0xFFFFFFFF, false, expectedCRC);
}

bool CRCTest::TestCompute_CRC32_MPEG2_inverted() {
    using namespace MARTe;
    MARTe::uint32 expectedCRC[] = { 0xD25539B1, 0x841F9BB8, 0x2578EFF4, 0x0E8CFE02, 0x55FFF6E5, 0x2D151158, 0x96C3DC4F, 0x43F06FE4 };
    return TestComputeCRC<uint32>(0x4C11DB7, 0xFFFFFFFF, true, expectedCRC);
}