/**
 * @file VDPrivate.cpp
 * @brief Header file for class AnyType
 * @date 8 Feb 2018
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

#include "VDPrivate.h"
#include "Matrix.h"
#include "TypeDescriptor.h"
#include "ZeroTerminatedArray.h"

namespace MARTe{

static inline bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

static inline uint32 toNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}

/** string to integer */
static inline uint32 readNumber(CCString &buffer){
	uint32 result = 0;
	while (isNumber(buffer[0])){
		result = result * 10u;
		result += toNumber(buffer[0]);
		buffer++;
	}
	return result;
}

/**
 * extracts information about a layer
 * updates the layer string pointer
 * used 8 times
 */
static inline void GetLayerInfo(CCString &modifierString,char8 &modifier,uint32 &size ){
	if (modifierString.IsNullPtr()){
		modifier = '\0';
		size = 0;
	} else {
		modifier = modifierString[0];
		if (modifier == '\0'){
			size = 0;
		} else {
			modifierString++;
			size = readNumber(modifierString);
		}
	}
}

static inline bool IsConst(char8 c){
	bool ret = true;
	if ((c >= 'A')  && (c <= 'Z')) {
		ret = false;
	}
	return ret;
}



//#include <stdio.h>
/**
 *
 */
DimensionHandler::DimensionHandler(CCString modifiers,TypeDescriptor tdIn){
	td = tdIn;
//printf("%i %i\n",dimensions.GetCapacity(),dimensions.GetMaxCapacity());
//printf( "[%s]\n ",modifiers.GetList());//TODO
	const CCString terminals = "VMvm";
	const CCString Zterminals = "ZDSzds";
	char8 modifier;
	uint32 size;
	GetLayerInfo(modifiers,modifier,size);
	char8 pointer = '\0';
	bool finished = false;
	while (!finished){
		if (modifier == '\0'){
			if (pointer != '\0') {
				td = PointerType;
				if (IsConst(pointer)){
					td.dataIsConstant = true;
				}
			}
			dimensions.Add(DimensionInfoElement('\0',1));
//printf("{0}\n");
			finished = true;
		} else
		if (modifier == 'A'){
			if (pointer == 'P'){
				dimensions.Add(DimensionInfoElement('F',size));
			} else
			if (pointer == 'p'){
				dimensions.Add(DimensionInfoElement('f',size));
			} else {
				dimensions.Add(DimensionInfoElement('A',size));
//printf("{A}\n");
			}
			pointer = '\0';
		} else {
			if ((modifier == 'P') || (modifier == 'p')){
				// PP double indirection is a terminal case
				if (pointer != '\0') {
					td = PointerType;
					if (IsConst(pointer)){
						td.dataIsConstant = true;
					}
					dimensions.Add(DimensionInfoElement('\0',1));
					finished = true;
				} else {
					pointer = modifier;
				}
			} else
			if (terminals.In(modifier)){
				// PV PM double indirection is a terminal case
				if (pointer != '\0') {
					td = PointerType;
					dimensions.Add(DimensionInfoElement('\0',1));
					finished = true;
				} else {
//printf("{%c}\n",modifier);
					dimensions.Add(DimensionInfoElement(modifier,SaturatedInteger<uint32>::Indeterminate()));
				}
			} else
			if (Zterminals.In(modifier)){
				// only PZ PD PS are allowed
				if (pointer != '\0') {
					pointer = '\0';
					dimensions.Add(DimensionInfoElement(modifier,SaturatedInteger<uint32>::Indeterminate()));
				} else {
					td = InvalidType(0);
				}
			}

		}
		if (!finished){
			GetLayerInfo(modifiers,modifier,size);
		}
	}
//printf ("dimensions = %i\n ",dimensions.GetSize());

//printf("%i %i\n",dimensions.GetCapacity(),dimensions.GetMaxCapacity());

	DimensionSize elementSize = 1;
	char8 layerEnd = '\0';
	for (int i = (dimensions.GetSize()-1); i >= 0; i--){
		if (dimensions[i].type != 'A'){
			layerEnd = dimensions[i].type;
			elementSize = Type2Size(dimensions[i].type);
		} else {
			elementSize = elementSize * dimensions[i].numberOfElements ;
		}
		dimensions.Access(i).layerEnd = layerEnd;
		dimensions.Access(i).elementSize = elementSize;

//char c = (dimensions[i].type=='\0')?'0':dimensions[i].type; //TODO
//char d = (dimensions[i].layerEnd=='\0')?'0':dimensions[i].layerEnd;//TODO
//printf( "[%i>%c %i %i %c]\n ",i,c,dimensions.Access(i).numberOfElements,dimensions.Access(i).elementSize,d);
	}
}

void DimensionHandler::GetOutputModifiers(DynamicCString &dc) const {
	ErrorManagement::ErrorType ret;

	for (int i = 0; i < NDimensions();i++){
		switch (dimensions[i].outputType){
		case '\0':{
		} break;
		case 'A':{
			dc.Append('A');
			uint32 size = dimensions[i].numberOfElements.GetData();
			dc.Append(size);
		}break;
		case 'f':{
			dc.Append("pA");
			uint32 size = dimensions[i].numberOfElements.GetData();
			dc.Append(size);
		}break;
		case 'F':{
			dc.Append("PA");
			uint32 size = dimensions[i].numberOfElements.GetData();
			dc.Append(size);
		}break;
		default:{
			dc.Append(dimensions[i].outputType);
		}
		};
	}
}

ErrorManagement::ErrorType  DimensionHandler::HasSameDimensionsAs(const DimensionHandler &other) const{
	ErrorManagement::ErrorType ok = true;
	uint32 nDim = NDimensions();
	uint32 nDimO = other.NDimensions();
	if (nDim == 0){
		ok.internalSetupError=true;
        REPORT_ERROR(ok, "NDimensions == 0");
	}
	if (nDim != nDimO){
		ok.invalidOperation=true;
        REPORT_ERROR(ok, "different number of dimensions");
		DynamicCString errM;
		errM.Append("N of dim[");
		for (int i = 0;i < nDim;i++){
			char8 type = dimensions[i].type;
			if (type != '\0') errM.Append(type);
			else errM.Append('0');
		}
		errM.Append( "]= ");
		errM.Append(nDimO);
		errM.Append(" != [");
		for (int i = 0;i < nDim;i++){
			char8 type = other.dimensions[i].type;
			if (type != '\0') errM.Append(type);
			else errM.Append('0');
		}
		errM.Append(']');
		errM.Append(nDimO);
        REPORT_ERROR(ok, errM.GetList());
	}

	for (int i = 0;ok && (i < nDim); i++){
		DimensionSize d1 = dimensions[i].numberOfElements;
		DimensionSize d2 = other.dimensions[i].numberOfElements;

		if (((d1 != d2) && ( !d1.isIndeterminate()) && ( !d2.isIndeterminate())) || ( d1.isPositiveInf())){
			ok.invalidOperation=true;
			DynamicCString errM;
			errM.Append("dimension[");
			errM.Append(i);
			errM.Append("] d1= ");
			errM.Append(d1.GetData());
			errM.Append(" d2= ");
			errM.Append(d2.GetData());
	        REPORT_ERROR(ok, errM.GetList());
		}
	}

	return ok;
}

uint32 DimensionHandler::Type2Size(char8 c) const{
	const CCString matrixS = "Mm";
	const CCString vectorS = "Vv";
	const CCString pointerS = "FfSDZPsdzp";
	uint32 size = 0;
	if (pointerS.In(c)){
		size = sizeof(void *);
	} else
	if (matrixS.In(c)){
		size = sizeof(Matrix<char8>);
	} else
	if (vectorS.In(c)){
		size = sizeof(Vector<char8>);
	} else {
		size = td.StorageSize();
	}
	return size;
}


#include <stdio.h>
ErrorManagement::ErrorType DimensionHandler::UpdatePointerAndSize(
			uint32 			layerIndex,
			const uint8 *&	ptr,
			uint32 &		numberOfElementsIO,
			uint32 &		nextElementSize,
			uint32 &		overHead)  {
	ErrorManagement::ErrorType ok;

	const DimensionInfoElement &diNext = dimensions[layerIndex+1];
	DimensionInfoElement &di = dimensions.Access(layerIndex);

	uint8 type = di.type;
	bool isZta = false;
	bool allowNULL = false;
	bool updateNumberOfElements = false;
	overHead = 0;

	DimensionSize numberOfElements(0);
	DimensionSize nextElementSizeD(0);

	nextElementSizeD = diNext.elementSize;

	switch (type){

	case '\0':{
		numberOfElements = 1;
		type = 'A';
		nextElementSizeD = di.elementSize;
//printf ("elSz = %i nEl = %i\n",elementSize.toUint32Unchecked(),numberOfElements.toUint32Unchecked());

	} break;
	case 'x': // F or f with NULLs - created at this level
	case 'f':
	case 'F':{
		overHead = sizeof (void *);
		numberOfElements = di.numberOfElements;
		allowNULL = true;
	} break;
	case 'A':{
		numberOfElements = di.numberOfElements;
//printf("[ANEL=%i]",numberOfElements);
	}break;
	case 'd':
	case 's':
	case 'z':
	case 'D':
	case 'S':
	case 'Z':{
		isZta = true;
		updateNumberOfElements = true;
		overHead = sizeof (void *) + diNext.elementSize.GetData();
	}break;
	case 'v':
	case 'V':{
		overHead = sizeof (Vector<char8>);
		const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(ptr);
		numberOfElements = pv->GetNumberOfElements();
		if (numberOfElements == DimensionSize(0U)){
			allowNULL = true;
		}
		updateNumberOfElements = true;
//printf("[VNEL=%i]",numberOfElements);
	}break;
	case 'm':
	case 'M':{
		overHead = sizeof (Matrix<char8>);
		const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);
		numberOfElements = pm->GetNumberOfElements();
		if (numberOfElements == DimensionSize(0U)){
			allowNULL = true;
		}
		updateNumberOfElements = true;
	}break;
	default:{
		ok.fatalError = true;
        REPORT_ERROR(ok, "Default case reached");
	}break;

	}

	if ((type != 'A') && ok){
		// it works as vector is descendant of Pointer class
//printf("X p1=%p ",ptr);
		ok = RedirectP(ptr,allowNULL);
//printf(" p2=%p \n",ptr);
		if (!ok){
			DynamicCString errM;
			errM.Append("type ");
			errM.Append(type);
			errM.Append(" redirection failed ");
	        REPORT_ERROR(ok, errM.GetList());
		}
		// handle case of null pointers
		// change F to X type
		if (ptr == NULL) {
			numberOfElements = 0;
			if ((type == 'f') || (type == 'F')){
				di.type = 'x';
			}
		}
	}
//printf("els=%lli \n",elementSize);

	// calculate actual size of each ZTA
	if (isZta && ok){
		uint32 n;
		ok = diNext.elementSize.ToNumber(n);
	    CONDITIONAL_REPORT_ERROR(ok, "diNext.elementSize is infinite/indefinite");
		if (ok){
			numberOfElements = ZeroTerminatedArrayGetSize(ptr, n);
		}
	}

#if 0
//printf ("2elSz = %i nEl = %i\n",elementSize.toUint32Unchecked(),numberOfElements.toUint32Unchecked());
	if (updateNumberOfElements && ok){
		if (di.numberOfElements.isNotANumber()){
			if (di.numberOfElements.isIndeterminateSize()){
				di.numberOfElements = numberOfElements;
			}
		} else
		if (di.numberOfElements != numberOfElements){
			di.numberOfElements = variableSize;
		}
	}
#endif

	if (ok){
		ok = numberOfElements.ToNumber(numberOfElementsIO);
	    CONDITIONAL_REPORT_ERROR(ok, "numberOfElements is infinite/indefinite");
	}
	if (ok){
		ok = nextElementSizeD.ToNumber(nextElementSize);
	    CONDITIONAL_REPORT_ERROR(ok, "elementSize is infinite/indefinite");
	}
//printf ("3elSz = %i nEl = %i\n",elementSize.toUint32Unchecked(),numberOfElements.toUint32Unchecked());

	return ok;
}




} //MARTe