/**
 * @file BufferedStreamI.h
 * @brief Header file for class BufferedStreamI
 * @date 26/10/2015
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

 * @details This header file contains the declaration of the class BufferedStreamI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFEREDSTREAMI_H_
#define BUFFEREDSTREAMI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeoutType.h"
#include "AnyType.h"
#include "FormatDescriptor.h"
#include "IOBuffer.h"
#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstract super class for all buffered streams.
 *
 * @details This class provides the common interface specification for all the
 * streams which support buffering.
 *
 * The stream represents a sequence of bytes which can be mapped to arrays,
 * strings, files, sockets, and so on, but that it can be accessed uniformly
 * using a common read/write interface. A stream is also meant to be able to
 * have the notion of cursor, which can be positioned randomly on any byte of
 * the stream; size, which allows to know the actual size of the stream; and
 * timeout, which allows to define a time out for read and write operations,
 * so they will not last forever in case of I/O problems. Moreover, the
 * stream have implicit reading and writing buffers which are expected to be
 * defined by subclasses of the stream, but used by upper level functions
 * like read or write.
 *
 * Attributes:
 * - Size: The size of the stream, i.e. the number of bytes that it holds.
 * - Position: The position of the cursor, i.e. the zero-based index of the
 *   byte where the next read/write operation will occur.
 * - Timeout: It is the time out for read and write operations, infinite
 *   by default.
 * - ReadBuffer: It is the buffer for reading data, which will be used by
 *   the specific implementations of the read function.
 * - WriteBuffer: It is the buffer for writing data, which will be used by
 *   the specific implementations of the write function.
 *
 * Operators:
 * - AnyType()
 *
 * Operations:
 * - Read
 * - Write
 * - Seek
 * - Copy (*)
 * - GetToken (*)
 * - SkipTokens (*)
 * - GetLine (*)
 * - Printf (*)
 * - PrintFormatted (*)
 *
 * (*) The class supplies a standard implementation for these operations.
 *
 * @warning The generic capabilities for reading, writing, and seeking can not
 * assured to be available on all kind of mapped streams, so an additional set
 * of "CanXXX" methods is provided to verify the capabilities of the stream itself.
 *
 * - CanRead: Queries if the stream is readable, i.e. if the operation Read
 *   is implemented.
 * - CanWrite: Queries if the stream is writable, i.e. if the operation Write
 *   is implemented.
 * - CanSeek: Queries if the stream is seekable, i.e. if the operations Seek
 *   and RelativeSeek are implemented.
 *
 * @warning The size and position attributes can be meaningless in some kind
 * of streams, so in these cases size and position will be set to the
 * maximum value of uint64. This also implies that operations like read and
 * write, which increments position and/or size, will not change this value.
 */

class DLL_API BufferedStreamI: public StreamI {

public:

    /**
     * Default constructor
     * @post
     *   GetTimeout() == TTInfiniteWait
     */
    BufferedStreamI();

    /**
     * @brief Default destructor
     */
    virtual ~BufferedStreamI();

#if 0  // notneeded as AnyType supports StreamI
    /**
     * @brief Casts the stream to AnyType.
     * @return an AnyType representation of the stream.
     */
    inline operator AnyType();
#endif

    /**
     * @brief Reads a token from the stream into a character buffer.
     * @details Extracts a token from the stream until a terminator or \0
     * is found.
     * @param[in] delimiter contains character delimiters.
     * @param[in] skip contains a list of characters to be removed from the
     * \a outputBuffer. As a consequence, if a skipCharacter is also a
     * terminator, when consecutive instances of the same terminator are found
     * (e.g. ::A:::B:C, where : is the terminator), the terminator is skipped
     * until the next token is found (in the previous example, the first token
     * to be found would be A).
     * @param[out] token is the substring between delimiters (including delimeters if not in skip).
     * @param[out] saveTerminator if not NULL the found terminator (from the
     * terminator list) is stored in this variable.
     * @return false if no data is stored in the outputBuffer, true otherwise
     * (meaning that a token was found).
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    virtual bool GetToken(DynamicCString &     token,
                          CCString const       delimiters,
                          char8 &              saveTerminator,
						  CCString const       skip = NULL_PTR(const char8 *));


    /**
     * @brief Reads a token from the stream into another stream.
     * @details Extracts a token from the stream until a terminator or \0 is
     * found.
     * @param[in] delimiter contains character delimiters.
     * @param[in] skip contains a list of characters to be removed from the
     * \a outputBuffer. As a consequence, if a skipCharacter is also a
     * terminator, when consecutive instances of the same terminator are found
     * (e.g. ::A:::B:C, where : is the terminator), the terminator is skipped
     * until the next token is found (in the previous example, the first token
     * to be found would be A).
     * @param[out] token is the substring between delimiters (including delimeters if not in skip).
     * @param[out] saveTerminator if not NULL the found terminator (from the
     * terminator list) is stored in this variable.
     * @return false if no data is stored in the outputBuffer, true otherwise
     * (meaning that a token was found).
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool GetToken(       BufferedStreamI&     token,
                         CCString const       delimiters,
                         char8 &              saveTerminator,
      		             CCString const       skip = NULL_PTR(const char8 *));

    /**
     * @brief Skips a series of tokens delimited by terminators or \0.
     * @details The functionality skipCharacters=terminator (see GetToken) is
     * automatically set, so that consecutive instances of the same terminator
     * @param[in] count the number of tokens to be skipped.
     * @param[in] delimiter contains character delimiters.
     * @return false if no data is read, true (meaning that at least a token
     * was found).
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool SkipTokens(const uint32 count, CCString const       delimiters);

    /**
     * @brief Extracts a line from this stream into another stream.
     * @param[out] output is the output stream where the line must be written.
     * @param[in] skipTerminators if true the "\r" is skipped.
     * @return true if a line is successfully read from this stream and written
     * into \a output.
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool GetLine(BufferedStreamI & output,  bool skipTerminators = true);

    /**
     * @brief Extracts a line from this stream into a character buffer.
     * @param[out] line is the character buffer where the line must be
     * written into.
     * @param[in] skipTerminators if true the "\r" is skipped.
     * @return true if a line is successfully read from this stream and
     * written into \a outputBuffer.
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool GetLine(DynamicCString &     line, bool skipTerminators = true);

    /**
     * @brief Printf implementation.
     * @see FormatDescriptor::InitialiseFromString
     *
     * @details When \a pars wraps a multi-dimensional array of AnyTypes the \a format
     * will be applied to each scalar element. The output will be
     *
     * - scalar : "element";\n
     * - vector : "{ element_1 element_2 ... } ";\n
     * - matrix : "{ { element_1_1 element_1_2 ... } { element_2_1 element_2_2 ... } } ";\n
     *
     * @details When \a pars wraps classes or structures inputs, the object to be printed
     * must be introspectable and registered into the ClassRegistryDatabase. The output
     * is the one from the MARTe standard language. An example is:
     *
     * Class = "the class name"\n
     * member_1 = 'the member value'\n
     * member_2 = ...\n
     * ...\n
     * member_n = {\n
     *    Class = "the nested class name"\n
     *    ...\n
     * }\n
     *
     * @details When \a pars wraps introspection information about a class or structure ( \a format = %? ),
     * the output will respect the MARTe standard language and the grammar will be compatible with the parser used
     * to create the C code to automatically declare and register a class with its Introspection from
     * a configuration stream. An Example is:
     *
     * class_name = {\n
     *      first_member_name = {\n
     *          type = "the member type"\n
     *          modifiers = "the member modifiers"\n
     *          attributes = "the member attributes"\n
     *      }\n
     *      second_member_name = {\n
     *          ...\n
     *      }\n
     *      ...\n
     *  }
     *
     * @details When \a pars describes a StructuredDataI (i.e ConfigurationDatabase), the
     * output will respect the MARTe standard language and the grammar will be compatible
     * with the StandardParser used to generate a StructuredDataI from a configuration
     * stream.
     *
     * @param[in] format printf format as specified in FormatDescriptor::InitialiseFromString.
     * @param[in] pars the list of elements that are to be replaced in the
     * \a format string. It must be terminated by a voidAnyType element
     * (i.e. pars[length(pars)-1] == voidAnyType).
     * @return true if the string is successfully printed.
     * @pre CanWrite() && GetWriteBuffer() != NULL
     * @post see brief
     */
    bool PrintFormatted(CCString const format,  const AnyType pars[]);

    /**
     * @brief Copies a character buffer.
     * @detail Copies a character buffer into this stream (from the
     * current position).
     * @param[in] buffer is the buffer to be copied into the stream.
     * @return true if buffer is successfully copied into the stream.
     * @pre CanWrite() && GetWriteBuffer() != NULL
     * @post see brief
     */
    bool Copy(CCString buffer);

    /**
     * @brief Copies from a stream.
     * @detail Copies a stream into this stream (from the current
     * position).
     * @param[in,out] stream is the stream to be copied into the stream.
     * @return true if stream is successfully copied into the stream.
     * @pre CanWrite() && GetWriteBuffer() != NULL
     * @post see brief
     */
    bool Copy(BufferedStreamI &stream);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(CCString const format, const AnyType& par1);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(CCString const format,
                       const AnyType& par1,
                       const AnyType& par2);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(CCString const format,
                       const AnyType& par1,
                       const AnyType& par2,
                       const AnyType& par3);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(CCString const format,
                       const AnyType& par1,
                       const AnyType& par2,
                       const AnyType& par3,
                       const AnyType& par4);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(CCString const format,
                       const AnyType& par1,
                       const AnyType& par2,
                       const AnyType& par3,
                       const AnyType& par4,
                       const AnyType& par5);

protected:

    /**
     * @brief Gets the read buffer.s
     * @return a pointer to the read buffer.
     */
    virtual IOBuffer *GetReadBuffer() = 0;

    /**
     * @brief Gets the write buffer.
     * @return a pointer to the write buffer.
     */
    virtual IOBuffer *GetWriteBuffer() = 0;

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//BufferedStreamI::operator AnyType() {
//    void *dataPointer = static_cast<void *>(this);

//    return AnyType(StreamType, dataPointer);
//}

bool BufferedStreamI::Printf(CCString format, const AnyType& par1) {
    AnyType pars[2] = { par1, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool BufferedStreamI::Printf(CCString const format,
                             const AnyType& par1,
                             const AnyType& par2) {
    AnyType pars[3] = { par1, par2, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool BufferedStreamI::Printf(CCString const format,
                             const AnyType& par1,
                             const AnyType& par2,
                             const AnyType& par3) {
    AnyType pars[4] = { par1, par2, par3, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool BufferedStreamI::Printf(CCString const format,
                             const AnyType& par1,
                             const AnyType& par2,
                             const AnyType& par3,
                             const AnyType& par4) {
    AnyType pars[5] = { par1, par2, par3, par4, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool BufferedStreamI::Printf(CCString const format,
                             const AnyType& par1,
                             const AnyType& par2,
                             const AnyType& par3,
                             const AnyType& par4,
                             const AnyType& par5) {
    AnyType pars[6] = { par1, par2, par3, par4, par5, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

}
#endif /* BUFFEREDSTREAMI_H_ */

