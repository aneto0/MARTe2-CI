/**
 * @file messageI.cpp
 * @brief Source file for class messageI
 * @date Apr 14, 2016
 * @author fsartori
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
 * the class messageI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MessageI.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ReferenceT<MessageI> MessageI::FindDestination(CCString destination){
    ReferenceT<MessageI> destinationObject_MessageI;
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    if (ord != NULL_PTR(ObjectRegistryDatabase *)){
        Reference destinationObject = ord->Find(destination);
        if (destinationObject.IsValid()){
            destinationObject_MessageI = destinationObject;
        }
    }
    return destinationObject_MessageI;
}


ReturnType MessageI::SendMessage( ReferenceT<Message> &message,Object *sender){
    CCString destination = "";
    ReturnType ret(true);
    if (!message.IsValid()){
        ret.error.notParametersError = false;
        // TODO produce error message
    }

    // compute actual message parameters
    if (ret){
        // is this is a reply (must be a late reply)
        if (message->IsReplyMessage()){

            // if it is a reply then the destination is the original sender
            destination = message->GetSender();

        // not a reply
        } else {

            // if it is a reply then the destination is the original sender
            destination = message->GetDestination();

            // assigns the sender
            if (sender != NULL){
                message->SetSender(sender->GetName());
            } else {
                // no Object ==> no reply possible
                if (message->ReplyExpected()){
                    // TODO produce error message
                    ret.error.notParametersError = false;
                }
            }
        }
    }

    if (ret){
        ReferenceT<MessageI> destinationObject = FindDestination(destination);

        if (destinationObject.IsValid()){
            ret = destinationObject->ReceiveMessage(message);
        } else {
            ret.error.notUnsupportedFeature = false;
            // TODO produce error message
        }
    }

    if (ret){
        // if we wanted an immediate reply then we should have one
        if (message->ImmediateReplyExpected() && !message->IsReplyMessage()){
            ret.error.notCommunicationError = false;
            // TODO produce error message
        }
    }

    return ret;
}



ReturnType MessageI::SendMessageAndWaitReply(ReferenceT<Message> &message,Object *sender,TimeoutType maxWait){
    ReturnType ret(true);
    if (!message.IsValid()){
        ret.error.notParametersError = false;
        // TODO produce error message
    }

    if (ret){
        // reply to a reply NOT possible
        if (message->IsReplyMessage()){
            // TODO emit error
            ret.error.notCommunicationError = false;
        }
    }

    if (ret){
        // true means immediate reply
        message->MarkReplyExpected(true);
        message->SetReplyTimeout(maxWait);

        ret = SendMessage(message,sender);
    }
    return ret;
}

/**
 * TODO
 * Finds the target object
 * Calls the ReceiveMessage function of the target
 * Reply is expected but does not Waits for a reply and returns
 * */
ReturnType MessageI::SendMessageAndExpectReplyLater(ReferenceT<Message> &message,Object *sender){
    ReturnType ret(true);
    if (!message.IsValid()){
        ret.error.notParametersError = false;
        // TODO produce error message
    }

    if (ret){
        // reply to a reply NOT possible
        if (message->IsReplyMessage()){
            // TODO emit error
            ret.error.notCommunicationError = false;
        }
    }

    if (ret){
        // false means decoupled reply
        message->MarkReplyExpected(false);

        ret = SendMessage(message,sender);
    }
    return ret;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


ReturnType MessageI::ReceiveMessage(ReferenceT<Message> &message) {
    return SortMessage(message);
}

/**
 * TODO
 * Default message sorting mechanism
 * By default checks if there are usable registered methods
 * Otherwise calls HandleMessage
 * */
ReturnType MessageI::SortMessage(ReferenceT<Message> &message){

    ReturnType ret(true);

    Object *thisAsObject = dynamic_cast<Object *> (this);

    if (!message.IsValid()){
        ret.error.notParametersError = false;
        // TODO produce error message
    }

    //if this is an Object derived class then we can look for a registered method to call
    if (thisAsObject != NULL_PTR(Object *)){
        ret.error.notParametersError = false;
        // TODO produce error message
    }

    if (ret){
        CCString function = message->GetFunction();
        if (message->IsReplyMessage()){
            function = "HandleReply";
        }

        ret = thisAsObject->CallRegisteredMethod(function,*(message.operator->()));

        // automatically mark the message as reply
        if (ret && message->ReplyExpected()){
            message->MarkAsReply();
        }

    }

    // check if errors are only of function mismatch
    if (!ret) {
        ReturnType saveRet = ret;
        // try resetting the "good" errors
        ret.error.notUnsupportedFeature = true;
        ret.error.notParametersError = true;
        if (ret){
            ret = HandleMessage(message);
        } else {
            ret = saveRet;
        }
    }

    // shall we send a reply?
    if (ret && message->LateReplyExpected() && message->IsReplyMessage() ){
        ret = MessageI::SendMessage(message);
    }

    return ret;
}

ReturnType MessageI::HandleMessage(ReferenceT<Message> &message){
    return false;
}


}

	
