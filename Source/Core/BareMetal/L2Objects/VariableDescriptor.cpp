/**
 * @file VariableDescriptor.cpp
 * @brief Source file for class VariableDescriptor
 * @date Nov 29, 2016
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
 * the class VariableDescriptor (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "VariableDescriptor.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"
#include "DynamicCString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

static char8 toupper(char8 c){
    if ((c >='a') && (c <='z')) {
        c = c - ('a'-'A');
    }
    return c;
}
/*
static char8 tolower(char8 c){
    if ((c >='A') && (c <='Z')) {
        c = c + ('a'-'A');
    }
    return c;
}
*/
#if 0
uint32 FromHex(char8 c){
	uint32 ret = 0u;
	if ((c >='0') && (c <='9')){
		ret  = c - '0';
	} else
		if ((c >='A') && (c <='F')){
			ret  = c - 'A' + 10u;
		}
	return ret;
}

static uint32 ReadHex(char8 *&buffer,uint32 nOfChars){
	uint32 i=0u;

	uint32 ret = 0u;
	for (i=0u;i<nOfChars;i++){
		uint32 n = FromHex(*buffer++);
		ret+= n << (4u*i);
	}
	return ret;
}



/**
 * returns tokens A=array P=pointer V=Vector M=Matrix
 * returns size as size of Arrays
 * buffer is input and is modified
 */
static void BrowseModifierBuffer(char8 &token, bool &constant, uint32 &size,char8  *&buffer){
    token = '\0';
    size = 0u;
    constant = false;
    if (buffer != NULL){
        char8 c = *buffer++;
        constant = (c >= 'a');
        c = toupper(c);
        token = c ;
        switch (c){
        case '\0':{
        	buffer--;
        	token = '\0';
        	size = 0u;
        } break;
        case 'A':{
            token = 'A' ;
            size = ReadHex(buffer,2);
        } break;
        case 'B':{
            token = 'A' ;
            size = ReadHex(buffer,4);
        } break;
        case 'C':{
            token = 'A' ;
            size = ReadHex(buffer,8);

        } break;
        }
    }
}

void VariableDescriptor::GetModifier(char8 &token, bool &constant, uint32 &size,uint32 depth) const {
    char8 *buffer = modifiers.GetList();
    depth++;
    do {
        BrowseModifierBuffer(token, constant, size,buffer);
        depth--;
    } while ((token != '\0') && (depth > 0u));
}


void VariableDescriptor::AddGenericToModifiers(char8 token,bool &constant){
    if (constant){
        constant = false;
        modifiers.Append(tolower(token));
    } else {
        modifiers.Append(token);
    }
}

static char8 ToHex(uint32 nibble){
	char8 ret = '?';
	if (nibble < 10u){
		ret = '0'+nibble;
	} else
		if (nibble < 16u){
			ret = 'A' + nibble - 10u;
		}
	return ret;
}

static void AppendHex(DynamicCString &s, uint32 n, uint32 size){
	uint32 i;
	for (i=0u;i<size;i++){
		uint32 nibble = n & 0x0000000F;
		n >>=4;
		s.Append(ToHex(nibble));
	}
}

void VariableDescriptor::AddArrayToModifiers(uint32 vectorSize,bool &constant){
    char8 c = '\0';
    if (constant){
        constant = false;
        c = ('a' - 'A');
    }
    if (vectorSize < 256u) {
        c += 'A';
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,2);
    } else
    if (vectorSize < 65536u) {
        c += 'B';
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,4);
    } else
    {
        c += 'C';
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,8);
    }
}

void StoreCode(const ModifierCode &code){
	switch (code.)

}


void VariableDescriptor::QueueCode(ModifierCode &code){
	if (code.HasCode()) {
		StoreCode(mf);
		code.Clear();
	}
}
#endif

// syntax Annn where nn is numeric only
static void AddArrayDataToModifiers(DynamicCString &modifiers, uint32 size){
	char8 buffer[12];
	char8 *pBuffer = &buffer[11];
	*pBuffer-- = '\0';
	while (size > 0){
		uint32 n = size % 10;
		size = size / 10;
		*pBuffer-- = '0'+ n;
	}
	pBuffer++;
	modifiers.AppendN(pBuffer);
}

void VariableDescriptor::MoveCodeToModifiers(){
	bool isConstant = typeDescriptor.isConstant;
	switch(typeDescriptor.arrayType){
	case ArrayUnknown:{
		if (isConstant) modifiers.Append('C');
		typeDescriptor.all = 0;
	} break;
	case Array1D:{
		volatile uint32 currentSize = typeDescriptor.arraySize;

		if (currentSize == 0){
			modifiers.Append('V');
			typeDescriptor.all = 0;
			typeDescriptor.isConstant = isConstant;
		} else
     	if (currentSize > 1){
			modifiers.Append('A');
			AddArrayDataToModifiers(modifiers,currentSize);
			typeDescriptor.all = 0;
			typeDescriptor.isConstant = isConstant;
		}
	}break;
	case Array2D:{
		modifiers.Append('M');
		typeDescriptor.all = 0;
		typeDescriptor.isConstant = isConstant;
	}break;
	case ZeroTermArray:{
		if (typeDescriptor.isConstant){
			modifiers.Append('z');
		} else {
			modifiers.Append('Z');
		}
		typeDescriptor.all = 0;
	}break;
	case DynamicZeroTermArray:{
		if (typeDescriptor.isConstant){
			modifiers.Append('d');
		} else {
			modifiers.Append('D');
		}
		typeDescriptor.all = 0;
	}break;
	case StaticZeroTermArray:{
		if (typeDescriptor.isConstant){
			modifiers.Append('s');
		} else {
			modifiers.Append('S');
		}
		AddArrayDataToModifiers(modifiers,typeDescriptor.arraySize);
		typeDescriptor.all = 0;
	}break;
	case PointerArray:{
		if (typeDescriptor.isConstant){
			modifiers.Append('p');
		} else {
			modifiers.Append('P');
		}
		typeDescriptor.all = 0;
	}break;
	default:{

	}break;
	}

}

void VariableDescriptor::AddArrayCode(BasicArrayType bat, uint32 size1){
	switch(bat){
	case Array1D:{
		//tries to store the array in the TypeDescriptor  either by creating a Array1D or an Array2D
		//failing outputs current state to Modifiers
		MoveCodeToModifiers();
		if (typeDescriptor.arrayType == ArrayUnknown){
			if (size1 < 0xFFFFFu){
				typeDescriptor.arrayType = Array1D;
				typeDescriptor.arraySize = size1;
				typeDescriptor.isConstant = false;
			} else {
				modifiers.Append('A');
				AddArrayDataToModifiers(modifiers,size1);
			}
		}
	}break;
	// only Matrix<T> case supported -- assumes size1 = 0
	case Array2D:{
		MoveCodeToModifiers();
		typeDescriptor.arrayType = Array2D;
		typeDescriptor.arraySize = 0;
		typeDescriptor.isConstant = false;
	}break;
	case ZeroTermArray:{
		MoveCodeToModifiers();
		typeDescriptor.arrayType = ZeroTermArray;
		typeDescriptor.isConstant = false;
	}break;
	case DynamicZeroTermArray:{
		MoveCodeToModifiers();
		typeDescriptor.arrayType = DynamicZeroTermArray;
		typeDescriptor.isConstant = false;
	}break;
	case StaticZeroTermArray:{
		MoveCodeToModifiers();
		if (size1 < 0xFFFFFu){
			typeDescriptor.arrayType = StaticZeroTermArray;
			typeDescriptor.arraySize = size1;
			typeDescriptor.isConstant = false;
		} else {
			modifiers.Append('S');
			AddArrayDataToModifiers(modifiers,size1);
		}
	}break;
	case PointerArray:{
		MoveCodeToModifiers();
		typeDescriptor.arrayType = PointerArray;
		typeDescriptor.isConstant = false;
	}break;
	default:{

	}break;
	}
}

void VariableDescriptor::AddConstantCode(){
	typeDescriptor.isConstant = true;
}

void VariableDescriptor::FinaliseCode(TypeDescriptor td){
	bool constant = typeDescriptor.isConstant;
	if (typeDescriptor.arrayType == ArrayUnknown) {
		typeDescriptor = td;
	} else
	if (td.isStructuredData){
		MoveCodeToModifiers();
		typeDescriptor = td;
	} else
	// can only merge scalar to vector definitions
	if ((td.arrayType != Array1D) || (td.arraySize != 1)){
		MoveCodeToModifiers();
		typeDescriptor = td;
	} else {
		BasicType bt = td.type;
		switch (bt){
		// basic types supporting any modifier
		case SignedInteger:
		case UnsignedInteger:
		case Float:
		case Char:
		{
			uint32 tmp = td.objectSize;
			typeDescriptor.objectSize = tmp;
//			typeDescriptor.objectSize = td.objectSize;
			tmp = td.type;
			typeDescriptor.type = tmp;
		} break;
		case Void:
		{
			typeDescriptor.type = td.type;
		} break;
		// not supporting modifiers
		case SignedBitInteger:
		case UnsignedBitInteger:{
			MoveCodeToModifiers();
			typeDescriptor= td;
		}break;
		/// do not know how to handle yet
		case SString:
		case Stream:
		case StructuredDataInterface:
		case Invalid:
		default:{
			MoveCodeToModifiers();
			typeDescriptor= td;
			} break;
		}
	}
	typeDescriptor.isConstant = typeDescriptor.isConstant | constant;
}
/*
static bool isToken(char8 c){
	return (((c >='A') && (c <='Z')) || ((c >='a') && (c <='z')));
}
*/
static bool isConstantToken(char8 c){
	return ((c >='a') && (c <='z'));
}
static bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}
static uint32 toNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}
static uint32 readNumber(char8 *&buffer){
	uint32 result = 0;
	while (isNumber(*buffer)){
		result = result * 10u;
		result += toNumber(*buffer);
		buffer++;
	}
	return result;
}

void VariableDescriptor::GetModifier(char8 &token, bool &constant, uint32 &size,uint32 depth) const {
    char8 *buffer = modifiers.GetList();

    depth++;
    do {
    	token = *buffer;
    	constant = isConstantToken(token);
    	token = toupper(token);
    	buffer++;
    	size = readNumber(buffer);

    	depth--;
    } while ((token != '\0') && (depth > 0u));
}



VariableDescriptor::VariableDescriptor(){
    typeDescriptor = InvalidType;
}

VariableDescriptor::~VariableDescriptor(){
}

VariableDescriptor::VariableDescriptor(const TypeDescriptor &td){
    typeDescriptor = td;
}

VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.AppendN(x.modifiers.GetList());
}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.AppendN(x.modifiers.GetList());

}

const TypeDescriptor &VariableDescriptor::GetFullTypeDescriptor() const {
    return typeDescriptor;
}


}


	
