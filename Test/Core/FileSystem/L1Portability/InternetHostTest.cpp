/**
 * @file InternetHostTest.cpp
 * @brief Source file for class InternetHostTest
 * @date 28/10/2015
 * @author Giuseppe Ferrò
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
 * the class InternetHostTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InternetHostTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


#include "InternetHostTest.h"
#include "String.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool InternetHostTest::TestDefaultConstructor() {

    InternetHost addr;
    if (addr.GetPort() != 0) {
        return false;
    }

    String ret = addr.GetAddress();

    printf("\n%s\n", ret.Buffer());
    return ret == "0.0.0.0";
}

bool InternetHostTest::TestFullConstructor(uint16 port,
                                              const char8 * address,
                                              const char8 * expected) {

    InternetHost addr(port, address);
    if (addr.GetPort() != port) {
        return false;
    }

    String ret = addr.GetAddress();

    printf("\n%s\n", ret.Buffer());
    return (ret == expected);

}

bool InternetHostTest::TestGetPort(uint16 port) {
    InternetHost addr(port);
    printf("\n%d\n", addr.GetPort());
    return addr.GetPort() == port;
}

bool InternetHostTest::TestGetAddress(const char8 *table[][2]) {

    uint32 i = 0;
    while (table[i][0] != NULL) {
        InternetHost addr(0, table[i][0]);
        String ret = addr.GetAddress();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;
}

bool InternetHostTest::TestGetHostName(const char8 *table[][2]) {

    uint32 i = 0;
    while (table[i][0] != NULL) {
        InternetHost addr(0, table[i][0]);
        String ret = addr.GetHostName();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;
}

bool InternetHostTest::TestGetAddressAsNumber(const InternetHostTestTable *table) {

    uint32 i = 0;
    while (table[i].address != NULL) {
        InternetHost addr(0, table[i].address);
        uint32 ret = addr.GetAddressAsNumber();
        printf("\n%x %d\n", ret, i);
        if (ret != table[i].relatedNumber) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestGetLocalHostName() {

    String ret = InternetHost::GetLocalHostName();

    printf("\n%s\n", ret.Buffer());

    return true;

}

bool InternetHostTest::TestGetLocalAddress() {

    String ret = InternetHost::GetLocalAddress();

    printf("\n%s\n", ret.Buffer());

    return ((ret == "127.0.1.1") || (ret == "127.0.0.1"));
}


bool InternetHostTest::TestGetLocalAddressAsNumber() {
    printf("\n%x\n", InternetHost::GetLocalAddressAsNumber());

    return (InternetHost::GetLocalAddressAsNumber() == 0x0101007f) || (InternetHost::GetLocalAddressAsNumber() == 0x0100007f);

}

bool InternetHostTest::TestSetPort(uint16 port) {
    InternetHost addr;

    addr.SetPort(port);

    return addr.GetPort() == port;
}

bool InternetHostTest::TestSetAddress(const char8 *table[][2]) {
    InternetHost addr;

    uint32 i = 0;
    while (table[i][0] != NULL) {
        addr.SetAddress(table[i][0]);
        String ret = addr.GetAddress();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestSetAddressByHostName(const char8 *table[][2]) {

    InternetHost addr;

    uint32 i = 0;
    while (table[i][0] != NULL) {
        addr.SetAddressByHostName(table[i][1]);
        String ret = addr.GetAddress();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][0]) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestSetAddressByNumber(const InternetHostTestTable *table) {
    InternetHost addr;

    uint32 i = 0;
    while (table[i].address != NULL) {
        addr.SetAddressByNumber(table[i].relatedNumber);
        String ret = addr.GetAddress();
        printf("\n%s %d\n", ret.Buffer(), i);
        if (ret != table[i].address) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestSetLocalAddress() {
    InternetHost addr;
    addr.SetLocalAddress();

    String hostname = addr.GetHostName();
    String dotAddr = addr.GetAddress();
    return ((hostname == "localhost") && ((dotAddr == "127.0.1.1") || (dotAddr == "127.0.0.1")));
}

bool InternetHostTest::TestGetInternetHost(const InternetHostTestTable *table) {

    uint32 i = 0;
    while (table[i].address != NULL) {
        InternetHost addr(table[i].port, table[i].address);
        InternetHostCore *copy = addr.GetInternetHost();
#if OPERATING_SYSTEM==Linux
        if (copy->sin_addr.s_addr != addr.GetAddressAsNumber()) {
            printf("\n%x %x %d\n", copy->sin_addr.s_addr, addr.GetAddressAsNumber(), i);
            return false;
        }
        if (copy->sin_port != htons(addr.GetPort())) {
            printf("\n%x %x %d\n", copy->sin_port, addr.GetPort(), i);
            return false;
        }
#endif

        i++;
    }
    return true;
}

bool InternetHostTest::TestSize() {

    InternetHost addr;
    return addr.Size() == sizeof(InternetHostCore);
}

