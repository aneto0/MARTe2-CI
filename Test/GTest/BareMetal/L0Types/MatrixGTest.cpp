/**
 * @file MatrixGTest.cpp
 * @brief Source file for class MatrixGTest
 * @date 17/11/2015
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

 * @details This source file contains the definition of all the methods for
 * the class MatrixGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"
#include "MatrixTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(MatrixGTest,TestDefaultConstructor) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestDefaultConstructor());
}

TEST(MatrixGTest,TestConstructorOnHeap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestConstructorOnHeap());
}

TEST(MatrixGTest,TestConstructorByPointerStatic_Int8) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int8 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_UInt8) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint8 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_Int16) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int16 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_UInt16) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint16 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_Int32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int32 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_UInt32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_Int64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_UInt64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_Float32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    float32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j) + 0.956 * i - 1.112 * j;
        }
    }
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByPointerStatic_Float64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    float64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j) + 0.956 * i - 1.112 * j;
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic((int32*)&matrix[0][0], nRows, nCols));
}

TEST(MatrixGTest,TestConstructorByTable_Int8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int8 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_UInt8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint8 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_Int16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int16 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_UInt16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint16 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_Int32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int32 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_UInt32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint32 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_Int64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int64 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_UInt64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint64 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_Float32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float32 matrix[][nCols] = { { 1.99, -0.002 }, { -399E+3, 4.0099 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestConstructorByTable_Float64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float64 matrix[][nCols] = { { 1.99, -0.002 }, { -399E+3, 4.0099 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(MatrixGTest,TestGetNumberOfColumns_32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfColumns(32));
}

TEST(MatrixGTest,TestGetNumberOfColumns_0) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfColumns(0));
}

TEST(MatrixGTest,TestGetNumberOfRows_32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfRows(32));
}

TEST(MatrixGTest,TestGetNumberOfRows_0) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfRows(0));
}

TEST(MatrixGTest,TestMatrixOperator_Static) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixOperator_Static());
}

TEST(MatrixGTest,TestMatrixOperator_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixOperator_Heap());
}

TEST(MatrixGTest,TestMatrixFunctionCallOperator_Static) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixFunctionCallOperator_Static());
}

TEST(MatrixGTest,TestMatrixFunctionCallOperator_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixFunctionCallOperator_Heap());
}

TEST(MatrixGTest,TestGetDataPointer) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetDataPointer());
}

TEST(MatrixGTest,TestProduct) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestProduct());
}

TEST(MatrixGTest,TestSum) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSum());
}

TEST(MatrixGTest,TestCopy) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopy());
}

TEST(MatrixGTest,TestSubMatrix) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSubMatrix());
}

TEST(MatrixGTest,TestDeterminant) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestDeterminant());
}

TEST(MatrixGTest,TestTranspose) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestTranspose());
}

TEST(MatrixGTest,TestInverse_float32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float32());
}

TEST(MatrixGTest,TestInverse_float64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float64());
}

TEST(MatrixGTest,TestInverse_NullDeterminant) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_NullDeterminant());
}

TEST(MatrixGTest,TestProduct_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestProduct_Heap());
}

TEST(MatrixGTest,TestSum_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSum_Heap());
}

TEST(MatrixGTest,TestCopy_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopy_Heap());
}

TEST(MatrixGTest,TestSubMatrix_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSubMatrix_Heap());
}

TEST(MatrixGTest,TestDeterminant_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestDeterminant_Heap());
}

TEST(MatrixGTest,TestTranspose_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestTranspose_Heap());
}

TEST(MatrixGTest,TestInverse_float32_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float32_Heap());
}

TEST(MatrixGTest,TestInverse_float64_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float64_Heap());
}




