/**
 * @file PlatformEventSource.h
 * @brief Header file for class AnyType
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

#ifndef PLATFORMEVENTSOURCE_H_
#define PLATFORMEVENTSOURCE_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <poll.h>
#include <unistd.h>
#include "TypeCharacteristics.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * a special handle that can be used in WaiForMultipleObjectEx
 */
class EventSourceData{
public:
	/**
	 *
	 */
	EventSourceData(){
		counter = 0;
		pfd.events = 0;
		pfd.revents = 0;
		pfd.fd = 0;
	}

	~EventSourceData(){
		if (pfd.fd !=-1){
			ErrorManagement::ErrorType ret;
			ret.OSError = (close(pfd.fd) < 0);
			REPORT_ERROR(ret,"EventSourceData::Close() failed");
		}
	}

	/**
	 * a struct to manage poll
	 */
	struct pollfd pfd;

	/// how many handles ?
	int32 counter;
private:
	void operator=(EventSourceData &toCopyFrom){}
	inline EventSourceData(const EventSourceData & data){}

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_ENVIRONMENT_LINUX_PLATFORMEVENTSOURCE_H_ */
