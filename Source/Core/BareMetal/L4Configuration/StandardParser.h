/**
 * @file StandardParser.h
 * @brief Header file for class StandardParser
 * @date 09/12/2015
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

 * @details This header file contains the declaration of the class StandardParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STANDARDPARSER_H_
#define STANDARDPARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StructuredDataParserI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * The lexical elements for MARTe configuration streams encoded in standard MARTe configuration language.
 */
static const GrammarInfo StandardGrammar = { "\n\r\t, ", "//", "/*", "*/", 	"={}()" };

namespace StandardParserData{
	extern const ParserData parserData;
}


/**
 * @brief Concrete class for MARTe::ParserI abstract class, configured
 * for streams of characters encoded in MARTe configuration language.
 *
 * @details This class is a concrete class for MARTe::ParserI providing the
 * actual lexical elements and parsing rules for interpreting a stream of
 * characters encoded in the MARTe configuration language.
 *
 * Each instance of the parser is bound when it is constructed, with all the
 * objects involved in the parsing analysis, as follows:
 * - An input stream of characters that contains the serialization of a
 * hierarchy of objects, encoded into the MARTe configuration language.
 * - An output structured data store where the parser will build the in-
 * memory objects defined into the input stream of characters.
 * - An output stream of characters where the parser will write all the errors
 * found on the input stream of characters.
 *
 * All the instances of the parser use the lexical elements defined
 * in MARTe::StandardGrammar and apply the parsing rules of the following
 * grammar:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.py
 * EXPRESSION:
 *   STRING __GetNodeName   = VARIABLE __AddLeaf
 *   STRING __GetNodeName   = ( STRING __GetTypeCast ) VARIABLE __AddLeaf
 *   STRING __CreateNode    = \\{ { EXPRESSION }+ \\} __EndBlock
 *
 * VARIABLE:
 *   SCALAR
 *   VECTOR
 *   MATRIX
 *
 * SCALAR:
 *   TOKEN __AddScalar
 *
 * VECTOR:
 *   { SCALAR }+ __EndVector
 *
 * MATRIX:
 *   { VECTOR }+ __EndMatrix
 *
 * TOKEN:
 *   STRING
 *   NUMBER
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Note: This grammar is written in the SLK language and refers to functions
 * declared in MARTe::ParserI.
 */
class DLL_API StandardParser: public StructuredDataParserI {

public:

    /**
     * @brief Default constructor.
     * @param[in] stream is the stream to be parsed.
     * @param[out] databaseIn is the built StructuredData in output.
     * @param[out] err is the stream where error messages are printed to.
     */
    StandardParser(StreamI &stream,
                   StructuredDataI &databaseIn,
                   BufferedStreamI * const err = static_cast<BufferedStreamI*>(NULL));

    /**
     * @brief Destructor.
     */
    virtual ~StandardParser();

private:

    /**
     * @see ParserI::Execute(*).
     */
    virtual void Execute(const uint32 number);

    /**
     * implemented in XMLParserData.cpp
     */
	void MapMethods();

    /**
     * The array of functions needed by the parser.
     */
    void (StandardParser::*Action[10])(void);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STANDARDPARSER_H_ */

