/**
 * @file ReplyMessageCatcherMessageFilter.cpp
 * @brief Source file for class ReplyMessageCatcherMessageFilter
 * @date 19/08/2016
 * @author Filippo Sartori
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
 * the class ReplyMessageCatcherMessageFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ReplyMessageCatcherMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ReplyMessageCatcherMessageFilter::ReplyMessageCatcherMessageFilter() :
        MessageFilter(false),
        Object() {
    caught = false;
}

void ReplyMessageCatcherMessageFilter::SetMessageToCatch(const ReferenceT<Message> &message) {
    messageToCatch = message;
}

ReplyMessageCatcherMessageFilter::~ReplyMessageCatcherMessageFilter() {
    caught = true;
}

ErrorManagement::ErrorType ReplyMessageCatcherMessageFilter::ConsumeMessage(ReferenceT<Message> &messageToTest) {

    ErrorManagement::ErrorType ret(true);

    if (messageToTest == messageToCatch) {

        //check reply flag
        if (!messageToTest->IsReply()) {
            COMPOSITE_REPORT_ERROR(ErrorManagement::Warning, "The message caught is not a reply ", messageToTest.operator ->()->GetName());
            ret.warning = true;
        }
        HandleReplyMessage(messageToTest);

    }
    else {
        ret.unsupportedFeature = true;
    }

    return ret;

}

ErrorManagement::ErrorType ReplyMessageCatcherMessageFilter::Wait(const Ticks &maxWait,
                                                                  const uint32 pollingTimeUsec) {
    ErrorManagement::ErrorType ret;
    uint64 endT = HighResolutionTimer::Counter() + maxWait.GetTimeRaw();

    MicroSeconds pollingTime(pollingTimeUsec,Units::us);

    while ((ret) && (!caught)) {
        Sleep::NoMore(pollingTime);
        if (maxWait.IsValid()) {
            ret.timeout = (HighResolutionTimer::Counter() > endT);
        }
    }

    return ret;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12] symbol available to classes that specialise this method*/
void ReplyMessageCatcherMessageFilter::HandleReplyMessage(ReferenceT<Message> &replyMessage) {
    caught = true;
}

CLASS_REGISTER(ReplyMessageCatcherMessageFilter, "1.0")

}

