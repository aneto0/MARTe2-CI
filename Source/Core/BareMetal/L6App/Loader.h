/**
 * @file Loader.h
 * @brief Header file for class Loader
 * @date 4/4/2018
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class Loader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_LOADER_H_
#define L6APP_LOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "Message.h"
#include "ParserI.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO.
 *
 * @details TODO.
 *
 */
class DLL_API Loader: public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO
     */
Loader    ();

    /**
     * @brief TODO
     */
    virtual ~Loader();

    /**
     * @brief TODO
     */
    virtual ErrorManagement::ErrorType Initialise(StructuredDataI &data, StreamI &configuration);

    /**
     * @brief TODO
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief TODO
     */
    virtual ErrorManagement::ErrorType Stop();

protected:
    /**
     * @brief TODO
     */
    ConfigurationDatabase parsedConfiguration;

private:
    /**
     * @brief TODO
     */
    StreamString messageDestination;

    /**
     * @brief TODO
     */
    StreamString messageFunction;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_APPLICATION_H_ */

