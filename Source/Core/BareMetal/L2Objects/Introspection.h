/**
 * @file Introspection.h
 * @brief Header file for class Introspection
 * @date 05/08/2015
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class Introspection
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTION_H_
#define INTROSPECTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "IntrospectionEntry.h"
#include "ZeroTerminatedArray.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



namespace MARTe {


/**
 * @brief Describes the class
 */
class  Introspection {

public:

    Introspection(const IntrospectionEntry ** const introspectionListIn);

    const IntrospectionEntry operator[](const uint32 index) const;

private:

    /**
     * An array of pointer to the class member's descriptors.
     * The array must be zero-terminated.
     */
    ZeroTerminatedArray<const IntrospectionEntry *> fields;

};



}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#define introspectionMemberIndex(className, memberName) \
    (intptr)&(((className *)0)->memberName)


#define introspectionMemberSize(className, memberName) \
    sizeof(((className *)0)->memberName)


#define DECLARE_CLASS_MEMBER(className, memberName, type, isConstant, modifierString ) \
    static const IntrospectionEntry className ## _ ## memberName ## _introspectionEntry =      \
    IntrospectionEntry(                                                                        \
        #memberName,                                                                           \
        #type,                                                                                 \
        isConstant,                                                                            \
        modifierString,                                                                        \
        introspectionMemberSize(className, memberName),                                        \
        introspectionMemberIndex(className, memberName)                                        \
    )

#endif /* INTROSPECTION_H_ */
