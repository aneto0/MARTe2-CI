/**
 * @file GAM.h
 * @brief Header file for class GAM
 * @date 18/02/2016
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

 * @details This header file contains the declaration of the class GAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5GAMS_GAM_H_
#define L5GAMS_GAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "StructuredDataI.h"
#include "StreamString.h"
#include "ReferenceT.h"
#include "RealTimeState.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The Generic Acquisition Module.
 * @details In order to allow custom implementations, some of the methods
 * of this class are pure virtual. A generic derived class should have
 * specific IO structures and their definitions (see RealTimeDataDef)
 * can be expressed in the global configuration stream and / or in the local
 * configuration stream.
 *
 * @details The Initialise method (which can be override) configures the GAM reading
 * from the global configuration stream. If the definitions of the input and output
 * structures is incomplete (or also empty), it is possible use the method Verify(*) passing the
 * local configuration stream and completing all the definitions.
 */
class GAM: public ReferenceContainer {
public:

    /**
     * @brief Constructor
     */
    GAM();

    /**
     * @brief Destructor
     * @details Frees the array containing the supported states names.
     */
    virtual ~GAM();


    /**
     * @brief Generates the data source definitions ( in RealTimeApplication_name.+Data ) looking to the path
     * of each RealTimeDataDefI declared into this GAM.
     */
    virtual bool ConfigureDataSource();

    /**
     * @brief Links this GAM to its RealTimeApplication.
     * @param[in] rtApp is a reference to the RealTimeApplication where this GAM is declared into.
     */
    void SetApplication(ReferenceT<RealTimeApplication> rtApp);

    /**
     * @brief Links this GAM to its GAMGroup.
     * @param[in] gamGroup is the GAMGroup involving this GAM.
     */
    void SetGAMGroup(ReferenceT<GAMGroup> gamGroup);


    /**
     * @brief Adds the name of a RealTimeState where this GAM is declared into.
     * @param[in] stateName is the RealTimeState name.
     */
    void AddState(const char8 *stateName);

    /**
     * @brief The core function to be executed.
     * @param[in] activeContextBuffer is the context buffer currently active.
     */
    virtual void Execute(uint8 activeContextBuffer)=0;

    /**
     * @brief calls the Initialise(*) function for each sub-node, then calls the functions
     * SetUp(*) and ConfigureFunction(*) due to initialise the local environment.
     */
    virtual bool Initialise(StructuredDataI & data);


    /**
     * @brief Retrieves the states names where this class is declared into
     */
    StreamString *GetSupportedStates() ;

    /**
     * @brief Returns the number of the supported states.
     * @return the number of the supported states.
     */
    uint32 GetNumberOfSupportedStates() ;


protected:

    /**
     * @brief Setup the GAM.
     * @details Initialises the local status (memory allocation
     * of the IO structures, local configuration file, ecc)
     */
    virtual void SetUp()=0;

    /**
     * The names of the supported states
     */
    StreamString *supportedStates;

    /**
     * How many supported states
     */
    uint32 numberOfSupportedStates;


    /**
     * The local configuration
     */
    StructuredDataI* localData;

    /**
     * Link to the RealTimeApplication
     */
    ReferenceT<RealTimeApplication> application;

    /**
     * Link to the GAMGroup
     */
    ReferenceT<GAMGroup> group;
    //? IOData?
    //? context?

private:
    /**
     * @brief Completes the IO structure definitions (see RealTimeDataDefI) found in the global CDB
     * with the definitions in the local CDB and check their consistency.
     * @param[in] localData is the local StructuredData.
     * @return false in case of conflicts between the local and the global definitions, or
     * if the definitions are inconsistent with registered types. True otherwise.
     */
    bool ConfigureFunction();


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_GAM_H_ */

