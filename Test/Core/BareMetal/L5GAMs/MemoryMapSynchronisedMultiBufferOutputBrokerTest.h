/**
 * @file MemoryMapSynchronisedMultiBufferOutputBrokerTest.h
 * @brief Header file for class MemoryMapSynchronisedMultiBufferOutputBrokerTest
 * @date 26/04/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class MemoryMapSynchronisedMultiBufferOutputBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPSYNCHRONISEDMULTIBUFFEROUTPUTBROKERTEST_H_
#define MEMORYMAPSYNCHRONISEDMULTIBUFFEROUTPUTBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the MemoryMapSynchronisedMultiBufferOutputBroker public methods.
 */
class MemoryMapSynchronisedMultiBufferOutputBrokerTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    MemoryMapSynchronisedMultiBufferOutputBrokerTest();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapSynchronisedMultiBufferOutputBrokerTest();

    /**
     * @brief Tests the Constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPSYNCHRONISEDMULTIBUFFEROUTPUTBROKERTEST_H_ */

