/**
 * @file FastPollingEventSem.cpp
 * @brief Source file for class FastPollingEventSem
 * @date 08/ago/2015
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
 * the class FastPollingEventSem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FastPollingEventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe {
FastPollingEventSem::FastPollingEventSem() {
    internalFlag = 0;
    flag = &internalFlag;
}

FastPollingEventSem::FastPollingEventSem(volatile int32 &externalFlag) {
    flag = &externalFlag;
}

FastPollingEventSem::~FastPollingEventSem() {
}

void FastPollingEventSem::Create(const bool wait) {
    if (wait) {
        *flag = 0;
    }
    else {
        *flag = 1;
    }
}

ErrorManagement::ErrorType FastPollingEventSem::FastWait(const TimeoutType &msecTimeout,
                                                         float64 sleepTime) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    uint64 ticksStop = msecTimeout.HighResolutionTimerTicks();
    ticksStop += HighResolutionTimer::Counter();

    bool noSleep = IsEqual(sleepTime, 0.0);

    while (*flag == 0) {
        if (msecTimeout != TTInfiniteWait) {
            if (HighResolutionTimer::Counter() > ticksStop) {
                err = ErrorManagement::Timeout;
                break;
            }
        }
        if (!noSleep) {
            Sleep::Sec(sleepTime);
        }
    }

    return err;
}

void FastPollingEventSem::FastPost() {
    (void) (Atomic::TestAndSet(flag));
}

void FastPollingEventSem::Reset() {
    *flag = 0;
}

ErrorManagement::ErrorType FastPollingEventSem::FastResetWait(const TimeoutType &msecTimeout, float64 sleepTime) {
    Reset();
    return FastWait(msecTimeout, sleepTime);
}

}
