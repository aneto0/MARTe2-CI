/**
 * @file MultipleEventSem.cpp
 * @brief
 * @date 21 Aug 2019
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/
#include "CompositeErrorManagement.h"
#include "TypeCharacteristics.h"
#include "MultipleEventSem.h"

#include <poll.h>
#include <sys/resource.h>
#include <errno.h>

namespace MARTe{

MultipleEventSem::MultipleEventSem(){
	data.pending = 0;

}

MultipleEventSem::~MultipleEventSem(){
}

ErrorManagement::ErrorType MultipleEventSem::Wait(const MilliSeconds &timeout){
	ErrorManagement::ErrorType ret;

	if (data.pending <= 0){
		int waitTime = -1;
		if (timeout.IsValid()){
			uint32 temp = timeout.GetTimeRaw();
			if (temp <  static_cast<uint32>(TypeCharacteristics<int>::MaxValue())){
				waitTime = temp;
			} else {
				waitTime = static_cast<uint32>(TypeCharacteristics<int>::MaxValue());
			}
		}

		int wret = poll(&data.handles.Access(0),data.handles.GetSize(),waitTime);

	    ret.OSError = (wret < 0);
		REPORT_ERROR(ret,"Synchronyzer::Wait() failed");

		ret.timeout =  (wret == 0) ;

		if (ret){
			data.pending = wret;
		}
	}

	// assuming there are pending events and no error
	if (ret && (data.pending > 0)){
		uint32 max = data.handles.GetSize();
		bool found = false;
		uint32 index = 0;
		for (; (index < max) && !found; index++){
			const struct pollfd &result = data.handles[index];
			found = (result.revents != 0);
		}

		if (found){
			ret.SetNonErrorCode(index);
			data.pending--;
		}
	}

	return ret;
}

ErrorManagement::ErrorType MultipleEventSem::AddEvent(const EventSource &event){
	ErrorManagement::ErrorType ret;

	ret.unsupportedFeature = (data.handles.GetSize() >= RLIMIT_NOFILE );
	COMPOSITE_REPORT_ERROR(ret,"AddEvent supports up to ",RLIMIT_NOFILE," event sources");

	if (ret){
		data.handles.Add(event.pfd);
	}

	return ret;
}

ErrorManagement::ErrorType MultipleEventSem::Reset(){
	ErrorManagement::ErrorType ret;

	data.pending = 0;
	return ret;
}



} //MARTe