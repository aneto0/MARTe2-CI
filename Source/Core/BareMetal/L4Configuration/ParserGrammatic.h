/**
 * @file ParserGrammatic.h
 * @brief Header file for class ParserGrammatic
 * @date 27/11/2015
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

 * @details This header file contains the declaration of the class ParserGrammatic
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PARSERGRAMMATIC_H_
#define PARSERGRAMMATIC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Contains the terminal and separator characters used by Parser and LexicalAnalyzer.
 * @details
 *   In order to specify a valid grammatic for the implemented parser, all the characters must be !='\0'
 *   and the \a openTypeCast character must be different from the other open terminals.
 */
struct ParserGrammatic {
    /**
     * List of separator characters.
     */
    const char8 *separators;
    /**
     *Aassignment operator
     */
    char8 assignment;
    /**
     * Specifies that a block begins
     */
    char8 openBlock;
    /**
     * Specifies that a block ends
     */
    char8 closeBlock;
    /**
     * Specifies that a vector begins
     */
    char8 openVector;
    /**
     * Specifies that a vector ends
     */
    char8 closeVector;
    /**
     * Specifies that a matrix begins
     */
    char8 openMatrix;
    /**
     * Specifies that a matrix ends
     */
    char8 closeMatrix;
    /**
     * Specifies that a type cast expression begins
     */
    char8 openTypeCast;
    /**
     * Specifies that a type cast expression ends
     */
    char8 closeTypeCast;
    /**
     * Allows to get the terminals as a C-string.
     */
    char8 terminal;
};

/**
 * The most used terminal and separator characters in MARTe configuration streams.
 */
static const ParserGrammatic StandardGrammatic = { "\n\r\t, ", '=', '{', '}', '{', '}', '{', '}', '(', ')', '\0'};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PARSERGRAMMATIC_H_ */

