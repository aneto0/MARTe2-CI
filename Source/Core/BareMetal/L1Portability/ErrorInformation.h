/**
 * @file ErrorInformation.h
 * @brief Header file for class ErrorInformation
 * @date 24/08/2015
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

 * @details This header file contains the declaration of the class ErrorInformation
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ERRORINFORMATION_H_
#define ERRORINFORMATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ErrorType.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace ErrorManagement {

/**
 @brief Information structure associated to the error.
 */
struct ErrorInformation {

    /**
     * Definition of the header.
     */
    struct {

        /**
         * The error code.
         */
        ErrorType errorType;

        /**
         * The error line number.
         */
        int16 lineNumber;

        /**
         * Specified is the error is triggered within an object.
         */
        bool isObject :1;

    } header;

    /**
     *  High resolution timer ticks.
     */
    uint64 hrtTime;

    /**
     * The error file name.
     * A pointer to a const char * which is persistent
     * so a real constant, not a char * relabeled as const char *
     * scope. It should be global to the application and persistent
     */
    CCString fileName;

    /**
     * The error function name
     * A pointer to a const char * which is persistent
     * so a real constant, not a char * relabeled as const char *
     * scope. It should be global to the application and persistent
     */
    CCString functionName;

    /**
     * thread ID of the threads who generate the error.
     */
//    ThreadIdentifier threadId;
    /**
     * The Address of the object that produced the error.
     * Object may be temporary in memory because the
     * objectPointer will only be printed, not used
     */
    void * objectPointer;

    /**
     * A pointer to a const char * which is persistent
     * so a real constant, not a char * relabeled as const char *
     * scope. It should be global to the application and persistent
     */
    CCString className;

};
}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ERRORINFORMATION_H_ */

