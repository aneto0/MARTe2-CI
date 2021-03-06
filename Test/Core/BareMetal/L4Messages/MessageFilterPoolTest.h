/**
 * @file MessageFilterPoolTest.h
 * @brief Header file for class MessageFilterPoolTest
 * @date 12/09/2016
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

 * @details This header file contains the declaration of the class MessageFilterPoolTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEFILTERPOOLTEST_H_
#define MESSAGEFILTERPOOLTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class MessageFilterPoolTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the ReceiveMessage method.
     */
    bool TestReceiveMessage();

    /**
     * @brief Tests the ReceiveMessage method and verifies that an error in a filter is correctly propagated.
     */
    bool TestReceiveMessage_ErrorInFilter();

    /**
     * @brief Tests the ReceiveMessage method and verifies that an error is triggered in the message is not found (aka matched).
     */
    bool TestReceiveMessage_MessageNotFound();

    /**
     * @brief Tests the ReceiveMessage method and verifies that a not permanent filter is removed.
     */
    bool TestReceiveMessage_NotPermanentRemoved();

    /**
     * @brief Tests the ReceiveMessage method and verifies that a permanent filter is not removed.
     */
    bool TestReceiveMessage_PermanentNotRemoved();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGEFILTERPOOLTEST_H_ */
