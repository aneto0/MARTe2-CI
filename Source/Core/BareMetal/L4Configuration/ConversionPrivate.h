/**
 * @file ConversionPrivate.h
 * @brief Header file for class AnyType
 * @date 12 Nov 2017
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

#ifndef CONVERSIONPRIVATE_H_
#define CONVERSIONPRIVATE_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe {


bool StringToInteger(const char8 * const input,uint8  &number);
bool StringToInteger(const char8 * const input,uint16 &number);
bool StringToInteger(const char8 * const input,uint32 &number);
bool StringToInteger(const char8 * const input,uint64 &number);
bool StringToInteger(const char8 * const input,int8   &number);
bool StringToInteger(const char8 * const input,int16  &number);
bool StringToInteger(const char8 * const input,int32  &number);
bool StringToInteger(const char8 * const input,int64  &number);
bool StringToFloat  (const char8 * const input,float  &number);
bool StringToFloat  (const char8 * const input,double &number);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* CONVERSIONPRIVATE_H_ */
