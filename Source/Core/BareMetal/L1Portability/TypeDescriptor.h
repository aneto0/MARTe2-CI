/**
 * @file TypeDescriptor.h
 * @brief Header file for class TypeDescriptor
 * @date 26/08/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class TypeDescriptor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPEDESCRIPTOR_H_
#define TYPEDESCRIPTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "BasicType.h"
#include "BitRange.h"
#include "BitBoolean.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A structure Used to describe the type pointed to by a pointer.
 * @details Depending on the first bit isStructuredData it may contain a code identifying a structure
 * or the remaining bits can be used to identify a specific basic type.
 *
 * Basic types are integers 8-64 bit, floats, doubles, char pointers and void pointers.
 *
 * @note The TypeDescriptor is internally represented as a 16-bit bitfield-like union with one of the following structures (depending on the type
 * described, if a basic one, or a structured one):
 * | isStructuredData   | isConstant  | type   | numberOfBits |
 * | :----:             | :----:      | :----: | :----:       |
 * |  1                 | 1           | 4      | 10           |
 *
 * | isStructuredData   | isConstant  | structuredDataIdCode  |
 * | :----:             | :----:      | :----:                |
 * |  1                 | 1           | 14                    |
 *
 */
class TypeDescriptor {
public:

    /**
     * A structure for type informations storing.
     */
    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {
        /**
         * If true then the data is a structure or class and its definition
         * has to be found in the ObjectRegistryDatabase
         */
        BitBoolean<uint16, 0u> isStructuredData;

        /**
         * The data is constant - cannot be written to
         */
        BitBoolean<uint16, 1u> isConstant;

        /**
         * The actual type of data
         */
        BitRange<uint16, 4u, 2u> type;

        /**
         * The size in bits
         */
        BitRange<uint16, 10u, 6u> numberOfBits;

        /**
         * A code related univocally to a record in the ObjectRegistryDatabase
         */
        BitRange<uint16, 14u, 2u> structuredDataIdCode;

        /**
         * Fills all the memory area.
         */
        BitRange<uint16, 16u, 0u> all;
    };

    /**
     * @brief Constructor by 16 bit integer.
     * @param[in] x contains the type informations which must be stored into this memory area.
     */
    TypeDescriptor(const uint16 x = 0u);

    /**
     * @brief Basic Type constructor.
     * @param[in] isConstantIn specifies if the type is constant.
     * @param[in] typeIn is the type.
     * @param[in] numberOfBitsIn the number of bits associated to the type.
     */
    TypeDescriptor(const bool isConstantIn,
                          const uint16 typeIn,
                          const uint16 numberOfBitsIn);

    /**
     * @brief Structured objects constructor.
     * @param[in] isConstantIn in specifies if the object is constant.
     * @param[in] structuredDataIdCodeIn is the 14 bit code associated to the object type.
     */
    TypeDescriptor(const bool isConstantIn,
                          const ClassUID &structuredDataIdCodeIn);

    /**
     * @brief Equality operator used to compare types.
     * @param[in] typeDescriptor is the type to be compared with.
     * @return In case of native types returns true if type and size fields are equal.
     * If the type is an object compares the structuredDataIdCode.
     */
    /*lint -e(1739) , operation basic_type == TypeDescriptor will not be supported*/
    bool operator==(const TypeDescriptor &typeDescriptor) const;

    /**
     * @brief Inequality operator used to compare types.
     * @param[in] typeDescriptor is the type to be compared with.
     * @return In case of native types returns true if type and size fields are equal.
     * If the type is an object compares the structuredDataIdCode.
     */
    /*lint -e(1739) , operation basic_type != TypeDescriptor will not be supported*/
    bool operator!=(const TypeDescriptor &typeDescriptor) const;


    static TypeDescriptor GetTypeDescriptorFromTypeName(const char8 * const typeName);

    static const char8 *GetTypeNameFromTypeDescriptor(const TypeDescriptor &typeDescriptor);

    static TypeDescriptor GetTypeDescriptorFromStaticTable(const uint32 index);

    static const char8 *GetTypeNameFromStaticTable(const uint32 index);

};


/**
 * 8 bit Character.
 */
static const TypeDescriptor Character8Bit(false, Character, 8u);

/**
 * 32 bit float descriptor.
 */
static const TypeDescriptor Float32Bit(false, Float, 32u);

/**
 * 64 bit float descriptor.
 */
static const TypeDescriptor Float64Bit(false, Float, 64u);

/**
 * 128 bit Float descriptor
 */
static const TypeDescriptor Float128Bit(false, Float, 128u);

/**
 * Void descriptor
 */
static const TypeDescriptor VoidType(false, SignedInteger, 0u);

/**
 * 8 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger8Bit(false, SignedInteger, 8u);

/**
 * 8 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger8Bit(false, UnsignedInteger, 8u);

/**
 * 16 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger16Bit(false, SignedInteger, 16u);

/**
 * 16 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger16Bit(false, UnsignedInteger, 16u);

/**
 * 32 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger32Bit(false, SignedInteger, 32u);

/**
 * 32 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger32Bit(false, UnsignedInteger, 32u);

/**
 * 64 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger64Bit(false, SignedInteger, 64u);

/**
 * 64 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger64Bit(false, UnsignedInteger, 64u);

/**
 * Constant char pointer descriptor
 */
static const TypeDescriptor ConstCharString(true, CCString, sizeof(const char8*) * 8u);

/**
 * Char pointer descriptor
 */
static const TypeDescriptor CharString(false, CCString, sizeof(char8*) * 8u);


static const TypeDescriptor InvalidType(0u);
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* TYPEDESCRIPTOR_H_ */