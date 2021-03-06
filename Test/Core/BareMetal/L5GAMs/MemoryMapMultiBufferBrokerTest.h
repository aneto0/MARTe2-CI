/**
 * @file MemoryMapMultiBufferBrokerTest.h
 * @brief Header file for class MemoryMapMultiBufferBrokerTest
 * @date 04/05/2018
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

 * @details This header file contains the declaration of the class MemoryMapMultiBufferBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPMULTIBUFFERBROKERTEST_H_
#define MEMORYMAPMULTIBUFFERBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapMultiBufferBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the MemoryMapInputBroker public methods.
 */
class MemoryMapMultiBufferBrokerTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    MemoryMapMultiBufferBrokerTest();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapMultiBufferBrokerTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Init method.
     */
    bool TestInit();

    /**
     * @brief Tests the Init method with another configuration.
     */
    bool TestInit1();

    /**
     * @brief Tests the CopyInputs method.
     */
    bool TestCopyInputs();

    /**
     * @brief Tests the CopyInputs method with more N times more samples than buffers.
     */
    bool TestCopyInputs_N_Times_More_Samples_Than_Buffers();

    /**
     * @brief Tests the CopyInputs method with a number of samples which is an exact number of the number of buffer.
     */
    bool TestCopyInputs_Equal_Samples_And_Buffers();

    /**
     * @brief Tests the CopyInputs method with a number of samples which is an exact multiple of the number buffers.
     */
    bool TestCopyInputs_Equal_Samples_And_Buffers_NTimes();

    /**
     * @brief Tests the CopyInputs method with no samples.
     */
    bool TestCopyInputs_NoSamples();

    /**
     * @brief Tests the CopyInputs method with more buffers than samples.
     */
    bool TestCopyInputs_MoreBuffersThanSamples();

    /**
     * @brief Tests the CopyOutputs method.
     */
    bool TestCopyOutputs();

    /**
     * @brief Tests the CopyOutputs method with no samples.
     */
    bool TestCopyOutputs_NoSamples();

    /**
     * @brief Tests the CopyOutputs method with more N times more samples than buffers.
     */
    bool TestCopyOutputs_N_Times_More_Samples_Than_Buffers();

    /**
     * @brief Tests the CopyOutputs method with a number of samples which is an exact number of the number of buffer.
     */
    bool TestCopyOutputs_Equal_Samples_And_Buffers();

    /**
     * @brief Tests the CopyOutputs method with  a number of samples which is an exact multiple of the number buffers.
     */
    bool TestCopyOutputs_Equal_Samples_And_Buffers_NTimes();

    /**
     * @brief Tests the CopyOutputs method with more buffers than samples.
     */
    bool TestCopyOutputs_MoreBuffersThanSamples();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPMULTIBUFFERBROKERTEST_H_ */

