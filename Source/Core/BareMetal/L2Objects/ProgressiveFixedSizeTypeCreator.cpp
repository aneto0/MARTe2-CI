/**
 * @file ProgressiveFixedSizeTypeCreator.cpp
 * @brief Header file for class AnyType
 * @date 10 Jan 2018
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

#define DLL_API

#include <new.h>
#include "ProgressiveFixedSizeTypeCreator.h"
#include "TypeConversionManager.h"
#include "GlobalObjectsDatabase.h"

namespace MARTe{



/**
 * @brief a class to contain synthetic objects
 */
class MemoryPageObject: public Object{

public:
    CLASS_REGISTER_DECLARATION()


	/**
	 * constructor
	 */
	MemoryPageObject(){	}

	/**
	 * destructor
	 */
	virtual ~MemoryPageObject(){	}

	/**
	 * constructor
	 */
	void Copy(MemoryPage &toStealFrom){
		mp.Copy(toStealFrom);
	}

    /**
     * @brief Allows to setup an Anytype of any type....
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] modifiers contains the modifiers string in the appropriate language (see VariableDescriptor).
     * @param[in] dataPointerIn is the pointer to the constant data.
     * @post
     *   GetDataPointer() == dataPointerIn &&
     *   GetDataDescriptor() == dataDescriptorIn
     */
    void Setup(TypeDescriptor dataDescriptorIn,CCString modifiers,const void* const dataPointerIn){
    	anyType.Setup(dataDescriptorIn,modifiers,dataPointerIn);
    }

	/**
	 * @brief The only interface provided by an AnyObject is the ability to provide its data via an AnyType.
	 * @return a valid AnyType that describes the content of this object and allows read only access to its content
	 */
	virtual operator AnyType(){
		return anyType;
	}

private:
	/**
	 * contains the memory of the object
	 */
	MemoryPage 	mp;

	/**
	 * the descriptor of the type
	 * and the reference to it
	 */
	AnyType 	anyType;
};

CLASS_REGISTER(MemoryPageObject, "1.0")

/**
 * @brief Creates the object selecting the type to convert to and the default PageSize
 */
ProgressiveFixedSizeTypeCreator::ProgressiveFixedSizeTypeCreator(uint32 pageSizeIn ){
	defaultPageSize 	= pageSizeIn;
	converter 			= NULL;
	Clean();
}

/**
 * @deletes the object and any memory allocated in the pages
 */
ProgressiveFixedSizeTypeCreator::~ProgressiveFixedSizeTypeCreator(){
	Clean();
}


void ProgressiveFixedSizeTypeCreator::Clean(){
	if (converter != NULL){
		delete converter;
		converter = NULL;
	}
	page.Clean();
	type 				= InvalidType(0);
	objectSize 			= 0;
	pageSize			= 0;
	sizeLeft 			= 0;
	pageWritePos 		= 0;
	status 				= notStarted;
	converter 			= NULL;
	matrixRowSize 		= 0;
	vectorSize 			= 0;
	currentVectorSize 	= 0;
	numberOfPages 		= 0;
	isString            = false;
	sizeStack.Clean();
	numberOfElements    = 0;
}

ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::Start(TypeDescriptor typeIn){
	ErrorManagement::ErrorType ret;

	Clean();

	type 				= typeIn;
	objectSize 			= typeIn.StorageSize();
	isString 			= typeIn.IsCharString();
	if (objectSize == 0){
		ret.parametersError = true;
		REPORT_ERROR(ret,"Type with 0 size");
	}
	if (!(isString || typeIn.IsBasicType())){
		ret.parametersError = !(isString || typeIn.IsBasicType());
		REPORT_ERROR(ret,"Unsupported type. Must be CString or BasicType");
	}
	if (ret && !isString){
		converter 			= TypeConversionManager::Instance().GetOperator(type,ConstCharString(sizeof(CString)),false);
		if (converter == NULL){
			ret.unsupportedFeature = true;
			REPORT_ERROR(ret,"Cannot find type converter");
		}
	}

	if (ret){
		status 				= started;
	} else {
		status 				= error;
	}
	return ret;
}

ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CheckMemoryStringEl(uint32 neededSize){
	ErrorManagement::ErrorType ret;

	if (ret){
		ret = CheckAndClosePage(neededSize);
	}

	// check for initial 0 size page or begin of new vector 0 size page (after CheckAndClosePage)
	if (ret){
		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		ret = CheckAndNewPage(defaultPageSize);
	}
	return ret;
}

static inline uint32 max(uint32 x,uint32 y){
	uint32 ret = x;
	if (y > x) ret = y;
	return ret;
}

ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CheckMemoryFixedSizeEl(bool newRow){
	ErrorManagement::ErrorType ret;

	// in case of new vector check if there is space for one based on the size of the previous
	// close page otherwise
	if (ret && newRow){
		ret = CheckAndClosePage(vectorSize * objectSize);
	}

	// check for initial 0 size page or begin of new vector 0 size page (after CheckAndClosePage)
	if (ret){
		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		ret = CheckAndNewPage(max(defaultPageSize,vectorSize * objectSize));
	}

	uint32 neededSize = objectSize;
	// check memory availability to complete current vector
	if (ret){
		// not enough space
		if (neededSize > sizeLeft){

			// need to allocate a new page and shrink this page?
			// or need to move part of current page into a new page and shrink this page?
			// or need to increase page size

			// there were some other vectors in this page
			// increase so that we can fit also this last one
			uint32 currentVectorUsedSize = (currentVectorSize-1) * objectSize;
			if (currentVectorUsedSize <= pageSize){
				// add enough to store the biggest vector so far
				ret = PageGrow(vectorSize * objectSize);

			} else { // means that one vector cannot fit in the page

				// multiply the size
				ret = PageGrow(defaultPageSize);
			}
		}
	}

	return ret;
}

	/**
	 * @adds an element. It will convert the string to the specified type
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::AddElement(CCString typeStringRepresentation){
		ErrorManagement::ErrorType ret;

		if (!Started()){
			ret.internalStateError = true;
			REPORT_ERROR(ret,"AddElement and status not started");
		}

		bool newRow = false;
		if (ret){
			/// update status
			switch (status){
			case started:{
				status = scalar;
				vectorSize = 1;
				matrixRowSize = 1;
				currentVectorSize = 1;
			}break;
			case scalar:{
				status = vector;
				vectorSize = 2;
				currentVectorSize = 2;
			}break;
			case vectorEnd:
			case matrixRowEnd:{
				status = matrixRow;
				currentVectorSize = 1;
				matrixRowSize++;
				newRow = true;
			}break;
			case sparseMatrixRE:{
				status = sparseMatrixRow;
				currentVectorSize=1;
				matrixRowSize++;
				newRow = true;
			}break;
			case vector:
			case sparseMatrixRow:
			case matrixRow:{
				currentVectorSize++;
			}break;
			default:{
				status = error;
				ret.unsupportedFeature = true;
				REPORT_ERROR(ErrorManagement::UnsupportedFeature,"unexpected state");
			}
			}
		}

		uint32 neededSize = objectSize;
		if (ret){
			if (isString){
				neededSize = typeStringRepresentation.GetSize()+1;
				// checks memory available in current page
				// if not enough opens a new page
				ret = CheckMemoryStringEl(neededSize);

				if (ret){
					MemoryOperationsHelper::Copy(page.Address(pageWritePos),typeStringRepresentation.GetList(),neededSize);
				}

			} else {
				// checks memory available in current page
				// if not enough resize
				// in case of new Rows check if space is enough for a row
				// otherwise opens a new page
				ret = CheckMemoryFixedSizeEl(newRow);

				if (ret){
					// queue type to memory
					ret = converter->Convert(	reinterpret_cast<uint8 *>(page.Address(pageWritePos)),
										reinterpret_cast<const uint8 *>(&typeStringRepresentation),1);
				}
			}
		}

		// all ok - means we used some memory
		if (ret){
			sizeLeft -= neededSize;
			pageWritePos += neededSize;
			numberOfElements++;
		} else {
			status = error;
			REPORT_ERROR(ret,"AddElement failed");
		}
		return ret;
	}


	/**
	 * @brief Marks the end of a row of elements
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::EndVector(){
		ErrorManagement::ErrorType  ret;

		/// update status
		switch (status){
		case started:{
			ret.illegalOperation = true;
			status = error;
		}break;
		case vector:
		case scalar:{
			status = vectorEnd;
			vectorSize = currentVectorSize;
			currentVectorSize = 0;
		}break;
		case vectorEnd:
		case matrixRowEnd:
		case matrixRow:{
			if (currentVectorSize != vectorSize){
				status = sparseMatrixRE;
				int i;
				for (i=0;i<(matrixRowSize-1);i++){
					sizeStack.Push(vectorSize);
				}
				sizeStack.Push(currentVectorSize);
			} else {
				status = matrixRowEnd;
			}
			if (currentVectorSize > vectorSize){
				vectorSize = currentVectorSize;
			}
			currentVectorSize = 0;
		}break;
		case sparseMatrixRow:
		case sparseMatrixRE:{
//printf("nEl = %i\n",currentVectorSize);
			sizeStack.Push(currentVectorSize);
			currentVectorSize = 0;
			status = sparseMatrixRE;
		}break;
		// if Finished() goes here
		default:{
			ret.illegalOperation = true;
			status = error;
		}
		}

		if (!ret){
			REPORT_ERROR(ret,"EndVector failed");
		}
		return ret;
	}

	/**
	 * @brief Marks the end of the object construction
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::End(){
		ErrorManagement::ErrorType  ret;

		switch (status){
		case started:{
			ret.notCompleted = true;
			status = error;
		}break;
		case scalar:{
			status = finishedS;
		}break;
		case vectorEnd:{
			status = finishedV;
		}break;
		case sparseMatrixRE:{
			status = finishedSM;
		}break;
		case matrixRowEnd:{
			status = finishedM;
		}break;
		case error:
		default:{
			status = error;
			REPORT_ERROR(ErrorManagement::InternalStateError,"unexpected state");
			ret.internalStateError = true;
		}
		}
		if (!ret){
			REPORT_ERROR(ret,"End() failed");
		}
		return ret;
	}

	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CompleteFixedSizeEl(uint32 &auxSize,void *&auxPtr){
		ErrorManagement::ErrorType  ret;

		if (status == finishedSM){
			auxSize  = sizeof (Vector<uint8>) * matrixRowSize;
		} else
		if ((status == finishedM)&&(numberOfPages > 1)){
			auxSize = sizeof (void *) * matrixRowSize;
		}

		if (auxSize > 0){
			// shrink if needed
			ret = CheckAndClosePage(auxSize);

			// new page if needed
			if (ret){
				ret = CheckAndNewPage(auxSize);
			}

			// get the address map and close up memory pages
			if (ret){
				auxPtr = reinterpret_cast<void *>(page.Address(this->pageWritePos));
				pageWritePos += auxSize;
				sizeLeft -= auxSize;
				ret = CheckAndClosePage(0);
			}
		}

		if ((numberOfPages > 0) && ret){
			page.FlipOrder();
		}

		return ret;
	}

	static inline bool stringBoundSize(CCString s, uint32 &sizeInOut){
		const char8 *p = s.GetList();
		bool ret = (p!=NULL);

		if (ret){
			uint32 sz = 0;
			while ((sz < sizeInOut) && (*p!='\0')){
				p++;
				sz++;
			}

			if (*p == '\0'){
				sizeInOut = sz;
			} else {
				ret = false;
			}
		}
		return ret;
	}


	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CompleteStringEl(void *&dataPtr,uint32 &auxSize,void *&auxPtr){
		ErrorManagement::ErrorType  ret;
/*
		uint32 numberOfElements = matrixRowSize * vectorSize;
		if (status == finishedSM){
			numberOfElements = 0;
			for (uint32 i= 0;i<matrixRowSize;i++){
				numberOfElements += sizeStack[i];
			}
		}
*/
		// table of CCString (pointers)
		uint32 CCStringTableSize = numberOfElements * sizeof(CCString);
		// table of Vector<CCString>
		if (status == finishedSM){
			auxSize = sizeof (Vector<uint8>) * matrixRowSize;
		}

		/** try to fit both */
		uint32 neededSize = CCStringTableSize + auxSize;

		if (ret){
			// close if not big enough
			ret = CheckAndClosePage(neededSize);
		}

		if (ret){
			// open if no page
			ret = CheckAndNewPage(neededSize);
		}

		// process memory and create list of pointers
		CCString *strings = NULL;
		if (ret){
			// remember pointer to string array
			strings = reinterpret_cast<CCString *>(page.Address(pageWritePos));
			if (auxSize > 0){
				auxPtr = page.Address(pageWritePos + CCStringTableSize);
			}

			// put oldest page on top
			if (numberOfPages > 1){
				page.FlipOrder();
				numberOfPages = 1;
			}

			uint64 pageDepth=0;
			// prepare table of string addresses
			for (int i = 0;(i<numberOfElements) && ret;i++){
				// consecutiveSpan as input is how many character we need
				uint32 consecutiveSpan=1;
				// get pointer to element within pages at overall address pageDepth
				CCString s = reinterpret_cast<char8 *> (page.DeepAddress(pageDepth,consecutiveSpan));
				// consecutiveSpan now is how much space is left on page
//printf("string=%s span = %i\n",s.GetList(),consecutiveSpan);
				// empty string error?
				if (s.GetList() == NULL){
					ret.fatalError = true;
					DynamicCString errM;
					errM.AppendN("String ");
					errM.AppendNum(i);
					errM.AppendN(" has zero length");
					REPORT_ERROR(ret,errM.GetList());
				}

				// calculate string length
				uint32 length = consecutiveSpan;
				if (ret){
					// length but do not scan beyond end of
					if (!stringBoundSize(s,length)){
						ret.fatalError = true;
						DynamicCString errM;
						errM.AppendN("String ");
						errM.AppendNum(i);
						errM.AppendN(" exceeding page boundaries ");
						errM.AppendNum(consecutiveSpan);
						REPORT_ERROR(ret,errM.GetList());
					}
				}

				// update pointer to page
				if (ret){
					// include 0
					length = length+1;
					strings[i] = s;
					pageDepth += length;
				}
			}
		}

		if (ret){
			dataPtr = reinterpret_cast<void *>(strings);
			if (status == finishedSM){
				auxPtr = page.Address(pageWritePos + auxSize);
			}
		}

		return ret;
	}

	/**
	 * @brief Allows retrieving the Object that has been built.
	 * Can only be done after End has been called
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::GetReference(Reference &x){
		ErrorManagement::ErrorType  ret;

		if (!Finished()){
			ret.fatalError = true;
			REPORT_ERROR(ret,"Not Finished");
		}

		// points to data or to pointers to data (CCString)
		void *dataPtr = NULL;
		// auxiliary structures (array of pointers or array of Vector<>
		void *auxPtr  = NULL;
		uint32 auxSize = 0;

		if (ret){
			if (isString){
				// Flip multi page data
				// prepares pointers to actual strings - returns that as main data
				// allocate Vector<> array in case of sparse matrices
				ret = CompleteStringEl(dataPtr,auxSize,auxPtr);
			} else {
				// Flip multi page data
				// allocate Vector<> array in case of sparse matrices
				// allocate array of pointers in case of multipage data
				ret = CompleteFixedSizeEl(auxSize,auxPtr);
				dataPtr =  page.Address(0);
			}
		}

		if (ret){
			// Completes operation
			// fills aux Data
			// creates Object
			ret = GetReferencePrivate(x, dataPtr, auxPtr,auxSize);
		}

		if (!ret) {
			REPORT_ERROR(ret,"GetReference failed");
		}

		return ret;
	}

	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::GetReferencePrivate(Reference &x, void *dataPtr, void *auxPtr,uint32 auxSize){
		ErrorManagement::ErrorType  ret;

		DynamicCString mods;
		switch (status){
		case finishedS:{
			CheckAndClosePage(0);
		}break;
		case finishedV:{
			CheckAndClosePage(0);
			mods.Append('A');
			mods.AppendNum(vectorSize);
		}break;
		case finishedM:{
			// auzPtr not NULL means fragmented
			if (auxPtr != NULL){
				if (auxSize != sizeof (void *) * matrixRowSize){
					ret.internalSetupError = true;
					REPORT_ERROR(ret,"auxSize is not adequate");
				}

				uint64 pageDepth;
				if (ret){
					ret = page.Address2Index(dataPtr,pageDepth);
				}

				// reorder the pages correctly to allow access to data
				if (ret){
					void **addressMap = reinterpret_cast<void **>(auxPtr);
//printf("addressMap = %p\n",addressMap);

//					uint64 pageDepth=0;
					uint32 vectorByteSize = vectorSize * objectSize;
//printf("vectorByteSize = %i\n",vectorByteSize);
					for (int i = 0;(i<matrixRowSize) && ret;i++){
						uint32 sizeLeftOnPage = vectorByteSize;
						void *address = page.DeepAddress(pageDepth,sizeLeftOnPage);
						if (address == NULL){
							ret.fatalError = true;
							REPORT_ERROR(ret,"deep address out of boundary");
						}

						if (ret){
//printf("depth = %lli address = %p %i\n",pageDepth,address,vectorByteSize);
							addressMap[i] = address;
							pageDepth = pageDepth + vectorByteSize;
						}
					}
				}

				if (ret){
					mods.Append('A');
					mods.AppendNum(matrixRowSize);
					mods.AppendN("PA");
					mods.AppendNum(vectorSize);
					dataPtr = auxPtr;
				}

			} else {
				mods.Append('A');
				mods.AppendNum(matrixRowSize);
				mods.Append('A');
				mods.AppendNum(vectorSize);
			}
		}break;
		case finishedSM:{
			if ((auxSize != (sizeof (Vector<uint8>) * matrixRowSize)) || (auxPtr == NULL)){
				ret.internalSetupError = true;
				if (auxPtr == NULL){
					REPORT_ERROR(ret,"auxPtr is NULL");
				} else {
					REPORT_ERROR(ret,"auxSize is not adequate");
				}
			}

			uint64 pageDepth;
			if (ret){
				ret = page.Address2Index(dataPtr,pageDepth);
			}

			// now get the address and then reorder the pages correctly to allow access to data
			if (ret){
				Vector<uint8> *addressMap = reinterpret_cast<Vector<uint8>*>(auxPtr);
//printf("aux=%p aMap = %p\n",auxPtr,addressMap);

//printf("rows = %i\n",matrixRowSize);
				for (int i = 0;(i<matrixRowSize) && ret;i++){
					uint32 size  = sizeStack[i];
					uint32 vectorByteSize = size * objectSize;
					uint32 sizeLeftOnPage = vectorByteSize;
					uint8 *address = reinterpret_cast<uint8 *> (page.DeepAddress(pageDepth,sizeLeftOnPage));
					if (address == NULL_PTR(uint8 *)){
						ret.fatalError = true;
						DynamicCString errM;
						errM.AppendN("deep address ");
						errM.AppendNum(pageDepth);
						errM.AppendN(" out of boundary");
						REPORT_ERROR(ret,"deep address out of boundary");
					}
//printf("index = %i size = %i depth = %lli vectorS = %i\n",i,size, pageDepth,vectorByteSize);

					if (ret){
//printf("@%p -> (%p %i)\n",&addressMap[i],address,size);
						addressMap[i].InitVector(address,size);
						pageDepth += vectorByteSize;
					}
				}
			}

			if (ret){
				mods.Append('A');
				mods.AppendNum(matrixRowSize);
				mods.Append('V');
				dataPtr = auxPtr;
			}

		}break;
		case error:{
			ret.fatalError = true;
			REPORT_ERROR(ret,"On Error");
		}break;
		default:{
			ret.notCompleted = true;
			REPORT_ERROR(ret,"Unexpected state?");
		}
		}

		ReferenceT<MemoryPageObject> mpor;
		if (ret){
			//ReferenceT<MemoryPageObject> mpor2(buildNow);
			mpor = ReferenceT<MemoryPageObject> (buildNow);
			if (!mpor.IsValid()){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"MemoryPageObject construction failed");
			}
		}

		if (ret){
//printf("data Ptr = %p\n",dataPtr);
			mpor->Setup(type,mods,dataPtr);
			mpor->Copy(page);
			x = mpor;

			status = notStarted;
		} else {
			REPORT_ERROR(ret,"GetReferencePrivate failed");
		}
		return ret;
	}


	/**
	 * @brief Check if the current segment has enough space to store another vector.
	 * If there is no need (neededSize== 0) or if the space is not large enough close the current memory page segment and open a new one
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CheckAndClosePage(uint32 neededSize){
		ErrorManagement::ErrorType ret;
		// do nothing if pageSize == 0
		if (pageSize > 0){
			// check if enough space to store another vector
			if ((sizeLeft < neededSize) || (neededSize == 0)){
				ret = page.Shrink(pageWritePos);
				if (ret){
//printf("end page to %i\n", pageWritePos);
					pageSize = 0;
					sizeLeft = 0;
					pageWritePos = 0;
				}
			}
		}
		if (!ret){
			DynamicCString errs;
			errs.AppendN("CheckAndClosePage(");
			errs.AppendNum(neededSize);
			errs.Append(')');
			REPORT_ERROR(ret,errs.GetList());
		}
		return ret;
	}


	/**
	 * @brief If pageSize is 0 allocate a new page of desired size
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CheckAndNewPage(uint32 neededSize){
		ErrorManagement::ErrorType ret;
		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		if (pageSize == 0){
			ret = page.Allocate(neededSize);
			if (ret){
//printf("new page %i @%p\n", neededSize,page.Address(0));
				sizeLeft = neededSize;
				pageWritePos = 0;
				pageSize = neededSize;
				numberOfPages++;
			}
		}
		if (!ret){
			DynamicCString errs;
			errs.AppendN("CheckAndNewPage(");
			errs.AppendNum(neededSize);
			errs.Append(')');
			REPORT_ERROR(ret,errs.GetList());
		}

		return ret;
	}


	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::PageGrow(uint32 amount){
		ErrorManagement::ErrorType ret;

		uint32 newPageSize = pageSize + amount;

		// check math overflow!
		if (newPageSize < pageSize){
			ret.fatalError = true;
			REPORT_ERROR(ret,"Overflow");
		}

		if (ret){
			ret = page.Grow(newPageSize);
			if (!ret){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"Out of Memory");
			}
		}

		if (ret){
			uint32 oldPageSize = pageSize;
			pageSize = page.Size();
			if (pageSize > oldPageSize){
				sizeLeft += (pageSize - oldPageSize);
//printf("end page to %i\n", pageSize);
			} else {
				ret.fatalError = true;
				REPORT_ERROR(ret,"page.Grow results in a smaller page!");
			}
		}

		return ret;
	}
	/**
	 * Header used in each page
	 */
	struct MemoryPageHeader{
		/**
		 * Link to previous/next
		 */
		MemoryPageHeader * 	previous;
		/**
		 * page size (payload only - no header)
		 */
		uint32				pageSize;
		/**
		 * Allow access to the payload section
		 */
		uint8 				*Data(){
			MemoryPageHeader *ptr = this +1;
			return reinterpret_cast<uint8 *>(ptr);
		};
		/**
		 * individual access to each member of payload
		 * range checks not performed
		 */
		uint8 &operator[] (uint32 index){
			return Data()[index];
		}
	};


	/**
	 * basic constructor empty pages
	 */
	MemoryPage::MemoryPage(){
		mph = NULL_PTR(MemoryPageHeader *);
	}
	/**
	 * @brief Steals content from another MemoryPage
	 */
	void MemoryPage::Copy (MemoryPage &stealFrom){
		mph = stealFrom.mph;
		stealFrom.mph = NULL_PTR(MemoryPageHeader *);
	}
	/**
	 * @brief Deletes all pages of memory
	 */
	MemoryPage::~MemoryPage(){
		Clean();
	}

	void MemoryPage::Clean(){
		while (mph != NULL_PTR(MemoryPageHeader *)){
			MemoryPageHeader * memoryP = mph->previous;
			free(mph);
			mph = memoryP;
		}
	}

	uint32 MemoryPage::Size(){
		uint32 size = 0;
		if (mph != NULL) {
			size = mph->pageSize;
		}
		return size;
	}


	ErrorManagement::ErrorType MemoryPage::Address2Index(void * address,uint64 &index) const{
		ErrorManagement::ErrorType ret;

		uint8 *addressU8 = reinterpret_cast<uint8 *>(address);
		index = 0;
		bool found = false;
		MemoryPageHeader *mphp = mph;
		while (!found && (mphp != NULL_PTR(MemoryPageHeader *))){
			uint8 *start = mphp->Data();
			uint8 *end   = mphp->Data() + mphp->pageSize;
			if ((addressU8 >= start) && (addressU8 <end)){
				found = true;
				index += (addressU8 - start);
			} else {
				index += mphp->pageSize;
				mphp = mphp->previous;
			}
		}

		if (!found){
			ret.outOfRange = true;
			REPORT_ERROR(ret,"address out of range");
		}

		return ret;
	}


	void *MemoryPage::DeepAddress(uint64 index,uint32 &consecutiveSpan){
//printf("%index = lli index\n",index);
		MemoryPageHeader *mphp = mph;

		uint32 localIndex = 0;
		while ((index > 0) && (mphp != NULL_PTR(MemoryPageHeader *))){
			if (index >= mphp->pageSize) {
				index -= mphp->pageSize;
				mphp = mphp->previous;
			} else {
				localIndex  = index;
				index = 0;
			}
		}

		void *address = NULL;
		if (mphp != NULL){
			uint32 requiredSpan = consecutiveSpan;
			consecutiveSpan = mphp->pageSize - localIndex;
			if (requiredSpan <= consecutiveSpan){
				address = reinterpret_cast<void *>((mphp->Data()+localIndex));
			}
//printf("%p req=%i avail=%i pos=%i\n",address, requiredSpan, consecutiveSpan,localIndex);
		}
		return address;
	}

	/**
	 * @brief return address of element index
	 */
	void *MemoryPage::Address(uint32 index){
		return reinterpret_cast<void *> (mph->Data()+index);
	}


	/**
	 * @brief increases current page to size newBufferSize
	 */
	ErrorManagement::ErrorType MemoryPage::Grow(uint32 newBufferSize){
		ErrorManagement::ErrorType ret;

		if (mph != NULL_PTR(MemoryPageHeader *)){
			// should grow not shrink
			if (newBufferSize <= mph->pageSize){
				ret.parametersError = true;
				REPORT_ERROR(ret,"Grow to a smaller size??");
			}
		}

		MemoryPageHeader *newMemory = NULL_PTR(MemoryPageHeader *);
		if (ret){
			// failure to allocate new page
			newMemory = reinterpret_cast<MemoryPageHeader*>(realloc(mph, newBufferSize + sizeof (MemoryPageHeader)));
			if (newMemory == NULL_PTR(MemoryPageHeader *)){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"realloc failed");
			}
		}

		if (ret){
			mph = newMemory;
			mph->pageSize = newBufferSize;
		}
		return ret;
	}

	/**
	 * @brief decreases current page to size newBufferSize
	 */
	ErrorManagement::ErrorType MemoryPage::Shrink(uint32 newBufferSize){
		ErrorManagement::ErrorType ret;

		if (mph != NULL_PTR(MemoryPageHeader *)){
			// should shrink not grow
			if (newBufferSize > mph->pageSize){
				ret.parametersError = true;
				DynamicCString errMsg;
				errMsg.AppendN("Shrink to a bigger size??:new= ");
				errMsg.AppendNum(newBufferSize);
				errMsg.AppendN(" old= ");
				errMsg.AppendNum(mph->pageSize);
				REPORT_ERROR(ret,errMsg.GetList());
			}

		}
		MemoryPageHeader *newMemory = NULL_PTR(MemoryPageHeader *);
		if (ret){
			// failure to allocate new page
			newMemory = reinterpret_cast<MemoryPageHeader*>(realloc(mph, newBufferSize + sizeof (MemoryPageHeader)));
			if (newMemory == NULL_PTR(MemoryPageHeader *)){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"realloc failed");
			}
		}

		if (ret){
			mph = newMemory;
			mph->pageSize = newBufferSize;
		}
		return ret;
	}

	/**
	 *  @brief allocate a new page
	 */
	ErrorManagement::ErrorType  MemoryPage::Allocate(uint32 size){
		ErrorManagement::ErrorType ret;
		MemoryPageHeader *newMemory;
		newMemory = reinterpret_cast<MemoryPageHeader *>(malloc(size+sizeof (MemoryPageHeader)));
//printf("malloc %i @%p\n", size,newMemory);

		if (newMemory == NULL_PTR(MemoryPageHeader *)){
			ret.outOfMemory = true;
			REPORT_ERROR(ret,"malloc failed");
		}

		if (ret){
			newMemory->pageSize = size;
			newMemory->previous = mph;
			mph = newMemory;
		}
		return ret;
	}

	// last operation before closing the use of this memory
	// change previous to next so that the memory can be parsed in the right order
	void MemoryPage::FlipOrder(){
		MemoryPageHeader *ptr = mph->previous;
		mph->previous = NULL;
		while (ptr != NULL){
			MemoryPageHeader *ptr2 = ptr->previous;
			ptr->previous = mph;
			mph = ptr;
			ptr = ptr2;
		}
	}


} //MARTe
