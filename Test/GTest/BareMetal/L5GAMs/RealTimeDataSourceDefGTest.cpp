/**
 * @file RealTimeDataSourceDefGTest.cpp
 * @brief Source file for class RealTimeDataSourceDefGTest
 * @date 04/03/2016
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
 * the class RealTimeDataSourceDefGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceDefTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataSourceDefGTest,TestConstructor) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestConstructor());
}

TEST(RealTimeDataSourceDefGTest,TestAddConsumer) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAddConsumer());
}

TEST(RealTimeDataSourceDefGTest,TestAddProducer) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestAddProducer());
}

TEST(RealTimeDataSourceDefGTest,TestGetNumberOfConsumers) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetNumberOfConsumers());
}

TEST(RealTimeDataSourceDefGTest,TestGetNumberOfProducers) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestGetNumberOfProducers());
}

TEST(RealTimeDataSourceDefGTest,TestVerifyTrue) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestVerifyTrue());
}

TEST(RealTimeDataSourceDefGTest,TestVerifyFalse_MoreThanOneProducer) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestVerifyFalse_MoreThanOneProducer());
}

TEST(RealTimeDataSourceDefGTest,TestVerifyNoConsumers) {
    RealTimeDataSourceDefTest rtdsdTest;
    ASSERT_TRUE(rtdsdTest.TestVerifyNoConsumers());
}
