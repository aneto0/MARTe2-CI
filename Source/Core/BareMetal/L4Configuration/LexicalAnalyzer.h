/**
 * @file LexicalAnalyzer.h
 * @brief Header file for class LexicalAnalyzer
 * @date 25/11/2015
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

 * @details This header file contains the declaration of the class LexicalAnalyzer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamI.h"
#include "StaticList.h"
#include "Token.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Generic lexical analyzer (lexer) which allows to interpret a stream
 * of characters as a sequence of tokens, applying specific lexical rules set
 * at instance level.
 *
 * @details Each instance of the lexer is bound when it is constructed, with
 * the stream of characters to be analysed, which will remain the same during
 * the object lifetime. At construction time, too, the lexer is initialized
 * with the configuration of lexical elements that will rule the analysis
 * (terminals, separators, and comment markers).
 *
 * Once the lexer is created, it allows its users to read tokens from the
 * stream of characters one by one, being the lexer responsible of applying
 * the lexical rules of the configuration.
 *
 * Note: Each read token is an instance of the class Token which can be of
 * one of the following types:
 * - NUMBER_TOKEN: If the token represents a number (also in hexadecimal,
 *   octal, or binary format).
 * - STRING_TOKEN: If the token represents a string (beginning with a ' " '
 *   or a 'non-number' character at the beginning).
 * - ERROR_TOKEN: If the token is invalid (e.g. 1234a56).
 * - TERMINAL_TOKEN: If the token is a terminal.
 * - EOF_TOKEN: If the read operation from the stream fails.
 *
 */
/*lint -e1712 . Justification: This class must be as per the only defined
 * constructor. No need for a default constructor.*/
class DLL_API LexicalAnalyzer {

public:

    /**
     * @brief Constructor which initializes the instance with the stream of
     * characters to analyze and the configuration of the analyzer (terminals,
     * separators, and comment markers).
     * @param[in] stream the stream of characters to be tokenized.
     * @param[in] terminalsIn C-string containing the list of terminal
     * characters, being each character a terminal.
     * @param[in] separatorsIn C-string containing the list of separator
     * characters, being each character a terminal.
     * @param[in] oneLineCommentBeginIn C-string containing the pattern that
     * it is used for marking the beginning of a single line comment.
     * @param[in] multipleLineCommentBeginIn C-string containing the pattern
     * that it is used for marking the beginning of a multiple line comment.
     * @param[in] multipleLineCommentEndIn C-string containing the pattern
     * that it is used for marking the end of a multiple line comment.
     */
    LexicalAnalyzer(StreamI &stream,
    		CCString const terminalsIn,
			CCString const separatorsIn,
			CCString const oneLineCommentBeginIn,
			CCString const multipleLineCommentBeginIn,
			CCString const multipleLineCommentEndIn);

    /**
     * @brief Destructor.
     */
    ~LexicalAnalyzer();

    /**
     * @brief Extracts the next token from the stream.
     * @return a pointer to the next token found in the stream.
     */
    Token *GetToken();

    /**
     * @brief Returns the nth token in the stream without consuming it.
     * @param[in] position is the position of the nth token in the stream (0 means the next).
     * @return the nth token found in the stream.
     */
    Token *PeekToken(const uint32 position);

private:

    /*
     * Implementation details of the class:
     * The implementation adopts a lazy strategy, so it tokenizes the stream of
     * characters on demand, each time GetToken or PeekToken are called.
     */

    /**
     * @brief Tokenizes the stream in input adding tokens to the internal queue.
     * @param[in] level is the number of tokens to add to the queue - 1.
     */
    void TokenizeInput(const uint32 level = 0u);

    /**
     * @brief Adds a token to the internal queue.
     * @param[in,out] tokenBuffer contains the token data. It will be modified to remove the "" in the case of a string
     * @param[in] isString specifies if the token represents a string.
     */
    void AddToken(CString tokenBuffer, const bool isString);

    /**
     * @brief Adds a terminal to the internal queue.
     * @param[in] terminal is the terminal character.
     */
    void AddTerminal(const char8 terminal);

    /**
     * @brief Skips the comments in the stream.
     * @param[in] stream is the stream of characters to be read.
     * @param[out] buffer contains the data read from the stream.
     * @param[out] bufferSize the actual size of the data in buffer
     * @param[out] lineNumber is the token line number.
     * @param[in] separators is the separator characters list.
     * @param[in] oneLineBegin C-string containing the pattern that
     * it is used for marking the beginning of a single line comment.
     * @param[in] multipleLineBegin C-string containing the pattern that
     * it is used for marking the beginning of a multiple line comment.
     * @param[in] multipleLineEnd C-string containing the pattern that
     * it is used for marking the end of a multiple line comment.
     * @param[out] separator returns the separator char found at the end of the comment.
     * @param[in] isNewToken specifies if the separators at the beginning must be skipped or not.
     * @return false if EOF, true otherwise.
     */
    bool SkipComment(StreamI &stream,
                            StaticCString<16> &buffer,
                            uint32 &bufferSize,
                            uint32 &lineNumber,
                            char8 &separator,
                            const bool isNewToken) ;

    /**
     * Internal token queue
     */
    StaticList<Token *> tokenQueue;

    /**
     * Separator characters
     */
    CCString const separators;

    /**
     * Terminal characters
     */
    CCString  terminals;

    /**
     * The begin of single line comment pattern.
     */
    CCString const oneLineCommentBegin;

    /**
     * The begin of multiple line comment pattern.
     */
    CCString const multipleLineCommentBegin;

    /**
     * The end of multiple line comment pattern.
     */
    CCString const multipleLineCommentEnd;

    /**
     * Default token types
     */
    TokenInfo 		tokenInfo[5];

    /**
     * The last token returned
     */
    Token *token;

    /**
     * Pointer to the stream to be tokenized
     */
    StreamI &inputStream;

    /**
     * Line number counter
     */
    uint32 lineNumber;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LEXICALANALYZER_H_ */

