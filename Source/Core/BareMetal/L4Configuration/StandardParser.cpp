/**
 * @file StandardParser.cpp
 * @brief Source file for class StandardParser
 * @date 09/12/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "License")
 * You may not use this work except in compliance with the License.
 * You may obtain a copy of the License at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License permissions and limitations under the License.

 * @details This source file contains the definition of all the methods for
 * the class StandardParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


StandardParser::StandardParser(StreamI &stream,
                               StructuredDataI &databaseIn,
                               BufferedStreamI * const err) :
		StructuredDataParserI(stream, databaseIn, err, StandardParserData::parserData)
{

	MapMethods();
}

StandardParser::~StandardParser() {

}



void StandardParser::Execute(const uint32 number,const Token *currentToken,BufferedStreamI *errorStream) {
	(this->*Action[number])(currentToken,errorStream);
}



}
