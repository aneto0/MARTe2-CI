/**
 * @file ObjectRegistryDatabaseMessageFilter.cpp
 * @brief Source file for class ObjectRegistryDatabaseMessageFilter
 * @date 19/06/2018
 * @author Andre' Neto
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
 * the class ObjectRegistryDatabaseMessageFilter (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectRegistryDatabaseMessageFilter.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ObjectRegistryDatabaseMessageFilter::ObjectRegistryDatabaseMessageFilter() :
        MessageFilter(true), Object() {

}

ObjectRegistryDatabaseMessageFilter::~ObjectRegistryDatabaseMessageFilter() {

}



ErrorManagement::ErrorType ObjectRegistryDatabaseMessageFilter::ConsumeMessage(ReferenceT<Message> &messageToTest) {
    ErrorManagement::ErrorType ret;
    ret.parametersError = !messageToTest.IsValid();
    REPORT_ERROR(ret, "Invalid messageToTest");

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<StructuredDataI> params;

    //Do not do this rootNode = ReferenceT<ReferenceContainer>(ord) as otherwise the smart pointer implementation would destroy the ObjectRegistryDatabase!
    Object *rootNode = NULL_PTR(Object *);

    CCString function = messageToTest->GetFunction();
    DynamicCString rootNodeStr;

    if (ret) {
        params = messageToTest->Get(0u);
        if (params.IsValid()) {
            if (params->Read("Root", rootNodeStr)) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::Information, "Going to act on ", rootNodeStr);
            }
        }
        if (rootNodeStr.GetSize() == 0u) {
            rootNode = ord;
            rootNodeStr = "root";
        }
        else {
            ReferenceT<Object> foundNode = ord->Find(rootNodeStr);
            ret.parametersError = !foundNode.IsValid();
        	COMPOSITE_REPORT_ERROR(ret, "Could not find the node with name ", rootNodeStr);
            if (ret) {
                rootNode = foundNode.operator ->();
            }
        }
    }
    //lint -e{613} the rootNode cannot be NULL as otherwise err.ErrorsCleared() would be false.
    if (ret) {
        if (function == "purge") {
            if (rootNode == ord) {
                ord->Purge();
            }
            else {
                ReferenceT<ReferenceContainer> rcNode = ord->Find(rootNodeStr);
                if (rcNode.IsValid()) {
                    rcNode->Purge();
                }
                ret.fatalError = !ord->Delete(rootNodeStr);
            }
        }
        else if (function == "load") {
            ReferenceContainer *rootNodePtr = ord;
            if (rootNode == ord) {
                rootNodePtr = ord;
            }
            else {
                ReferenceT<ReferenceContainer> rcNode = ord->Find(rootNodeStr);
                ret.fatalError = !rcNode.IsValid();
                REPORT_ERROR(ret, "The configuration shall be applied to a ReferenceContainer");
                if (ret) {
                    rootNodePtr = rcNode.operator ->();
                }
            }
            if (ret) {
                ret.parametersError = !params.IsValid();
                REPORT_ERROR(ret, "No configuration data found in the message");
            }
            if (ret) {
                ReferenceT<ReferenceContainer> rc(buildNow);
                ret.fatalError = !rc->Initialise(*params.operator ->());
                uint32 i;
                uint32 nElements = rc->Size();
                for (i = 0u; (ret) && (i < nElements); i++) {
                    ret.fatalError = !rootNodePtr->Insert(rc->Get(i));
                }
            }
        }
        else {
            ret.unsupportedFeature = true;
            COMPOSITE_REPORT_ERROR(ret, "Unsupported function ", function);
        }
    }
    return ret;

}

CLASS_REGISTER(ObjectRegistryDatabaseMessageFilter, "1.0")
}

