/**
 * @file CLASSMEMBERREGISTER.h
 * @brief Header file for class CLASSMEMBERREGISTER
 * @date 04/05/2017
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
 *
 * @details This header file contains the declaration of the class CLASSREGISTER
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */
#ifndef CLASSMEMBERREGISTER_H_
#define CLASSMEMBERREGISTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <typeinfo>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItem.h"
#include "ClassMember.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * These macros are required for the application to automatically register in the ClassRegistryDatabase
 * the information associated to every class that inherits from Object.
 * These have to be defined as macros given that the new and delete functions are specific for each
 * final class that inherits from Object (new and delete are static and thus cannot be virtual).
 */
namespace MARTe{
/**
 * This is a zero storage class.
 * It is used to define zero storage global static objects for the sole purpose of
 * calling the constructor code which will register method information about a class
 */
class ClassMemberRegister{
public:

	/**
	 * @brief constructor. Its job is to add class member info to the database
     * @param[in] cri is the class record to be filled with information : always MARTe::ClassRegistryItem::Instance<className>()
     * @param[in] member is the member to be documented
     * @param[in] nameIn is the name of the member to be documented
     * @param[in] offsetIn is the position of the member within the class
     * @param[in] size is the size of the member
	 */
	template <class  Tmember>
	ClassMemberRegister(
			            ClassRegistryItem * cri,
			            Tmember &member,
			            CCString nameIn,
						uint32 offsetIn,
			            uint32 size)
	{
	    if (cri != NULL_PTR(ClassRegistryItem * )){
	        cri->AddMember(new ClassMember(&member, nameIn, offsetIn));
	    }
	}

};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
/**
 * Use this macro set to help registration of private and protected class members
 * This first macro needs to be used within the class
 */
#define CLASS_PRIVATE_MEMBER_DECLARE(memberName)\
		static MARTe::ClassMemberRegister   _ ## memberName ## _ ## CMR;

/**
 * Use this macro set to help registration of private and protected class members
 * This second macro needs to be used within a single .cpp file
 */
#define CLASS_PRIVATE_MEMBER_REGISTER(className, memberName)\
		MARTe::ClassMemberRegister  className :: _ ## memberName ## _ ## CMR (\
               MARTe::ClassRegistryItem::Instance<className>(),\
               memberOf(className, memberName),\
               #memberName, \
               static_cast<uint32>(indexof(className, memberName)),\
               msizeof(className, memberName) );


#define CLASS_MEMBER_REGISTER(className, memberName)\
		MARTe::ClassMemberRegister  className ## _ ## memberName ## _ ## CMR (\
               MARTe::ClassRegistryItem::Instance<className>(),\
               memberOf(className, memberName),\
               #memberName, \
               static_cast<uint32>(indexof(className, memberName)),\
               msizeof(className, memberName) );

#define CLASS_INHERIT_REGISTER(className, className2)\
		MARTe::ClassMemberRegister  className ## _ ## className2 ## _ ## CMR (\
               MARTe::ClassRegistryItem::Instance<className>(),\
               *((className2 *)(1024)),\
               "", \
			   static_cast<uint32>(ancestorIndexof(className, className2)),\
               sizeof(className2) );


#endif /* CLASSMEMBERREGISTER_H_ */
