/**
 * @file SemaphoreTest.cpp
 * @brief Source file for class SemaphoreTest
 * @date 02/07/2015
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

 * @details This source file contains the definition of all the methods for
 * the class SemaphoreTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

#include "MutexSem.h"
#include "Threads.h"
#include "Sleep.h"
#include "HighResolutionTimer.h"
#include "Seconds.h"
#include "ErrorManagement.h"
#include "CompositeErrorManagement.h"
#include "SemaphoreTest.h"
#include "Vector.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


bool SemaphoreTest::TestConstructor() {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;

    ret.initialisationError = (testSem.Mode() != Semaphore::Closed);

    if (ret){
        ret.initialisationError = (testSem.Status() != 0);
    }

    return ret;
}

bool SemaphoreTest::TestDestructor() {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;
    ret = testSem.Open(Semaphore::Latching);

    if (ret){
        ret.initialisationError = (testSem.Mode() != Semaphore::Latching);
    }

    if (ret){
        testSem.~Semaphore();
        ret.initialisationError = (testSem.Mode() != Semaphore::Closed);
    }
    return ret;
}

bool SemaphoreTest::TestCreate() {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;
    ret = testSem.Open(Semaphore::Latching);
    if (ret) {
    	ret.internalSetupError = (testSem.Mode() != Semaphore::Latching);
    }
    if (ret) {
    	ret.internalSetupError = (testSem.Status() != 0);
    }
    if (ret) {
    	ret.internalSetupError = (testSem.Waiters() != 0);
    }
    testSem.Close();
    return ret;
}

bool SemaphoreTest::TestClose() {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;
    testSem.Open(Semaphore::Latching);
    ret = testSem.Close();
    return ret;
}

bool SemaphoreTest::TestStatus() {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;
    ret = testSem.Open(Semaphore::Latching);
    if (ret){
    	ret.internalSetupError = (testSem.Status() != 0);
    }
    if (ret){
    	ret = testSem.Set();
    }
    if (ret){
    	ret.internalSetupError = (testSem.Status() != 1);
    }
    ret = testSem.Close();
    return ret;
}

bool SemaphoreTest::TestTake_Latching(MilliSeconds timeoutTime) {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;
    ret = testSem.Open(Semaphore::Latching);
    if (ret){
    	ret = testSem.Reset();
    }
    if (ret){
    	ErrorManagement::ErrorType ret2 = testSem.Take(timeoutTime);
    	ret.fatalError = (ret2.timeout == false);
    }
    testSem.Close();
    return ret;
}

bool SemaphoreTest::TestSet_Latching() {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;
    ret = testSem.Open(Semaphore::Latching);
    if (ret){
    	ret = testSem.Reset();
    }
    if (ret){
    	ret = testSem.Set();
    }
    if (ret){
    	ret.fatalError = (testSem.Status() != 1);
    }

    testSem.Close();
    return ret;
}

bool SemaphoreTest::TestReset_Latching() {
	ErrorManagement::ErrorType ret;
    Semaphore testSem;
    ret = testSem.Open(Semaphore::Latching);
    if (ret){
    	ret = testSem.Reset();
    }
    if (ret){
    	ret = testSem.Set();
    }
    if (ret){
    	ret = testSem.Reset();
    }
    if (ret){
    	ret.fatalError = (testSem.Status() != 0);
    }

    testSem.Close();
    return ret;
}

class LocalSharedData{
public:
	volatile int32 sharedVariable;
	Semaphore sem;
	Semaphore sem2;
	Semaphore sem3;

	MilliSeconds timeout;
private:
	void operator=(const LocalSharedData &lsd){}
};

void PosterThreadCallback(LocalSharedData *SemaphoreTest) {
    uint32 maxTimeMS = 2000;
    Ticks startT = HighResolutionTimer::GetTicks();
    while (SemaphoreTest->sharedVariable < 2) {
        Sleep::Short(100,Units::ms);
        SemaphoreTest->sharedVariable = 1;
        SemaphoreTest->sem.Set();
        MilliSeconds deltaMS = HighResolutionTimer::GetTicks() - startT;

        if (deltaMS.GetTime() > maxTimeMS) {
        	REPORT_ERROR(ErrorManagement::Timeout," thread execution timeout");
            break;
        }
    }
    Threads::EndThread();
}

bool SemaphoreTest::TestTake_Latching_Thread() {
	ErrorManagement::ErrorType ret;
	LocalSharedData shared;
	shared.sharedVariable = 0;
	ThreadIdentifier tid = InvalidThreadIdentifier;

    ret = shared.sem.Open(Semaphore::Latching);
    if (ret){
    	ret = shared.sem.Reset();
    }
    if (ret){
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) PosterThreadCallback, &shared);
        ret.fatalError = (tid == InvalidThreadIdentifier);
    	REPORT_ERROR(ret,"failed launching thread");
    }
    if (ret){
    	ret = shared.sem.Take();
    	ret.fatalError = (shared.sharedVariable == 0);
    	REPORT_ERROR(ret,"Take failed waiting ");
    }
    if (tid != InvalidThreadIdentifier){
    	shared.sharedVariable = 2;
    	int counter=0;
        while (Threads::IsAlive(tid)) {
            Sleep::Short(100,Units::ms);;
            if (counter++ > 10) {
            	REPORT_ERROR(ErrorManagement::Timeout," thread exiting timeout: Killing");
                Threads::Kill(tid);
                break;
            }
        }
    }
    shared.sem.Close();
    return ret;
}

void ThreadLocked(LocalSharedData *tt) {
	ErrorManagement::ErrorType ret;

	Atomic::Increment(&tt->sharedVariable);
    ret = tt->sem.Take(tt->timeout);
    if (tt->timeout.GetTimeRaw() > 30){
    	REPORT_ERROR(ret," ThreadLocked exiting with timeout");
    }
	Atomic::Decrement(&tt->sharedVariable);
    Threads::EndThread();
}

bool SemaphoreTest::TestTake_Latching_Threads(uint32 nOfThreads,MilliSeconds timeout) {
	ErrorManagement::ErrorType ret;
	LocalSharedData shared;
	shared.sharedVariable = 0;

	Vector<ThreadIdentifier> tid(nOfThreads);
	shared.timeout = timeout;

	for (uint32 i = 0U;i < nOfThreads;i++){
		tid[i] = InvalidThreadIdentifier;
	}

    ret = shared.sem.Open(Semaphore::Latching);
    if (ret){
    	ret = shared.sem.Reset();
    }
    if (ret){
    	for (uint32 i = 0U;i < nOfThreads;i++){
    		tid[i] = Threads::BeginThread((ThreadFunctionType) ThreadLocked, &shared);
    	}
    	Sleep::Short(133,Units::ms);

    	ret.fatalError = (shared.sharedVariable != static_cast<int32>(nOfThreads));
    	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have reached the value ",nOfThreads," instead it's value is ",shared.sharedVariable );
    }

    if (ret){
    	ret = shared.sem.Set();
    	Sleep::Short(100,Units::ms);
    	ret.fatalError = (shared.sharedVariable != 0);
    	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have returned to 0 instead it's value is ",shared.sharedVariable );
    }

    Sleep::Short(100,Units::ms);;
	for (uint32 i = 0;i < nOfThreads;i++){
		if (tid[i] != InvalidThreadIdentifier){
	        if (Threads::IsAlive(tid[i])) {
                Threads::Kill(tid[i]);
	        }
		}
	}

    shared.sem.Close();
    return ret;
}

bool SemaphoreTest::TestTake_AutoResetting_Threads(uint32 nOfThreads,MilliSeconds timeout) {
	ErrorManagement::ErrorType ret;
	LocalSharedData shared;
	shared.sharedVariable = 0;

	Vector<ThreadIdentifier> tid(nOfThreads);
	shared.timeout = timeout;

	for (uint32 i = 0U;i < nOfThreads;i++){
		tid[i] = InvalidThreadIdentifier;
	}

    ret = shared.sem.Open(Semaphore::AutoResetting);
    if (ret){
    	ret = shared.sem.Reset();
    }
    if (ret){
    	for (uint32 i = 0U;i < nOfThreads;i++){
    		tid[i] = Threads::BeginThread((ThreadFunctionType) ThreadLocked, &shared);
    	}
    	Sleep::Short(166,Units::ms);

    	ret.fatalError = (shared.sharedVariable != static_cast<int32>(nOfThreads));
    	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have reached the value ",nOfThreads," instead it's value is ",shared.sharedVariable );
    }

//printf("\nbegin\n");
    if (ret){
		int32 oldSharedVariable = shared.sharedVariable;
    	while ((shared.sharedVariable > 0) && ret){
//printf("%i step\n",oldSharedVariable);
        	ret = shared.sem.Set();
        	Sleep::Short(25,Units::ms);
    		oldSharedVariable--;
        	int32 newSharedVariable = shared.sharedVariable;
        	ret.fatalError = (newSharedVariable != oldSharedVariable);
        	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have returned to ",oldSharedVariable," instead it's value is ",newSharedVariable );
    	}
    }
//printf("\nend\n");

	for (uint32 i = 0;i < nOfThreads;i++){
		if (tid[i] != InvalidThreadIdentifier){
	        if (Threads::IsAlive(tid[i])) {
                Threads::Kill(tid[i]);
	        }
		}
	}

    shared.sem.Close();
    return ret;
}


bool SemaphoreTest::TestTake_Counting_Threads(uint32 nOfThreads,MilliSeconds timeout) {
	ErrorManagement::ErrorType ret;
	LocalSharedData shared;
	shared.sharedVariable = 0;
	shared.timeout = timeout;

	Vector<ThreadIdentifier> tid(nOfThreads);

	for (uint32 i = 0U;i < nOfThreads;i++){
		tid[i] = InvalidThreadIdentifier;
	}

    ret = shared.sem.Open(Semaphore::Counting);
    REPORT_ERROR(ret,"sem.Open(Counting)");
    if (ret){
    	ret = shared.sem.Reset();
        REPORT_ERROR(ret,"sem.Reset()");
    }
    if (ret){
    	for (uint32 i = 0U;i < nOfThreads;i++){
    		tid[i] = Threads::BeginThread((ThreadFunctionType) ThreadLocked, &shared);
    	}
    	Sleep::Short(300,Units::ms);

    	ret.fatalError = (shared.sharedVariable != static_cast<int32>(nOfThreads));
    	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have reached the value ",nOfThreads," instead it's value is ",shared.sharedVariable );
    }

    if (ret){
		int32 oldSharedVariable = shared.sharedVariable;
    	while ((shared.sharedVariable > 0) && ret){
        	ret = shared.sem.Set(3);
            REPORT_ERROR(ret,"sem.Set()");

        	Sleep::Short(50,Units::ms);
    		oldSharedVariable-=3;
    		if (oldSharedVariable < 0) {
    			oldSharedVariable = 0;
    		}
        	int32 newSharedVariable = shared.sharedVariable;
        	ret.fatalError = (newSharedVariable != oldSharedVariable);
        	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have returned to ",oldSharedVariable," instead it's value is ",newSharedVariable );
    	}
    }

//    Sleep::Short(100,Units::ms);;
	for (uint32 i = 0;i < nOfThreads;i++){
		if (tid[i] != InvalidThreadIdentifier){
	        if (Threads::IsAlive(tid[i])) {
                Threads::Kill(tid[i]);
	        }
		}
	}

    shared.sem.Close();
    return ret;
}

void ThreadCombined(LocalSharedData *tt) {
	ErrorManagement::ErrorType ret;
    ret = tt->sem2.Set();

	Atomic::Increment(&tt->sharedVariable);
    ret = tt->sem.Take(tt->timeout);
    if (tt->timeout.GetTimeRaw() > 30){
    	REPORT_ERROR(ret," ThreadLocked exiting with timeout");
    }
	Atomic::Decrement(&tt->sharedVariable);
    ret = tt->sem3.Set();

	Threads::EndThread();
}


bool SemaphoreTest::TestTake_Combined_Threads(uint32 nOfThreads,MilliSeconds timeout) {
return false;
	ErrorManagement::ErrorType ret;
	LocalSharedData shared;
	shared.sharedVariable = 0;
	shared.timeout = timeout;

	Vector<ThreadIdentifier> tid(nOfThreads);

	for (uint32 i = 0U;i < nOfThreads;i++){
		tid[i] = InvalidThreadIdentifier;
	}

    ret = shared.sem.Open(Semaphore::Counting);
    if (ret){
    	ret = shared.sem.Reset();
    }
    if (ret){
    	ret = shared.sem2.Open(Semaphore::MultiLock);
    }
    if (ret){
    	ret = shared.sem2.Reset(nOfThreads);
    }
    if (ret){
    	ret = shared.sem3.Open(Semaphore::AutoResetting);
    }
    if (ret){
    	ret = shared.sem3.Reset();
    }
    if (ret){
    	for (uint32 i = 0U;i < nOfThreads;i++){
    		tid[i] = Threads::BeginThread((ThreadFunctionType) ThreadCombined, &shared);
    	}
    	ret = shared.sem2.Take(timeout);
//    	Sleep::Short(300,Units::ms);

    	ret.fatalError = (shared.sharedVariable != static_cast<int32>(nOfThreads));
    	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have reached the value ",nOfThreads," instead it's value is ",shared.sharedVariable );
    }

    if (ret){
		int32 oldSharedVariable = shared.sharedVariable;
    	while ((shared.sharedVariable > 0) && ret){
        	ret = shared.sem.Set(3);
        	ret = shared.sem3.Take(timeout);

//        	Sleep::Short(50,Units::ms);
    		oldSharedVariable-=3;
    		if (oldSharedVariable < 0) {
    			oldSharedVariable = 0;
    		}
        	int32 newSharedVariable = shared.sharedVariable;
        	ret.fatalError = (newSharedVariable != oldSharedVariable);
        	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have returned to ",oldSharedVariable," instead it's value is ",newSharedVariable );
    	}
    }

//    Sleep::Short(100,Units::ms);;
	for (uint32 i = 0;i < nOfThreads;i++){
		if (tid[i] != InvalidThreadIdentifier){
	        if (Threads::IsAlive(tid[i])) {
                Threads::Kill(tid[i]);
	        }
		}
	}

    shared.sem.Close();
    return ret;
}


