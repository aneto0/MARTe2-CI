/**
 * @file StaticCString.h
 * @brief Header file for class StaticCString
 * @date Dec 16, 2016
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

 * @details This header file contains the declaration of the class StaticCString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPES_STATICCSTRING_H_
#define L0TYPES_STATICCSTRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticZeroTerminatedArray.h"
#include "CCString.h"
#include "CStringTool.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
* TODO
* @brief Wrapper for writable char buffers
* */
template<uint32 size>
class StaticCString: public StaticZeroTerminatedArray<char8,size>{

public:
    /*
     * TODO
     */
    inline operator CCString() const;

    /**
     * @briefs uses the memory provided by s
     * s is reset to length 0;
    */
    inline StaticCString (char8  (&s)[size]);

    /**
     *
     */
    inline CStringTool operator()();

    /**
     * @brief Retrieves the size of the TArray().
     * @return the number of elements in the TArray() (excluding the terminator Zero).
     */
    inline uint32 GetSize() const;

    /**
     * @brief Retrieves the maximum size of the string that can be stored here
     * @return the number of elements of the buffer -1
     */
    inline uint32 MaximumSize() const;

    /**
     * @brief Returns the pointer to the beginning of the TArray().
     * @return the pointer to the beginning of the TArray().
     */
    inline char8 * GetList() const ;

#if 0
    /**
     * @brief append a number at the end of the string
     */
    inline bool AppendHex(uint64 num);

    /**
     * @brief append a char at the end of the string
     */
    inline bool Append(const char8 c);

    /**
     * @brief append a string at the end of the string
     */
    inline bool Append(const char8 *s,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief append a string at the end of the string
     */
    inline bool Append(const CCString &s,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief append a string at the end of the string
     */
    inline bool Append(const StaticCString &s,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint64 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint32 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint16 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint8 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int64 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int32 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int16 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int8 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const double num);
    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const float num);

private:
    template <typename T>
    inline bool AppendT(T num);
#endif

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <uint32 size>
StaticCString<size>::StaticCString (char8  (&s)[size]):StaticZeroTerminatedArray<char8,size >(s){
//    ZeroTerminatedArray<char8 >::array = &s[0];
    s[0] = '\0';
}

template <uint32 size>
CStringTool StaticCString<size>::operator()(){
	return CStringTool(NULL_PTR(char8 **),ZeroTerminatedArray<char8>::array ,size);
}

template <uint32 size>
StaticCString<size>::operator CCString() const{
	return CCString(ZeroTerminatedArray<char8>::array);
}

template <uint32 size>
uint32 StaticCString<size>::GetSize() const{
	return StaticZeroTerminatedArray<char8,size >::GetSize();
}

template <uint32 size>
inline uint32 StaticCString<size>::MaximumSize() const{
	return size-1;
}

template <uint32 size>
char8 * StaticCString<size>::GetList() const {
	return StaticZeroTerminatedArray<char8,size >::GetList();
}

#if 0
template <uint32 size>
StaticCString<size>::operator const char8*() const{
    return ZeroTerminatedArray<char8>::array;
}

template <uint32 size>
bool StaticCString<size>::Append(const uint64 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const uint32 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const uint16 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const uint8 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const int64 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const int32 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const int16 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const int8 num){
	return AppendT(num);
}

template<uint32 size>
bool StaticCString<size>::Append(const float num){
	int64 numI = num;
	return AppendT(numI);
}

template<uint32 size>
bool StaticCString<size>::Append(const double num){
	int64 numI = num;
	return AppendT(numI);
}

template<uint32 size>
template <typename T>
bool StaticCString<size>::AppendT(T num){
	bool ret = true;
	if (num < 0){
		ret = ret && Append('-');
		num = -num;
	}

	T tester = 1;
	T tested = num/10;
	while (tester <= tested){
		tester *= 10;
	}
	while (tester > 0){
		uint8 digit = num/tester;
		ret = ret && Append(static_cast<char8>(static_cast<uint8>('0')+digit));
		num     = num%tester;
		tester /= 10;
	}
	return ret;
}

template<uint32 size>
bool StaticCString<size>::AppendHex(uint64 num){
	bool ret = true;
	for (int i = 60;(i>=0) && ret;i-=4){
		uint8 n = (num >> i) & 0xF;
		if (n >= 10) ret = ret && Append(static_cast<char8>(static_cast<uint8>('A')+n-10));
		else         ret = ret && Append(static_cast<char8>(static_cast<uint8>('0')+n));
	}
	return ret;
}

template<uint32 size> bool StaticCString<size>::Append(const char8 c){
	return StaticZeroTerminatedArray<char8,size>::Append(c);
}

template<uint32 size> bool StaticCString<size>::Append(const char8 *s,uint32 maxAppendSize){
	return StaticZeroTerminatedArray<char8,size>::AppendN(s,maxAppendSize);
}
template<uint32 size> bool StaticCString<size>::Append(const StaticCString &s,uint32 maxAppendSize){
	return StaticZeroTerminatedArray<char8,size>::AppendN(s.GetList(),maxAppendSize);
}

template<uint32 size> bool StaticCString<size>::Append(const CCString &s,uint32 maxAppendSize){
	return StaticZeroTerminatedArray<char8,size>::AppendN(s.GetList(),maxAppendSize);
}

#endif

}
#endif /* L0TYPES_STATICCSTRING_H_ */
	
