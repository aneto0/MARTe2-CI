/**
 * @file ZeroTerminatedArray.h
 * @brief Header file for class ZeroTerminatedArray
 * @date 22/09/2015
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

 * @details This header file contains the declaration of the class ZeroTerminatedArray
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ZEROTERMINATEDARRAY_H_
#define ZEROTERMINATEDARRAY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief calculate the number of elements of any ZeroTermArray whose elements have size elSize
 * @param[in] pointer is the pointer to the data array
 * @param[in] elSize is the size of each element in the array
 * @return the number of elements until one with all zeroes is found
 */
uint32 ZeroTerminatedArrayGetSize(const uint8 *pointer, uint32 elSize);

/**
 * @brief Describes a zero-terminated array.
 *
 * @warning This class is only a wrapper of a pointer. The implementation assumes that the pointer
 * in input is a zero-terminated array and does not check if it is not zero-terminated or NULL.
 * If this pre-condition is not accomplished, a segmentation fault in runtime could happen.
 */
template<typename T>
class ZeroTerminatedArray {
public:

    /**
     * @brief Default constructor.
     * @param[in] arrayIn is the zero terminated array input.
     * @pre
     *   arrayIn is zero terminated.
     */
    ZeroTerminatedArray(T *arrayIn=NULL_PTR(T *));

    /**
     * @brief Returns the element in the specified position.
     * @param[in] index is the element position in the array.
     * @return the element in the \a index position.
     */
    inline T &operator[](const uint32 index) const;

    /**
     * @brief Retrieves the size of the array.
     * @return the number of elements in the array (excluding the terminator Zero).
     */
    uint32 GetSize() const;

    /**
     * @brief Finds the first location in the array that contains T
     * @return the position in the array where T is found, 0xFFFFFFFF if not found.
     */
    uint32 Find(const T & data) const;

    /**
     * @brief Checks if data is in this array
     * @return true if found
     */
    inline bool In(const T& data) const;
    /**
     * @brief Returns the pointer to the beginning of the array.
     * @return the pointer to the beginning of the array.
     */
    T * GetList() const ;

    /**
     * @brief Allows changing the list
     * @param[in] arrayIn is the zero terminated array input.
     */
    void SetList(T *arrayIn)  ;

    /**
     * move pointer ahead of one element. Checks limits
     * if !Zero(*array) && (array != NULL)  array++;
     */
    inline void Skip();

    /**
     * move pointer ahead of one element. Does not checks limits
     * note that the parameter int is only to specify postpending
     */
    inline void operator++(int);

    /**
     * @brief Checks if the input \a data is the array terminal character.
     * @details This function must be specialized for each new template type. The default
     * terminal is (as the name of the class implies) static_cast<T>(0)
     * @param[in] data is the element to be checked.
     * @return true if \a data is the array terminal, false otherwise.
     */
    inline bool Zero(const T & data) const;

    /**
     * @brief Checks if the ptr is valid
     * @return true if \a array is NULL.
     */
    inline bool IsNullPtr() const;

    /**
     * @brief Checks if the input \a arrayIn has the same content as the array
     * @details This function allows implementing operator==
     * @param[in] arrayIn is the array to be compared
     * @return true if \a arrayIn is the same.
     */
    inline bool isSameAs(const T *arrayIn) const;



protected:

    /**
     * The array
     */
    T * array;

    /**
     * to access pointer as void
     */
    inline void *&VoidArray();

    /**
     * to access pointer as void
     */
    inline T *&TArray();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
void *&ZeroTerminatedArray<T>::VoidArray(){
    return const_cast<void *&>(reinterpret_cast<void  * &>(array));
}

template<typename T>
T *&ZeroTerminatedArray<T>::TArray(){
    return array;
}

template<typename T>
ZeroTerminatedArray<T>::ZeroTerminatedArray(T *arrayIn) :
    array(arrayIn) {
}

template<typename T>
inline T &ZeroTerminatedArray<T>::operator[](const uint32 index) const {
    return array[index];
}

template<typename T>
uint32 ZeroTerminatedArray<T>::GetSize() const {
    uint32 size = 0u;
    if (array != NULL_PTR(T*)) {
        const T * listP = array;
        while (!Zero(listP[0])) {
            listP = &listP[1];
            size++;
        }
    }
    return size;
}


template<typename T>
uint32 ZeroTerminatedArray<T>::Find(const T & data) const{
    uint32 pos = 0xFFFFFFFFu;
    if (array != NULL_PTR(T*)) {
        uint32 index = 0;
        while (!Zero(array[index]) && (pos == 0xFFFFFFFFu)) {
        	if (data == array[index]){
        		pos = index;
        	}
            index++;
        }
    }
    return pos;
}

template<typename T>
T * ZeroTerminatedArray<T>::GetList() const{
    return array;
}

template<typename T>
void ZeroTerminatedArray<T>::SetList(T *arrayIn) {
    array = arrayIn;
}

template<typename T>
bool ZeroTerminatedArray<T>::Zero(const T & data) const {
	static const T term(0u);
    return (data == term);
}

template<typename T>
bool ZeroTerminatedArray<T>::IsNullPtr() const {
    return (array == NULL_PTR(T *));
}


template<typename T>
void ZeroTerminatedArray<T>::Skip() {
    if (array != NULL_PTR(T *))  {
        if (!Zero(*array)){
            array++;
        }
    }
}

template<typename T>
void ZeroTerminatedArray<T>::operator++(int) {//int is for postfix operator!
    array++;
}


template<typename T>
bool ZeroTerminatedArray<T>::isSameAs(const T *arrayIn) const {

    bool same = true;
    if ((array != NULL_PTR(T*))&&(arrayIn != NULL_PTR(T*))) {
        const T * listP = array;
        const T * list2P = arrayIn;
        while (!Zero(*listP) && same) {
            same = (*listP == *list2P);
            listP++;
            list2P++;
        }
        if (same){
        	same = Zero(*list2P);
        }
    }
    return same;

}

template<typename T>
bool ZeroTerminatedArray<T>::In(const T& data) const{
	return (Find(data)!= 0xFFFFFFFF);
}


}

#endif /* ZEROTERMINATEDARRAY_H_ */

