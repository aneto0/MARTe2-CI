/**
 * @file SafeMath.h
 * @brief Header file for class AnyType
 * @date Apr 14, 2020
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

#ifndef SAFEMATH_H_
#define SAFEMATH_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "FastMath.h"
#include "TypeCharacteristics.h"
#include "ErrorManagement.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace SafeMath{


/**
 *  calculates the sign bit for each type
 *  compatible only for integer types
 */
template <typename T>
const T SignBit(){
	return  0;
}

template <> const int8 SignBit<int8>(){
	return 0x80U;
}

template <> const int16 SignBit<int16>(){
	return 0x8000U;
}

template <> const int32 SignBit<int32>(){
	return 0x80000000U;
}

template <> const int64 SignBit<int64>(){
	return 0x8000000000000000ULL;
}

template <typename T>
const T AllOnes(){
	return  0;
}

template <> const int8 AllOnes<int8>(){
	return 0xFFU;
}

template <> const int16 AllOnes<int16>(){
	return 0xFFFFU;
}

template <> const int32 AllOnes<int32>(){
	return 0xFFFFFFFFU;
}

template <> const int64 AllOnes<int64>(){
	return 0xFFFFFFFFFFFFFFFFULL;
}


template <typename T>
ErrorManagement::ErrorType Addition(T a1, T a2, T &sum){
	ErrorManagement::ErrorType ret;

	sum = static_cast<T>(a1 + a2);
	// check saturation
	if (TypeCharacteristics<T>::IsSigned()){
		// for signed numbers overflows happens on the sign bit:
		// this means only happens if signs were the same and if result has flipped sign
//		const T signBit = static_cast<T>(static_cast<T>(1u) << (sizeof(T)*8u-1u));
		const T signBit = SignBit<T>();
		if ( (((a1 ^ a2) & signBit) == 0u) && (((a1 ^ sum) & signBit) != 0u)){
			if ((a1 & signBit) == 0u){
				ret = ErrorManagement::Overflow;
			} else {
				ret = ErrorManagement::Underflow;
			}
		}
		// for unsigned the result is smaller than one of them
	} else {
		if ((sum < a1 ) && (sum < a2)){
			ret = ErrorManagement::Overflow;
		}
	}

	return ret;
}


template <typename T>
ErrorManagement::ErrorType Subtraction(T a1, T a2, T &sum){
	ErrorManagement::ErrorType ret;

	sum = static_cast<T>(a1 - a2);
	// check saturation
	if (TypeCharacteristics<T>::IsSigned()){
		// for signed numbers overflows happens on the sign bit:
		// this means only happens if signs were the same and if result has flipped sign
//		const T signBit = static_cast<T>(static_cast<T>(1u) << (sizeof(T)*8u-1u));
		const T signBit = SignBit<T>();
		if ( (((a1 ^ a2) & signBit) != 0u) && (((a1 ^ sum) & signBit) != 0u)){
			if ((a1 & signBit) == 0u){
				ret = ErrorManagement::Overflow;
			} else {
				ret = ErrorManagement::Underflow;
			}
		}
		// for unsigned the result is smaller than one of them
	} else {
		if (a2 > a1){
			ret = ErrorManagement::Underflow;
		}
	}

	return ret;
}

template <typename T>
ErrorManagement::ErrorType Multiplication(T a1, T a2, T &product){
	ErrorManagement::ErrorType ret;

	T overflow;
	product = FastMath::CompleteMultiply(a1,a2,overflow);

	//check overflow for signed numbers
	if (TypeCharacteristics<T>::IsSigned()){
		               /* positive ovf */
		ret.overflow  =  (overflow > 0)            ||
					   /* change in sign --> positive overflow into the sign bit of product*/
				        ((overflow == 0) && (product < 0));
					   /* negative ovf - allones() would be sign extension */
		ret.underflow =  (overflow < AllOnes<T>()) ||
					   /* product should be negative - overflow into sign bit of product */
				        ((overflow == AllOnes<T>()) && (product >= 0));
	} else {
        /* positive ovf */
		ret.overflow  =  (overflow != 0);
	}

	return ret;
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // SafeMath

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_SAFEMATH_H_ */