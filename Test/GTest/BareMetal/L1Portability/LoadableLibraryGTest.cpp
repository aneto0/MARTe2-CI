/**
 * @file LoadableLibraryGTest.cpp
 * @brief Source file for class LoadableLibraryGTest
 * @date 23/07/2015
 * @author Llorenç Capellà
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
 * the class LoadableLibraryGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LoadableLibraryTest.h"
#include "TestSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(LoadableLibraryGTest,TestConstructor) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestConstructor());
}

TEST(LoadableLibraryGTest,TestOpenCloseLibrary) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestOpenLibrary("libm.so"));
}

TEST(LoadableLibraryGTest,TestCloseLibrary) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestCloseLibrary("libm.so"));
}

TEST(LoadableLibraryGTest,TestLoadFunction) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestLoadFunction());
}

TEST(LoadableLibraryGTest,TestLoadFunctionSquareBracketsOperator) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestLoadFunctionSquareBracketsOperator());
}

TEST(LoadableLibraryGTest,TestGetModule) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestGetModule());
}

TEST(LoadableLibraryGTest,TestSetModule) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestSetModule());
}
