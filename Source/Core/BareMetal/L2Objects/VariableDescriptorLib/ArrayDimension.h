/**
 * @file ArrayDimension.h
 * @brief Header file for class AnyType
 * @date 7 Oct 2018
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

 * @details This header file contains the specialization of Dimension for Arrays.
*/

#ifndef VD_ARRAYDIMENSION_H_
#define VD_ARRAYDIMENSION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Dimension.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace VariableDescriptorLib{

/**
 * 'A'
 */
class ArrayDimension: public Dimension{

public:

	/**
	 *
	 */
	ArrayDimension(uint32 numberOfElementsIn);

	/**
	 *
	 */
	virtual ~ArrayDimension();

	/**
	 * loops through layers stopping at a stack break.
	 * calculates combined size of all elements on this stack
	 * size of stack end * nOfEl each layer
	 */
	virtual DimensionSize GetLayerCombinedElementSize() const ;


	/**
	 * @see Dimension::UpdatePointerAndSizeEx().
	 */
	virtual ErrorManagement::ErrorType UpdatePointerAndSizeEx(
				const uint8 *&	ptr,
				uint32 &		numberOfColumns,
				uint32 &		numberOfRows) const;


	/**
	 * @see Dimension::NumberOfElements().
	 */
	virtual ErrorManagement::ErrorType GetNumberOfElements(uint32 &numberOfElements) const;

	/**
	 * @see Dimension::InitStack().
	 */
	virtual ErrorManagement::ErrorType InitStack(
				uint8 * 		ptr,
				DimensionSize	numberOfElements) const;

	/**
	 * @see Dimension::TypeSize().
	 */
	virtual uint32 TypeSize() const ;

	/**
	 * @see Dimension::TypeChar().
	 */
	virtual char8 TypeChar() const;

protected:
	/**
	 * number of elements in this layer
	 */
	uint32 			numberOfElements;
};



} //VariableDescriptorLib
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORLIB_VD_DIMENSION_H_ */
