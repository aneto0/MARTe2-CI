/**
 * @file BasicType.h
 * @brief Header file for class BasicType
 * @date 28/08/2015
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

 * @details This header file contains the declaration of the class BasicType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICTYPE_H_
#define BASICTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "FractionalInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Definition of BasicType as a 4 bit integer.
 */
typedef uint4 BasicType;

/**
 * @brief The type is a signed integer.
 */
const BasicType SignedInteger = 0u;

/**
 * @brief The type is an unsigned integer.
 */
const BasicType UnsignedInteger = 1u;

/**
 * @brief The type is a float number.
 */
const BasicType Float = 2u;

/**
 * @brief The type is a char .
 */
const BasicType Char = 3u;

/**
 * @brief The type is a pointer. A void*. No information on the type pointed to
 */
const BasicType Pointer = 4u;

/**
 * @brief The type is a signed bit range .
 */
const BasicType SignedBitInteger = 5u;

/**
 * @brief The type is an unsigned bit range.
 */
const BasicType UnsignedBitInteger = 6u;



/**
 * @brief The type is a C string. Cannot be extended. May or may not be modifiable.
 * (const CCString only ???)
 */
//const BasicType BT_CCString = 4u;

/**
 * The type is a pointer to a C string allocated on heap.
 * (char **)
 */
//const BasicType PCString = 5u;

/**
 * @brief The type is an array of chars with the size specified in the size field.
 */
//const BasicType CArray = 6u;

/**
 * @brief The type is a StreamString class.
 */
const BasicType SString = 7u;

/**
 * @brief The type is a StreamInterface class.
 */
const BasicType Stream = 8u;

/**
 * @brief  The StructuredDataI type
 */
const BasicType StructuredDataInterface = 13u;

/**
 * @brief The type is unspecified -dereferencing a void *
 */
const BasicType Void = 14u;


/**
 * @brief The type is not valid
 */
const BasicType Invalid = 15u;


}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTYPE_H_ */
