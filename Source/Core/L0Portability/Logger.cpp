/**
 * @file Logger.cpp
 * @brief Source file for module Logger
 * @date 25/ago/2015
 * @author pc
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
 * the module Logger (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Logger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Logger {

void NullLogMessageProcessFunction(const LogInformation &errorInfo,
                                   const char8 * const errorDescription) {
}

LogMessageProcessFunctionType logMessageProcessFunction = &NullLogMessageProcessFunction;

/*
 * A structure pairing an error code with its explanation.
 */
static const struct {
    const char8 *name;
    ErrorType error;
} errorNames[] = { { "NoError", NoError }, { "Debug Information", Debug }, { "Information", Information }, { "Warning", Warning }, { "FatalError", FatalError },
        { "RecoverableError", RecoverableError }, { "InitialisationError", InitialisationError }, { "OSError", OSError },
        { "ParametersError", ParametersError }, { "IllegalOperation", IllegalOperation }, { "ErrorSharing", ErrorSharing }, { "ErrorAccessDenied",
                ErrorAccessDenied }, { "Exception", Exception }, { "Timeout", Timeout }, { "CommunicationError", CommunicationError }, { "SyntaxError",
                SyntaxError }, { "UnsupportedError", UnsupportedFeature }, { static_cast<const char8 *>(NULL), SyntaxError }, };

const char8 *ToName(const ErrorType errorCode) {
    uint32 i = 0u;
    const char8* retString = "Unrecognized Error";

    while (errorNames[i].name != NULL) {
        if (errorNames[i].error == errorCode) {
            retString = errorNames[i].name;
            break;
        }
        i++;
    }
    return retString;
}

void ReportLogMessage(const ErrorType code,
                      const char8 * const errorDescription,
                      const char8 * const fileName,
                      const int16 lineNumber,
                      const char8 * const functionName) {
    LogInformation logInfo;
    logInfo.threadId = InvalidThreadIdentifier;
    logInfo.objectPointer = static_cast<void*>(NULL);
    logInfo.className = static_cast<const char8 *>(NULL);
    logInfo.header.errorType = code;
    logInfo.header.lineNumber = lineNumber;
    logInfo.fileName = fileName;
    logInfo.functionName = functionName;
    logInfo.hrtTime = HighResolutionTimer::Counter();
#ifndef INTERRUPT_SUPPORTED
    logInfo.threadId = Threads::Id();
#endif
    logMessageProcessFunction(logInfo, errorDescription);
}

void ReportLogMessageFullContext(const ErrorType code,
                                 const char8 * const errorDescription,
                                 const char8 * const fileName,
                                 const int16 lineNumber,
                                 const char8 * const functionName) {
    LogInformation logInfo;
    logInfo.threadId = InvalidThreadIdentifier;
    logInfo.objectPointer = static_cast<void*>(NULL);
    logInfo.className = static_cast<const char8 *>(NULL);
    logInfo.header.errorType = code;
    logInfo.header.lineNumber = lineNumber;
    logInfo.fileName = fileName;
    logInfo.functionName = functionName;
    logInfo.hrtTime = HighResolutionTimer::Counter();
    logInfo.threadId = Threads::Id();
    logMessageProcessFunction(logInfo, errorDescription);
}

void SetLogMessageProcessFunction(const LogMessageProcessFunctionType userFun) {
    if (userFun != NULL) {
        logMessageProcessFunction = userFun;
    }
}

}
