/**
 * @file SimpleStream.h
 * @brief Header file for class AnyType
 * @date 24 Jun 2019
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

#ifndef TEST_CORE_BAREMETAL_L2OBJECTS_SIMPLESTREAM_H_
#define TEST_CORE_BAREMETAL_L2OBJECTS_SIMPLESTREAM_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "VoidStream.h"
#include "DynamicCString.h"
/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class SimpleStream: public VoidStream,public DynamicCString {
	uint32 index;
public:
	SimpleStream(CCString init=emptyString);
	void Init(CCString init);
    virtual bool CanRead() const;
    virtual bool CanSeek() const;
    virtual bool CanWrite() const;
    virtual bool Read(char8 * const output,uint32 & size);
    virtual bool Seek(uint64 pos);
    virtual bool Write(const char8 * const input, uint32 & size);
    virtual bool SetSize(uint64 size);
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* TEST_CORE_BAREMETAL_L2OBJECTS_SIMPLESTREAM_H_ */
