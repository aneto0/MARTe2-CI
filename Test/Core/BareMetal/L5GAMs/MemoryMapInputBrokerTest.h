/**
 * @file MemoryMapInputBrokerTest.h
 * @brief Header file for class MemoryMapInputBrokerTest
 * @date 28/07/2016
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

 * @details This header file contains the declaration of the class MemoryMapInputBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPINPUTBROKERTEST_H_
#define MEMORYMAPINPUTBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the MemoryMapInputBroker public methods.
 */
class MemoryMapInputBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method with a signal with ranges.
     */
    bool TestExecute_Ranges();

    /**
     * @brief Tests the Execute method with a signal with samples.
     */
    bool TestExecute_Samples();

    /**
     * @brief Tests the Execute method with a multi-state buffers.
     */
    bool TestExecute_MultiStateBuffer();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPINPUTBROKERTEST_H_ */

