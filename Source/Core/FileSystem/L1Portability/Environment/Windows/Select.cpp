/**
 * @file Select.cpp
 * @brief Source file for class Select
 * @date Dec 2, 2015
 * @author Frank Perez Paz
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
 * the class Select (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "Select.h"
#include "MilliSeconds.h"
#include "CompositeErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

Select::Select() {
#if 0
    //Note that the readHandle will contain the read and the write handles (see WaitUntil which accepts only one array)
    readHandle.registeredHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    readHandle.selectHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    readHandle.selectedHandle = reinterpret_cast<Handle>(-1);

    writeHandle.registeredHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    writeHandle.selectHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    writeHandle.selectedHandle = reinterpret_cast<Handle>(-1);

    exceptionHandle.registeredHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    exceptionHandle.selectHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    exceptionHandle.selectedHandle = reinterpret_cast<Handle>(-1);

    for (uint32 i = 0; i < MAXIMUM_WAIT_OBJECTS; i++) {
        readHandle.registeredHandles[i] = NULL;
        readHandle.selectHandles[i] = NULL;

        writeHandle.registeredHandles[i] = NULL;
        writeHandle.selectHandles[i] = NULL;

        exceptionHandle.registeredHandles[i] = NULL;
        exceptionHandle.selectHandles[i] = NULL;
    }
    highestHandle = 0;
#endif
}

Select::~Select() {
#if 0
    delete[] readHandle.registeredHandles;
    delete[] readHandle.selectHandles;

    delete[] writeHandle.registeredHandles;
    delete[] writeHandle.selectHandles;

    delete[] exceptionHandle.registeredHandles;
    delete[] exceptionHandle.selectHandles;
#endif
}

#if 0
static inline bool AddHandle(const Handle &handle, SetIdentifier &handlesint32 eventMask){

	ErrorManagement::ErrorType ret;

    Handle hSocket = handle.GetReadHandle();
    ret.parametersError = (hSocket < 0);
    REPORT_ERROR(ret, "Select::AddHandle(). Invalid descriptor.");

    if (ret){
        ret.outOfRange = ((highestHandle + 1) > MAXIMUM_WAIT_OBJECTS);
        REPORT_ERROR(ret,"too many handles");
    }

    int32 indexMax = 0L;
    if (ret){
        while (handles.selectHandles[indexMax] != NULL) {
            indexMax++;
        }
        for (int32 i = 0; (i < indexMax) && ret; i++) {
            ret.illegalOperation = (handles.registeredHandles[i] == hSocket);
            REPORT_ERROR(ret, "Select::AddHandle(), Same Handle.");
        }
    }

    WSAEVENT wsaevent = WSA_INVALID_EVENT;
    if (ret){
        wsaevent = WSACreateEvent();
        ret.OSError = (wsaevent == WSA_INVALID_EVENT);
        REPORT_ERROR(ret, "Select::WSACreateEvent failed.");
    }

    if (ret){
        int32 error = WSAEventSelect(reinterpret_cast<SOCKET>(hSocket), wsaevent, eventMask);
        if (error != SOCKET_ERROR) {
        	handles.registeredHandles[indexMax] = wsaevent;
        }
        else {
        	handles.registeredHandles[indexMax] = hSocket;
        }
        handles.selectHandles[indexMax] = hSocket;
        highestHandle++;
    }
	return ret;
}

static inline bool RemoveHandle(const Handle &handle, SetIdentifier &handles,int32 &highestHandle){
	ErrorManagement::ErrorType ret;

    Handle hSocket = handle.GetReadHandle();
    ret.parametersError = (hSocket < 0);
    REPORT_ERROR(ret, "Select::RemoveHandle(). Invalid descriptor.");


    if (ret){
    	bool found = false;
        for (int32 i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++) {
            if ((handles.selectHandles[i] == hSocket)&& !found)  {
                found = true;
            }
            if (found) {
            	handles.registeredHandles[i] = handles.registeredHandles[i + 1];
            	handles.selectHandles[i] = handles.selectHandles[i + 1];
            }
        }

        ret.invalidOperation = !found;
        REPORT_ERROR(ret, "Select::RemoveHandle not found.");
    }

    if (ret){
        highestHandle--;
    }

	return ret;
}
#endif

uint32 SelectProperties::Find(HANDLE handle) const{
	bool found = false;
	uint32 index = 0;
	uint32 maxIndex = handleDB.GetSize();
	while (!found && (index < maxIndex)){
		found = (handleDB[index] == handle);
		if (!found) {
			index++;
		}
	}

	if (!found){
		index = 0xFFFFFFFF;
	}
	return index;
}

uint32 SelectProperties::SocketFind(SOCKET socket) const{
	bool found = false;
	uint32 index = 0;
	uint32 maxIndex = socketHandleDB.GetSize();
	while (!found && (index < maxIndex)){
		found = (socketHandleDB[index] == socket);
		if (!found) {
			index++;
		}
	}

	if (!found){
		index = 0xFFFFFFFF;
	}
	return index;
}


ErrorManagement::ErrorType SelectProperties::Add(HANDLE handle, const HandleFlags &flag){
	ErrorManagement::ErrorType ret;

	uint32 index = Find(handle);

	if (index < handleDB.GetSize()){
		handleFlagsDB.Access(index).Merge(flag);
	} else {
    	ret.outOfRange = (handleDB.GetSize() >= MAXIMUM_WAIT_OBJECTS);
    	REPORT_ERROR(ret,"Too many Handles");

    	if (ret){
    		ret.fatalError =  !handleDB.Add(handle);
        	REPORT_ERROR(ret," handleDB.Add failed");
    	}

    	if (ret){
    		ret.fatalError =  !handleFlagsDB.Add(flag);
        	REPORT_ERROR(ret," handleFlagsDB.Add failed");
    	}

    	if (ret){
    		ret.fatalError =  !socketHandleDB.Add(0);
        	REPORT_ERROR(ret," socketHandleDB.Add failed");
    	}
	}

	return ret;
}

ErrorManagement::ErrorType SelectProperties::AddSocket(SOCKET socket, const HandleFlags &flag){
	ErrorManagement::ErrorType ret;

	uint32 index = SocketFind(socket);

	WSAEVENT hEvent = 0;

	if (index < handleDB.GetSize()){
		handleFlagsDB.Access(index).Merge(flag);

		WSAEVENT hEvent = handleDB[index];

		ret.OSError = (WSAEventSelect(socket,hEvent,handleFlagsDB.Access(index).GetEvents())!=0);
    	REPORT_ERROR(ret,"WSAEventSelect failed");

	} else {
    	ret.outOfRange = (handleDB.GetSize() >= MAXIMUM_WAIT_OBJECTS);
    	REPORT_ERROR(ret,"Too many Handles");

    	if (ret){
    		ret.fatalError =  !socketHandleDB.Add(socket);
        	REPORT_ERROR(ret," socketHandleDB.Add failed");
    	}

    	if (ret){
    		hEvent = WSACreateEvent();
    		ret.fatalError =  !handleDB.Add(hEvent);
        	REPORT_ERROR(ret," handleDB.Add failed");
    	}

    	if (ret){
    		ret.OSError = (WSAEventSelect(socket,hEvent,flag.GetEvents())!=0);
        	REPORT_ERROR(ret,"WSAEventSelect failed");
    	}

    	if (ret){
    		ret.fatalError =  !handleFlagsDB.Add(flag);
        	REPORT_ERROR(ret," handleFlagsDB.Add failed");
    	}
	}

	return ret;

}

ErrorManagement::ErrorType SelectProperties::Remove(HANDLE handle, const HandleFlags &flag){
	ErrorManagement::ErrorType ret;

	uint32 index = Find(handle);

	ret.unsupportedFeature = (index >= handleDB.GetSize());
	REPORT_ERROR(ret,"Handle not found");

	if (ret){

		handleFlagsDB.Access(index).Subtract(flag);

		if (handleFlagsDB[index].IsEmpty()){
			ErrorManagement::ErrorType ret2;
			ret2.fatalError =  !handleDB.Remove(index);
	    	COMPOSITE_REPORT_ERROR(ret2," handleDB.Remove(",index,") failed");
	    	ret.internalSetupError = ret.internalSetupError | !ret2;

			ret2.fatalError =  !socketHandleDB.Remove(index);
	    	COMPOSITE_REPORT_ERROR(ret2," socketHandleDB.Remove(",index,") failed");
	    	ret.internalSetupError = ret.internalSetupError | !ret2;

			ret2.fatalError =  !handleFlagsDB.Remove(index);
	    	COMPOSITE_REPORT_ERROR(ret2," handleFlagsDB.Remove(",index,") failed");
	    	ret.internalSetupError = ret.internalSetupError | !ret2;
		} else {
			ret = Add(handle,handleFlagsDB[index]);
		}
	}
	return ret;
}


ErrorManagement::ErrorType SelectProperties::RemoveSocket(SOCKET socket, const HandleFlags &flag){
	ErrorManagement::ErrorType ret;

	uint32 index = SocketFind(socket);

	ret.unsupportedFeature = (index >= handleDB.GetSize());
	REPORT_ERROR(ret,"Handle not found");

	if (ret){

		handleFlagsDB.Access(index).Subtract(flag);

		if (handleFlagsDB[index].IsEmpty()){
			ret.OSError = (WSACloseEvent(handleDB[index]) != TRUE);
			REPORT_ERROR(ret,"WSACloseEvent failed");

			ErrorManagement::ErrorType ret2;

			ret2.fatalError =  !handleDB.Remove(index);
	    	COMPOSITE_REPORT_ERROR(ret2," handleDB.Remove(",index,") failed");
	    	ret.internalSetupError = ret.internalSetupError | !ret2;

			ret2.fatalError =  !socketHandleDB.Remove(index);
	    	COMPOSITE_REPORT_ERROR(ret2," handleDB.Remove(",index,") failed");
	    	ret.internalSetupError = ret.internalSetupError | !ret2;

			ret2.fatalError =  !handleFlagsDB.Remove(index);
	    	COMPOSITE_REPORT_ERROR(ret2," handleFlagsDB.Remove(",index,") failed");
	    	ret.internalSetupError = ret.internalSetupError | !ret2;
		} else {
			ret = AddSocket(socket,handleFlagsDB[index]);
		}
	}

	return ret;
}

void SelectProperties::Clean(){
	ErrorManagement::ErrorType ret;

	for (uint32 i=0; i< handleDB.GetSize();i++){
		if (socketHandleDB[i]!=0){
			ErrorManagement::ErrorType ret;
			ret.OSError = (WSACloseEvent(handleDB[i]) != TRUE);
			REPORT_ERROR(ret,"WSACloseEvent failed");
		}
	}

	socketHandleDB.Clean();
	handleDB.Clean();
	handleFlagsDB.Clean();
}

HandleFlags SelectProperties::GetProperties(HANDLE handle) const{
	HandleFlags ret;

	uint32 index = Find(handle);

	if (index < handleFlagsDB.GetSize()){
		ret =  handleFlagsDB[index];
	}
	return ret;
}

bool Select::Add(const Handle &handle,bool readEvent,bool writeEvent,bool exceptEvent){
	ErrorManagement::ErrorType ret;
	HANDLE h = handle;
	ret.internalSetupError = ((h==NULL) || (h == INVALID_HANDLE_VALUE));
	REPORT_ERROR(ret,"invalid Handle ");

	if (ret){
		ret = 	selectProperties.Add(h,HandleFlags(readEvent,writeEvent,exceptEvent));
		REPORT_ERROR(ret,"selectProperties.Add(h,readFlag) failed");
	}

	return ret;
}

bool Select::Add(const BasicSocket &socket,bool readEvent,bool writeEvent,bool exceptEvent){
	ErrorManagement::ErrorType ret;
	ret = 	selectProperties.AddSocket(socket.GetSocket(),HandleFlags(readEvent,writeEvent,exceptEvent));
	REPORT_ERROR(ret,"selectProperties.AddSocket(h,readFlag) failed");
	return ret;
}

bool Select::Remove(const Handle &handle,bool readEvent,bool writeEvent,bool exceptEvent){
	ErrorManagement::ErrorType ret;
	HANDLE h = handle;
	ret = 	selectProperties.Remove(h,HandleFlags(readEvent,writeEvent,exceptEvent));
	REPORT_ERROR(ret,"selectProperties.Remove(h,readFlag) failed");
	return ret;
}

bool Select::Remove(const BasicSocket &socket,bool readEvent,bool writeEvent,bool exceptEvent){
	ErrorManagement::ErrorType ret;
	ret = 	selectProperties.RemoveSocket(socket.GetSocket(),HandleFlags(readEvent,writeEvent,exceptEvent));
	REPORT_ERROR(ret,"selectProperties.Remove(h,readFlag) failed");
	return ret;
}

#if 0
bool Select::AddReadHandle(const Handle &handle) {
	ErrorManagement::ErrorType ret;
	Handle h = handle.GetReadHandle();
	ret = 	selectProperties.Add(h,readFlag);
	REPORT_ERROR(ret,"selectProperties.Add(h,readFlag) failed");
	return ret;

//	return AddHandle(handle, readHandle,highestHandle,FD_READ);

/*
	ErrorManagement::ErrorType ret;

    Handle hSocket = handle.GetReadHandle();
    ret.parametersError = (hSocket < 0);
    REPORT_ERROR(ret, "Select::AddHandle(). Invalid descriptor.");

    if (ret){
        ret.outOfRange = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
        REPORT_ERROR(ret,"too many handles");
    }

    int32 indexMax = 0L;
    if (ret){
        while (readHandle.selectHandles[indexMax] != NULL) {
            indexMax++;
        }
        for (int32 i = 0; (i < indexMax) && ret; i++) {
            ret.illegalOperation = (readHandle.registeredHandles[i] == hSocket);
            REPORT_ERROR(ret, "Select::AddHandle(), Same Handle.");
        }
    }

    WSAEVENT wsaevent = WSA_INVALID_EVENT;
    if (ret){
        wsaevent = WSACreateEvent();
        ret.OSError = (wsaevent == WSA_INVALID_EVENT);
        REPORT_ERROR(ret, "Select::WSACreateEvent failed.");
    }

    if (ret){
        int32 error = WSAEventSelect(reinterpret_cast<SOCKET>(hSocket), wsaevent, FD_READ);
        if (error != SOCKET_ERROR) {
            readHandle.registeredHandles[indexMax] = wsaevent;
        }
        else {
            readHandle.registeredHandles[indexMax] = hSocket;
        }
        readHandle.selectHandles[indexMax] = hSocket;
        highestHandle++;
    }

    return ret;
*/
}

bool Select::AddWriteHandle(const Handle &handle) {
	ErrorManagement::ErrorType ret;
	Handle h = handle.GetWriteHandle();
	ret = 	selectProperties.Add(h,writeFlag);
	REPORT_ERROR(ret,"selectProperties.Add(h,readFlag) failed");
	return ret;

//	return AddHandle(handle, writeHandle,highestHandle,FD_WRITE);
/*
	ErrorManagement::ErrorType ret;

    Handle hSocket = handle.GetReadHandle();
    ret.parametersError = (hSocket < 0);
    REPORT_ERROR(ret, "Select::AddHandle(). Invalid descriptor.");

    if (ret){
        ret.outOfRange = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
        REPORT_ERROR(ret,"too many handles");
    }

    int32 indexMax = 0L;
    if (ret){
        while (writeHandle.selectHandles[indexMax] != NULL) {
            indexMax++;
        }
        for (int32 i = 0; (i < indexMax) && ret; i++) {
            ret.illegalOperation = (writeHandle.registeredHandles[i] == hSocket);
            REPORT_ERROR(ret, "Select::AddHandle(), Same Handle.");
        }
    }

    WSAEVENT wsaevent = WSA_INVALID_EVENT;
    if (ret){
        wsaevent = WSACreateEvent();
        ret.OSError = (wsaevent == WSA_INVALID_EVENT);
        REPORT_ERROR(ret, "Select::WSACreateEvent failed.");
    }

    if (ret){
        int32 error = WSAEventSelect(hSocket, wsaevent, FD_WRITE);
        if (error != SOCKET_ERROR) {
        	writeHandle.registeredHandles[indexMax] = wsaevent;
        }
        else {
        	writeHandle.registeredHandles[indexMax] = hSocket;
        }
        writeHandle.selectHandles[indexMax] = hSocket;
        highestHandle++;
    }

    return ret;
    */
}

bool Select::AddExceptionHandle(const Handle &handle) {
	ErrorManagement::ErrorType ret;
	Handle h = handle.GetReadHandle();
	ret =selectProperties.Add(h,exceptionFlag);
	REPORT_ERROR(ret,"selectProperties.Add(h,readFlag) failed");
	return ret;

//	return AddHandle(handle, exceptionHandle,highestHandle,FD_READ);

	/*

    bool retVal = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
    Handle h = handle.GetReadHandle();
    int32 indexMax = 0L;
    if (reinterpret_cast<int32>(h) >= 0) {
        if (retVal) {
            while (exceptionHandle.selectHandles[indexMax] != NULL) {
                indexMax++;
            }
            for (int32 i = 0; i < indexMax; i++) {
                if (exceptionHandle.registeredHandles[i] == h) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(), Same Handle.");
                    retVal = false;
                }
            }
        }
        if (retVal) {
            WSAEVENT wsaevent;
            wsaevent = WSACreateEvent();
            if (wsaevent != WSA_INVALID_EVENT) {
                int32 hSocket = (int32) h;
                int32 error = WSAEventSelect(hSocket, wsaevent, FD_READ);
                if (error != SOCKET_ERROR) {
                    exceptionHandle.registeredHandles[indexMax] = wsaevent;
                }
                else {
                    exceptionHandle.registeredHandles[indexMax] = h;
                }
                exceptionHandle.selectHandles[indexMax] = h;
                highestHandle++;
            }

        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
    */
}


bool Select::RemoveReadHandle(const Handle &handle) {
	ErrorManagement::ErrorType ret;
	Handle h = handle.GetReadHandle();
	ret = 	selectProperties.Remove(h);
	REPORT_ERROR(ret,"selectProperties.Remove(h) failed");
	return ret;

//	return RemoveHandle(handle, readHandle,highestHandle);
/*
	bool retVal = false;
    Handle h = handle.GetReadHandle();
    if (reinterpret_cast<int32>(h) >= 0) {
        for (int32 i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++) {
            if (readHandle.selectHandles[i] == h) {
                retVal = true;
            }
            if (retVal) {
                readHandle.registeredHandles[i] = readHandle.registeredHandles[i + 1];
                readHandle.selectHandles[i] = readHandle.selectHandles[i + 1];
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    if (retVal) {
        highestHandle--;
    }
    return retVal;
    */
}

bool Select::RemoveWriteHandle(const Handle &handle) {
	ErrorManagement::ErrorType ret;
	Handle h = handle.GetWriteHandle();
	ret = 	selectProperties.Remove(h);
	REPORT_ERROR(ret,"selectProperties.Remove(h) failed");
	return ret;
//	return RemoveHandle(handle, writeHandle,highestHandle);
	/*
    bool retVal = false;
    Handle h = handle.GetWriteHandle();
    if (reinterpret_cast<int32>(h) >= 0) {
        for (int32 i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++) {
            if (writeHandle.selectHandles[i] == h) {
                retVal = true;
            }
            if (retVal) {
                writeHandle.registeredHandles[i] = writeHandle.registeredHandles[i + 1];
                writeHandle.selectHandles[i] = writeHandle.selectHandles[i + 1];
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    if (retVal) {
        highestHandle--;
    }
    return retVal;
    */
}

bool Select::RemoveExceptionHandle(const Handle &handle) {
	ErrorManagement::ErrorType ret;
	Handle h = handle.GetReadHandle();
	ret = 	selectProperties.Remove(h);
	REPORT_ERROR(ret,"selectProperties.Remove(h) failed");
	return ret;

//	return RemoveHandle(handle, exceptionHandle,highestHandle);
/*
    bool retVal = false;
    Handle h = handle.GetReadHandle();
    if (reinterpret_cast<int32>(h) >= 0) {
        for (int32 i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++) {
            if (exceptionHandle.selectHandles[i] == h) {
                retVal = true;
            }
            if (retVal) {
                exceptionHandle.registeredHandles[i] = exceptionHandle.registeredHandles[i + 1];
                exceptionHandle.selectHandles[i] = exceptionHandle.selectHandles[i + 1];
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    if (retVal) {
        highestHandle--;
    }
    return retVal;
    */
}

#endif

void Select::ClearAllHandles() {
	selectProperties.Clean();
/*
    for (int32 i = 0; i < highestHandle; i++) {
        readHandle.registeredHandles[i] = NULL;
        readHandle.selectHandles[i] = NULL;

        writeHandle.registeredHandles[i] = NULL;
        writeHandle.selectHandles[i] = NULL;

        exceptionHandle.registeredHandles[i] = NULL;
        exceptionHandle.selectHandles[i] = NULL;
    }
    highestHandle = 0;
*/
    return;
}

bool Select::IsSet(const Handle &handle,bool readEvent,bool writeEvent,bool exceptEvent) const {
	HandleFlags hf = selectProperties.GetProperties(handle);
	return  hf.hasBeenSelected;
}

bool Select::IsSet(const BasicSocket &socket,bool readEvent,bool writeEvent,bool exceptEvent) const {
	ErrorManagement::ErrorType ret;

	uint32 index = selectProperties.SocketFind(socket.GetSocket());

	ret.unsupportedFeature = (index >= selectProperties.handleDB.GetSize());
	REPORT_ERROR(ret,"Handle not found");

	bool result = false;
	if (ret){
		HandleFlags hf = selectProperties.GetProperties(selectProperties.handleDB[index]);
		result = hf.hasBeenSelected;
	}
	return result;
}

/*
    bool retVal = false;
    Handle hr = handle.GetReadHandle();
    if (hr >= 0) {
        if (readHandle.selectedHandle == hr) {
            retVal = true;
        }

        if (writeHandle.selectedHandle == hr) {
            retVal = true;
        }

        if (exceptionHandle.selectedHandle == hr) {
            retVal = true;
        }
    }
    Handle hw = handle.GetWriteHandle();
    if (hw >= 0) {
        if (readHandle.selectedHandle == hw) {
            retVal = true;
        }

        if (writeHandle.selectedHandle == hw) {
            retVal = true;
        }

        if (exceptionHandle.selectedHandle == hw) {
            retVal = true;
        }
    }
    return retVal;
    */


int32 Select::WaitUntil(const MilliSeconds &msecTimeout) {

	uint32 nOfHandles = selectProperties.handleDB.GetSize();
	const HANDLE *handles = selectProperties.handleDB.GetAllocatedMemoryConst();
    uint32 selected = WaitForMultipleObjectsEx(nOfHandles, handles, false, msecTimeout.GetTimeRaw(), true);

    if (selected < (WAIT_OBJECT_0 + selectProperties.handleFlagsDB.GetSize())){
        selectProperties.handleFlagsDB.Access(selected-WAIT_OBJECT_0).hasBeenSelected = true;

        return 1;
    }
    if (selected == WAIT_TIMEOUT){
    	return 0;
    }

    return -1;
}
#if 0
    Handle * allHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    uint8 i = 0;
    uint16 counter = 0;
    uint16 regRead = 0;
    uint16 regWrite = 0;
    while (readHandle.registeredHandles[i] != NULL) {
        allHandles[counter] = readHandle.registeredHandles[i];
        i++;
        counter++;
    }
    regRead = counter;
    i = 0;
    while (writeHandle.registeredHandles[i] != NULL) {
        allHandles[counter] = writeHandle.registeredHandles[i];
        i++;
        counter++;
    }
    regWrite = counter;
    i = 0;
    while (exceptionHandle.registeredHandles[i] != NULL) {
        allHandles[counter] = exceptionHandle.registeredHandles[i];
        i++;
        counter++;
    }
    int32 retVal= -1;
    uint32 selected = WaitForMultipleObjectsEx(static_cast<DWORD>(highestHandle), &allHandles[0], false, msecTimeout.GetTimeRaw(), true);
    if (selected < (WAIT_OBJECT_0 + counter)){
    	retVal = 1;
    	if (selected < regRead){
            readHandle.selectedHandle = allHandles[selected];
    	} else
    	if (selected < regWrite){
    		writeHandle.selectedHandle = allHandles[selected];
    	} else {
            exceptionHandle.selectedHandle = allHandles[selected];
    	}
    } else
    if (selected == WAIT_TIMEOUT){
    	retVal = 0;
    }
    delete[] allHandles;
    return retVal;
}
#endif
}

