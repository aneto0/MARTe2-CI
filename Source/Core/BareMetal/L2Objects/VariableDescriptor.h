/**
 * @file VariableDescriptor.h
 * @brief Header file for class VariableDescriptor
 * @date Nov 23, 2016
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

 * @details This header file contains the declaration of the class VariableDescriptor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2OBJECTS_VARIABLEDESCRIPTOR_H_
#define L2OBJECTS_VARIABLEDESCRIPTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "ClassRegistryDatabase.h"
#include "FractionalInteger.h"
#include "Matrix.h"
#include "StringHelper.h"
#include "ZeroTerminatedArray.h"
#include "DynamicZeroTerminatedArray.h"
#include "StaticZeroTerminatedArray.h"

namespace MARTe {
class Object;
class DynamicCString;
class StreamString;
class StructuredDataI;
class testStruct;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief description of a dimension.
 */
struct DimensionInfo{
	/**
	 * @brief number of elements in this dimension (or nothing if pointer is invalid).
	 */
	uint32 numberOfElements;
	/**
	 * @brief A for arrays P for pointers to arays p for const pointers to arrays
	 */
	char8  type;
};

/**
 * @brief full description of the type of a variable including modifiers
 */
class DLL_API VariableDescriptor{

public:

    /**
     * default constructor
     * */
    VariableDescriptor();

    /**
     * destructor
     * */
    ~VariableDescriptor();

    /**
     * @brief main constructor by learning
     */
    template <class T>
    inline  VariableDescriptor( T  x);

    /**
     * @brief copy
     * Note that const VariableDescription on its own will not work as it will not match univocally a non cost  VariableDescriptor
     */
    VariableDescriptor(VariableDescriptor &x ) ;

    /**
     * @brief copy constructor
     */
    VariableDescriptor(const VariableDescriptor &x );

    /**
     * @brief copy operator
     */
    VariableDescriptor &operator=(const VariableDescriptor &x );

    /**
     * @brief sets the typeDescriptor
     */
    VariableDescriptor(const TypeDescriptor &td);

    /**
     * @brief returns size of all the memory addressed by this variable.
     * @param[in] pointer, the pointer to the variable
     * @param[in] maxDepth  determine the number of indirected memory to include.
     * limit 0 means just the top layer
     * Note that pointers to type only address one element
     * Vector, Matrix, ZeroTermArray are all supported
     * @param[out] dataSize is the full size of the memory necessary to store this var: auxilliary storages + actual data
     * @param[out] overHeadSize is the memory used as overhead: pointers etc
     * @return true if all ok
     */
    ErrorManagement::ErrorType GetSize(const uint8 *pointer,uint64 &dataSize, uint64 *overHeadSize=NULL,uint8 maxDepth=10) const;

    /**
     * @brief removes one indirection layer and update variable pointer
     * @param[in,out] pointer, the pointer to the variable
     * @param[in] index the offset
     * @param[in] modifierString, used internally to perform recursion
     * @return true if all ok or the error
     */
    ErrorManagement::ErrorType Redirect(const uint8 *&pointer,uint32 index,CCString modifierString=emptyString) ;

    /**
     * @brief copies the variable layer by layer. The copied layer is implemented in contiguous memory
     * @param[in] sourcePtr, the pointer to the variable to be copied
     * @param[in,out] destFreePtr, pointer to the memory area to copy the variable to. Returns the pointer to the unused area
     * @param[in,out] destFreeSize, length of the memory area to copy to. Returns the unused area size
     * @param[out] destVd, the variable descriptor of the used area, must be empty to start!. Note that all varieties of ZeroTermarrays become ZeroTermArray<const T>
     * @param[in] maxDepth, the max number of pointer redirection to include in the copy
     * @param[in] modifierString, at start points at full modifiers.After each recursion it is progressively consumed.
     * @return true if all ok or the error
     */
    ErrorManagement::ErrorType Copy(
    		const uint8 *sourcePtr,
    		uint8 *&destFreePtr,
			uint64 &destFreeSize,
			VariableDescriptor &destVd,
			uint8 maxDepth=100,
			CCString modifierString = emptyString) const;

    ErrorManagement::ErrorType VariableDescriptor::CopyTo(
    		const uint8 *sourcePtr,
    			  uint8 *destPtr,
    		VariableDescriptor destVd
    		) const ;

    /**
     * @brief Converts type descriptor to C/c++ equivalent
     * @return true if all ok
     */
    ErrorManagement::ErrorType  ToString(DynamicCString &string,bool rawFormat=false) const;

    /**
     * @brief Converts C/c++ string to type descriptor
     * @return true if all ok
     */
    ErrorManagement::ErrorType  FromString(DynamicCString &string);

    /**
     * @brief provides a typeDescriptor for the overall variable.
     * @details only simple variables are detailed here.
     * Including all variants of CStrings that are detailed as CCString.
     * T*,Vector<>,Array<> are all seen as GenericPointers
     * @return the summary type descriptor
     */
    TypeDescriptor GetSummaryTypeDescriptor() const;

    /**
     * @brief obtains information about multidimensional arrays
     * @details only handles arrays and pointers to array. Vector and Matrix or ZTA terminate the scan
     * @param[out] dimensions will be cleaned and reallocated to contain the list of dimensions.
     * Each dimension can be positive, negative.
     * Positive refers to array dimensions Ann
     * Negative refers to pointer to arrays dimensions PAnn
     * @return the type descriptor of the array element. It uses GetSummaryTypeDescriptor()
     */
    TypeDescriptor GetDimensionsInformation(DynamicZeroTerminatedArray<DimensionInfo,4> &dimensions) const;
private:
    /**
     *  @brief a zero terminated sequence of tokens.
     *  @full each token can be a character or a sequence of characters and bytes
    */
    DynamicCString      modifiers;

    /**
     * @brief The type of the (final after redirections) variable
    */
    TypeDescriptor     typeDescriptor;

/*
 *  PRIVATE METHODS
 */

    /**
     * @brief adds one layer of modifiers to the top
     * @return true if operation succeeded                       */
    void AddModifiersLayerConst(char8 modifier, uint64 size);

    /**
     * @brief Converts type descriptor to C/c++ equivalent
     *
     */
    ErrorManagement::ErrorType  ToStringPrivate(DynamicCString &string,CCString modifierString,bool start,int8 &priority) const;

    /**
     * @brief calculate size of a full layer - n of array alements * elementsize
     * @param[in] modifierString, the string of variable modifiers
     * @return the full size of the memory necessary to store this layer
     */
    uint64 FullLayerSize(CCString modifierString,const uint8 *pointer) const;

    /**
     * @brief calculate properties of a full layer - n of array alements * elementsize
     * @details the layers start with a fixed/variable modifier and groups all following fixed modifiers
     * @details this does not calculate size. Only used to speedup operations
     * @param[in,out] modifierString, the string of variable modifiers
     * @param[out] numberOfElements is the number of elements in this layer
     * @param[out] elementSize is the storage space occupied by an element - not the data it contains.
     * @param[out] overheadSize is the size not used for data content
     * @param[out] arrayStringSize number of characters in modifierString consumed in arrays 'a' layers
     * @param[out] numberOfTermElements number of elements of size elementSize used as terminator in a ZTA
     * @param[out] modifier is the last code that terminated this scan (0 for the end of modifiers or one of PVNZSDpvnzsd
     */
    ErrorManagement::ErrorType FullLayerInfo(CCString &modifierString,const uint8 *pointer,
    		uint64 &numberOfElements,uint32 &elementSize,
			uint32 &arrayStringSize,uint32 &numberOfTermElements,
			char8 &modifier) const;

    /**
     * @brief returns size of all the memory addressed by this variable.
     * @param[in] modifierString, the string of variable modifiers
     * @param[in] pointer, the pointer to the variable
     * @param[out] dataSize is the full size of the memory necessary to store this var including redirections
     * @param[out] overHeadSz is the memory used in intermediate redirection layers
     * @param[in] maxDepth  determine the number of indirected memory to include.
     * -1 means no limit 0 means just the top layer
     * Note that pointers to type only address one element
     * Vector, Matrix, ZeroTermArray are all supported
     * @param[in] layerMultiplier >1 it is an array[layerMultiplier] of the type described by modifierString
     * @return true if all ok
     */
    ErrorManagement::ErrorType GetDeepSize(CCString modifierString, const uint8 *pointer,
    		uint64 &dataSize, uint64 &overHeadSz,uint8 maxDepth=100,uint32 layerMultiplier=1) const;

    /**
     *  @brief To encode the fact that what is coming next is a constant
     */
    inline void AddConstantCode();

    /**
     * @brief to encode the actual data type. It will incorporate the const and Array code
     */
    void FinaliseCode(TypeDescriptor td);

    // GENERAL MATCHES
    // called by the main constructor

    /**
     * @brief Matches a Matrix.
     * @tparam T the type of the elements in the matrix
     * @param[in] mat the matrix from whose this Match will be constructed.
     * @post Adds a M to the modifiers
     */
    template<typename T>
    void Match(Matrix<T> * mat);

    /**
     * @brief Matches a Vector
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T>
    void Match(Vector<T> * vec);

    /**
     * @brief Matches a static zero terminated array
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T>
    void Match(ZeroTerminatedArray<T> * vec);

    /**
     * @brief Matches a dynamic zero terminated array
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T >
    void Match(DynamicZeroTerminatedArray<T,16u> * vec);

    /**
     * @brief Matches a zero terminated array statically allocated T[N]
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this Match will be constructed.
     * @post Adds a V to the modifiers
     */
    template<typename T,uint32 sz >
    void Match(StaticZeroTerminatedArray<T,sz> * vec);

    /**
     * @brief Matches a T* const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T * const * x);

    /**
     * @brief Matches a T[n]
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a [n] to the modifiers
     */
    template <class T,unsigned int n>
    inline void Match(T (*x) [n]);

    /**
     * @brief Matches a T[n]
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a [n] to the modifiers
     */
    template <class T,unsigned int n>
    inline void Match(const T (*x) [n]);

    /**
     * @brief Matches a T*
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T ** x);


    /**
     * @brief Matches a T const *
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a * to the modifiers
     */
    template <class T>
    inline void Match(T const * * x);

    /**
     * @brief Matches a T const * const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * const * x);

    /**
     * @brief Matches a T const
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post Adds a C to the modifiers
     */
    template <class T>
    inline void Match(T const * x);


    // GENERIC FINAL  MATCHES
    /**
     * @brief Matches a T
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post delegates to MatchFinal
     */
    template <class T>
    inline void Match(T * x){
    	MatchFinal(x,x);
    }

    /**
     * @brief Matches a T if not derivative of StreamI Object or StructuredDataI
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<!isSameOrBaseOf(StreamI,T)&&!isSameOrBaseOf(Object,T)&&!isSameOrBaseOf(StructuredDataI,T), T>::type *x);

    /**
     * @brief Matches a T if derivative of StreamI but not StreamString
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StreamI,T) && !isSame(StreamString,T), T>::type *x);

    /**
     * @brief Matches a T if StreamString - late matching allows sizeof incomplete class
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<isSame(StreamString,T), T>::type *x);

    /**
     * @brief Matches a T if derivative of StructuredDataI
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StructuredDataI,T), T>::type *x);

    /**
     * @brief Matches a T if derivative of Object
     * @tparam T the type of the elements in the vector
     * @param[in] x
     * @post closes the matching chain assigning the typeDescriptor
     */
    template <class T>
    inline void MatchFinal(T *y,typename enable_if<isSameOrBaseOf(Object,T), T>::type *x);

    // SPECIFIC FINAL MATCHES
    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     */
    inline void Match(char8 *i);

    /**
     * @brief Constructor from signed 8 bit integer.
     * @param[in] i is the signed 8 bit integer input.
     */
    inline void Match(int8 *i);

    /**
     * @brief Constructor from unsigned 8 bit integer.
     * @param[in] i is the unsigned 8 bit integer input.
     */
    inline void Match(uint8 *i);

    /**
     * @brief Constructor from constant signed 8 bit integer.
     * @param[in] i is the constant signed 8 bit integer input.
     */

    inline void Match(int16 *i);

    /**
     * @brief Constructor from unsigned 16 bit integer.
     * @param[in] i is the unsigned 16 bit integer input.
     */

    inline void Match(uint16 *i);

    /**
     * @brief Constructor from signed 32 bit integer.
     * @param[in] i is the signed 32 bit integer input.
     */
    inline void Match(int32 *i);

    /**
     * @brief Constructor from unsigned 32 bit integer.
     * @param[in] i is the unsigned 32 bit integer input.
     */
    inline void Match(uint32 *i);

    /**
     * @brief Constructor from signed 64 bit integer.
     * @param[in] i is the signed 64 bit integer input.
     */
    inline void Match(int64 *i);

    /**
     * @brief Constructor from unsigned 64 bit integer.
     * @param[in] i is the unsigned 64 bit integer input.
     */
    inline void Match(uint64 *i);

    /**
     * @brief Constructor from 32 bit float32 number.
     * @param[in] i is the 32 bit float32 number input.
     */
    inline void Match(float32 *i);

    /**
     * @brief Constructor from 64 bit float32 number.
     * @param[in] i the 64 bit float32 number input.
     */
    inline void Match(float64 *i);

    /**
     * @brief Constructor from void pointer.
     * @param[in] p is the void pointer input.
     */
    inline void Match(void * *p);

    /**
     * @brief Constructor by BitBoolean.
     * @tparam baseType the standard type which is used as a base for bitBool's type
     * @tparam bitOffset the actual bit offset of the bitBool's type
     * @param[in] bitBool is the BitBoolean object input.
     */
    template<typename baseType, uint8 bitOffset>
    void Match(BitBoolean<baseType, bitOffset> * bitBool);

    /**
     * @brief Constructor by BitRange.
     * @tparam baseType the standard type which is used as a base for bitRange's type
     * @tparam bitSize the actual bit size of the bitRange's type
     * @tparam bitOffset the actual bit offset of the bitRange's type
     * @param[in] bitRange is the BitRange object input.
     */
    template<typename baseType, uint8 bitSize, uint8 bitOffset>
    void Match(BitRange<baseType, bitSize, bitOffset> * bitRange);

    /**
     * @brief Constructor by FractionalInteger.
     * @tparam baseType the standard type which is used as a base for fractionalInt's type
     * @tparam bitSize the actual bit size of the fractionalInt's type
     * @param[in] fractionalInt is the FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    void Match(FractionalInteger<baseType, bitSize> * fractionalInt);

    /**
     * @brief Constructor from zeroterm malloced char *
     * @param[in] s is the string
     */
    inline void Match(DynamicCString *s);


    /**
     * @brief Constructor from zeroterm char[]
     * @param[in] s is the string
     */
    template <uint32 sz>
    inline void Match(StaticCString<sz> *s);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     */
    inline void Match(CString *s);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     */
    inline void Match(CCString *s);
    /**
     * @brief Constructor from StreamString (or inherited class).
     * @param[in] obj the source Object.
     * @details function templated to allow late calculation of sizeof(StreamString)
     */
    template <class T>
    inline void Match(StreamString *obj);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <class T>
inline  VariableDescriptor::VariableDescriptor( T  x){
    typeDescriptor = VoidType;
    Match(x);
}

template<typename T>
void VariableDescriptor::Match(Vector<T> * vec) {
	AddModifiersLayerConst('V', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(Matrix<T> * mat) {
	AddModifiersLayerConst('M', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T>
void VariableDescriptor::Match(ZeroTerminatedArray<T> * vec){
	AddModifiersLayerConst('P', 0);
	AddModifiersLayerConst('Z', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T >
void VariableDescriptor::Match(DynamicZeroTerminatedArray<T,16u> * vec){
	AddModifiersLayerConst('P', 0);
	AddModifiersLayerConst('D', 0);

    T *pp = NULL;
    Match(pp);
}

template<typename T, uint32 sz >
void VariableDescriptor::Match(StaticZeroTerminatedArray<T,sz> * vec){
	AddModifiersLayerConst('P', 0);
	AddModifiersLayerConst('S', sz);

    T *pp = NULL;
    Match(pp);
}

void VariableDescriptor::Match(DynamicCString *s){
//	AddModifiersLayerConst('P', 0);
//	AddModifiersLayerConst('D', 0);
//	FinaliseCode(Character8Bit);
	bool isConstant = typeDescriptor.dataIsConstant;
	if (isConstant){
		FinaliseCode(ConstCharString(sizeof(DynamicCString)));
	} else {
		typeDescriptor.dataIsConstant = false;
		FinaliseCode(DynamicCharString);
	}
}

void VariableDescriptor::Match(CString *s){
//	AddModifiersLayerConst('P', 0);
//	AddModifiersLayerConst('Z', 0);
//	FinaliseCode(Character8Bit);
	FinaliseCode(CharString);
}

void VariableDescriptor::Match(CCString *s){
//	AddModifiersLayerConst('P', 0);
//	AddModifiersLayerConst('Z', 0);
//    AddConstantCode();
//	FinaliseCode(Character8Bit);
	FinaliseCode(ConstCharString(sizeof(CString)));
}

// TODO wrong. this is not a pointer!
template <uint32 sz>
void VariableDescriptor::Match(StaticCString<sz> *s){
//	AddModifiersLayerConst('P', 0);
//	AddModifiersLayerConst('S', sz);
//	FinaliseCode(Character8Bit);
	FinaliseCode(ConstCharString(sz));
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match(T (*x) [n]){
	AddModifiersLayerConst('A', n);

    T *pp = NULL;
    Match(pp);
}

template <class T,unsigned int n>
inline void VariableDescriptor::Match( const T (*x) [n]){
	AddModifiersLayerConst('A', n);

    const T *pp = NULL;
    Match(pp);
}


template <class T>
inline void VariableDescriptor::Match(T ** x){
	AddModifiersLayerConst('P', 0);

    T *pp = NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T * const * x){

    AddConstantCode();
    T ** pp=NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T const * * x){
	AddModifiersLayerConst('P', 0);

    T const * pp=NULL;
    Match(pp);
}

template <class T>
inline void VariableDescriptor::Match(T const * const * x){

    AddConstantCode();
    T const * * pp=NULL;
    Match(pp);
}

template <class T>
void VariableDescriptor::Match(T const * x){

    AddConstantCode();
    T * pp=NULL;
    Match(pp);
}

template <class T>
void VariableDescriptor::Match(StreamString *s){
	FinaliseCode(StreamStringType(sizeof(T)));
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<!isSameOrBaseOf(StreamI,T)&&!isSameOrBaseOf(Object,T)&&!isSameOrBaseOf(StructuredDataI,T), T>::type *x){
	ClassRegistryItem *cri =  ClassRegistryItem::Instance<T>();
    if (cri != NULL) {
    	FinaliseCode(cri->GetTypeDescriptor());
    }
    else {
    	FinaliseCode(VoidType);
    }
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StreamI,T) && !isSame(StreamString,T), T>::type *x){
	FinaliseCode(StreamType(sizeof(T)));
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSame(StreamString,T), T>::type *x){
	FinaliseCode(StreamStringType(sizeof(T)));
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSameOrBaseOf(StructuredDataI,T), T>::type *x){
	FinaliseCode(StructuredDataType(sizeof(T)));
}

template <class T>
void VariableDescriptor::MatchFinal(T *y,typename enable_if<isSameOrBaseOf(Object,T), T>::type *x){
	FinaliseCode(ObjectType(sizeof(T)));
}


void VariableDescriptor::Match(char8 * i) {
	FinaliseCode(Character8Bit);
}

void VariableDescriptor::Match(int8 * i) {
	FinaliseCode(SignedInteger8Bit);
}

void VariableDescriptor::Match(uint8 * i) {
	FinaliseCode(UnsignedInteger8Bit);
}

void VariableDescriptor::Match(int16 * i) {
	FinaliseCode(SignedInteger16Bit);
}

void VariableDescriptor::Match(uint16 * i) {
	FinaliseCode(UnsignedInteger16Bit);
}

void VariableDescriptor::Match(int32 * i) {
	FinaliseCode(SignedInteger32Bit);
}

void VariableDescriptor::Match(uint32 * i) {
	FinaliseCode(UnsignedInteger32Bit);
}

void VariableDescriptor::Match(int64 * i) {
	FinaliseCode(SignedInteger64Bit);
}

void VariableDescriptor::Match(uint64 * i) {
	FinaliseCode(UnsignedInteger64Bit);
}

void VariableDescriptor::Match(float32 * i) {
	FinaliseCode(Float32Bit);
}

void VariableDescriptor::Match(float64 * i) {
	FinaliseCode(Float64Bit);
}

void VariableDescriptor::Match(void * * p) {
	FinaliseCode(PointerType);
}

template<typename baseType, uint8 bitOffset>
void VariableDescriptor::Match(BitBoolean<baseType, bitOffset> * bitBool) {
	FinaliseCode( BitSetBoolean(baseType,bitOffset));
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
void VariableDescriptor::Match(BitRange<baseType, bitSize, bitOffset> * bitRange) {
	FinaliseCode((TypeCharacteristics::IsSigned<baseType>()) ? SignedBitSet(baseType,bitSize,bitOffset) : UnsignedBitSet(baseType,bitSize,bitOffset));
}

template<typename baseType, uint8 bitSize>
void VariableDescriptor::Match(FractionalInteger<baseType, bitSize> * fractionalInt) {
	FinaliseCode((TypeCharacteristics::IsSigned<baseType>()) ? SignedBitSet(baseType,bitSize,0) : UnsignedBitSet(baseType,bitSize,0));
}

void VariableDescriptor::AddConstantCode(){
	typeDescriptor.dataIsConstant = true;
}




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* L2OBJECTS_VARIABLEDESCRIPTOR_H_ */
	
