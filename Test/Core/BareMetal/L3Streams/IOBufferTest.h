/**
 * @file IOBufferTest.h
 * @brief Header file for class IOBufferTest
 * @date 09/10/2015
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

 * @details This header file contains the declaration of the class IOBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef IOBUFFERTEST_H_
#define IOBUFFERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "IOBuffer.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
class IOBufferTest {

public:

    /**
     * @brief Tests if the attributes are initialized correctly (buffer position =0 , used size=0)
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the undo level is equal to the value passed by argument in the constructor.
     */
    bool TestFullConstructor(uint32 undoLevel);

    /**
     * @brief Tests if the destructor puts the internal buffer pointer to NULL.
     */
    bool TestDestructor();

    /**
     * @brief Tests if the size returned is the first granularity multiple greater than size.
     */
    bool TestBufferSize(uint32 size,
                        uint32 granularity);

    /**
     * @brief Tests if the max usable amount in a buffer is equal to the total size allocated minus the
     * reserved space at end specified.
     */
    bool TestMaxUsableAmount(uint32 size,
                             uint32 endSpace,
                             uint32 granularity);

    /**
     * @brief Tests if the amount left is equal to the space from the current position to the end of the usable memory.
     */
    bool TestAmountLeft(uint32 size,
                        uint32 endSpace,
                        uint32 granularity);

    /**
     * @brief Tests if the function returns the space from the current position to the end of the filled memory.
     */
    bool TestUsedAmountLeft(uint32 size,
                            uint32 endSpace,
                            uint32 granularity);

    /**
     * @brief Tests if the function returns the pointer to the begin of the buffer in read only mode.
     */
    bool TestBuffer();

    /**
     * @brief Tests if the function returns the pointer to the begin of the buffer with write access.
     */
    bool TestBufferReference();

    /**
     * @brief Tests if the function returns the current position in the buffer.
     */
    bool TestPosition();

    /**
     * @brief Tests if the function returns the amount of the filled memory area.
     */
    bool TestUsedSize(uint32 size,
                      uint32 endSpace,
                      uint32 granularity);

    /**
     * @brief Tests if the function returns the first 2**x value minor than the granularity value passed in the constructor.
     */
    bool TestAllocationGranularity(uint32 granularity,
                                   uint32 expected);

    /**
     * @brief Tests if the function returns the undoLevel attribute specified in the constructor (or 0 by default).
     */
    bool TestUndoLevel(uint32 undoLevel);


    bool TestCanWrite();

    /**
     * @brief Tests if the function allocates the space required.
     */
    bool TestSetBufferHeapMemory(uint32 size,
                                 uint32 endSpace,
                                 uint32 granularity);

    /**
     * @brief Tests the reallocation of the memory increasing it.
     */
    bool TestSetBufferHeapMemoryIncrease();

    /**
     * @brief Tests the reallocation of the memory decreasing it.
     */
    bool TestSetBufferHeapMemoryDecrease();

    /**
     * @brief Tests if the function assigns the buffer pointer to a preallocated read only memory buffer.
     */
    bool TestSetBufferReadOnlyReferencedMemory();

    /**
     * @brief Tests if the function assigns the buffer pointer to a preallocated memory buffer with write access.
     */
    bool TestSetBufferReferencedMemory();

    /**
     * @brief Tests if the function returns false.
     */
    bool TestResync();

    /**
     * @brief Test if the position changes correctly after a seek and if the function returns false if the specified position is greater than the
     * end of the filled memory.
     */
    bool TestSeek(uint32 usedSize,
                  uint32 seek,
                  bool expected);

    /**
     * @brief Tests if the position changes correctly after a relative seek and if the function returns false if the final position is minor than zero
     * or greater than the end of the filled memory. In this last case checks also if the position becomes the begin ore the end of the buffer.
     */
    bool TestRelativeSeek(uint32 initialPos,
                          int32 delta,
                          bool expected);

    /**
     * @brief Tests if the function adds a character on a buffer created on heap.
     */
    bool TestPutC_Heap();

    /**
     * @brief Tests if the function adds a character on a pre existent buffer.
     */
    bool TestPutC_MemoryReference();

    /**
     * @brief Tests if the function returns true in case of buffer with only read only access.
     */
    bool TestPutC_MemoryReferenceRO();

    /**
     * @brief Tests if the function returns the character at the current position on a buffer created on heap.
     */
    bool TestGetC_Heap();

    /**
     * @brief Tests if the function returns the character at the current position on a pre existent buffer.
     */
    bool TestGetC_MemoryReference();

    /**
     * @brief Tests if the function returns the character at the current position on a pre existent buffer with only read only access.
     */
    bool TestGetC_MemoryReferenceRO();

    /**
     * @brief Tests if the function decrements the position and, in case of position=(end of filled memory) the function decrements also the used size attribute.
     */
    bool TestUnPutC();

    /**
     * @brief Tests if the function decrements the position.
     */
    bool TestUnGetC();

    /**
     * @brief Tests if the function sets the used size if the input is minor than the maximum usable amount of memory.
     */
    bool TestSetUsedSize(uint32 allocatedSize,
                         uint32 desiredSize);

    /**
     * @brief Tests if the function writes an input buffer on a buffer created on heap.
     */
    bool TestWrite_Heap(uint32 allocationSize,
                        uint32 writeSize,
                        const char8 *string);

    /**
     * @brief Tests if the function writes an input buffer on a pre-existent buffer.
     */
    bool TestWrite_Memoryreference();

    /**
     * @brief Tests if the function returns false because the buffer has read only access.
     */
    bool TestWrite_MemoryreferenceRO();

    /**
     * @brief Tests if the function returns false in case of null input buffer.
     */
    bool TestWrite_NULL_Buffer();

    /**
     * @brief Tests if the function reads correctly the specified number of bytes from a buffer created on heap.
     */
    bool TestRead_Heap(uint32 allocationSize,
                       uint32 readSize);

    /**
     * @brief Tests if the function reads correctly the specified number of bytes from a pre existent buffer.
     */
    bool TestRead_MemoryReference();

    /**
     * @brief Tests if the function returns false in case of null output buffer.
     */
    bool TestRead_NULL_Buffer();

    /**
     * @see TestWrite_Heap
     */
    bool TestWriteAll(uint32 allocationSize,
                      uint32 writeSize,
                      const char8* string);

    /**
     * @brief Tests if the function resets the attributes to the default value (position=0, used size=0).
     */
    bool TestEmpty();

    /**
     * @brief Tests the PrintFormatted against any of the tables defined in StreamTestHelper.
     */
    bool TestPrintFormatted(uint32 allocationGranularity, const PrintfNode table[]);

    /**
     * @brief Tests the print of a C string.
     */
    bool TestPrintFormatted_CCString();

    /**
     * @brief Tests the print of a pointer.
     */
    bool TestPrintFormatted_Pointer();

    /**
     * @brief Tests the print of a stream.
     */
    bool TestPrintFormatted_Stream();

    /**
     * Tests the limit of 10000 characters for a print of a stream.
     */
    bool TestPrintFormatted_TooBigStream();

    /**
     * Tests if it is printed an erro message in case of not seekable stream.
     */
    bool TestPrintFormatted_Stream_NotSeekable();

    /**
     * @brief Tests the print of a unsigned bit set.
     */
    bool TestPrintFormatted_BitSet_Unsigned();

    /**
     * @brief Tests the print of a signed bit set.
     */
    bool TestPrintFormatted_BitSet_Signed();

    /**
     * @brief Tests if the function gets the correct token passing a C string buffer as output.
     */
    bool TestGetToken_ConstCharOutput(const TokenTestTableRow *table);

    /**
     * @brief Tests if the token is clipped when the output buffer is too short.
     */
    bool GetToken_ConstCharOutput_ClipSize();

    /**
     * @brief Tests if the function gets the correct token passing another IOBuffer as output.
     */
    bool TestGetToken_IOBufferOutput(const TokenTestTableRow *table);

    /**
     * @brief Tests if the function skips correctly the tokens.
     */
    bool TestSkipToken(const SkipTokensTestTableRow *table);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* IOBUFFERTEST_H_ */

