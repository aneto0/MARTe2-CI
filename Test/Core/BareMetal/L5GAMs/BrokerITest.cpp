/**
 * @file BrokerITest.cpp
 * @brief Source file for class BrokerITest
 * @date 27/07/2016
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

 * @details This source file contains the definition of all the methods for
 * the class BrokerITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "BrokerITest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class BrokerITestHelper: public BrokerI {
public:
    CLASS_REGISTER_DECLARATION()virtual bool Init(SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void *gamMemoryAddress);

    virtual bool Execute() {
        return true;
    }
};

bool BrokerITestHelper::Init(SignalDirection direction,
                             DataSourceI &dataSourceIn,
                             const char8 * const functionName,
                             void *gamMemoryAddress) {
    return InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);
}
CLASS_REGISTER(BrokerITestHelper, "1.0");

class BrokerIDataSourceTestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

BrokerIDataSourceTestHelper    ();

    virtual ~BrokerIDataSourceTestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx,
            uint32 bufferIdx,
            void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            SignalDirection direction);

    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* functionName,
            void * gamMemPtr);

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* functionName,
            void * gamMemPtr);

};

BrokerIDataSourceTestHelper::BrokerIDataSourceTestHelper() :
        DataSourceI() {

}

BrokerIDataSourceTestHelper::~BrokerIDataSourceTestHelper() {

}

bool BrokerIDataSourceTestHelper::AllocateMemory() {
    return true;
}

uint32 BrokerIDataSourceTestHelper::GetNumberOfMemoryBuffers() {
    return 0u;
}

bool BrokerIDataSourceTestHelper::GetSignalMemoryBuffer(uint32 signalIdx,
                                                        uint32 bufferIdx,
                                                        void *&signalAddress) {
    return true;
}

const char8 *BrokerIDataSourceTestHelper::GetBrokerName(StructuredDataI &data,
                                                        SignalDirection direction) {
    if (direction == InputSignals) {
        return "BrokerITestHelper";
    }
    return "BrokerITestHelper";
}

bool BrokerIDataSourceTestHelper::PrepareNextState(const RealTimeStateInfo &status) {
    return true;
}

bool BrokerIDataSourceTestHelper::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                  const char8* functionName,
                                                  void * gamMemPtr) {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool BrokerIDataSourceTestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                   const char8* functionName,
                                                   void * gamMemPtr) {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}
CLASS_REGISTER(BrokerIDataSourceTestHelper, "1.0");

static bool InitialiseBrokerIEnviroment(const char8 * const config) {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    parser.Parse();

    ObjectRegistryDatabase::Instance()->CleanUp();

    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");

    return application->ConfigureApplication();
}

static const char8 * const testConfig1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "                   Frequency = 5.0"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                    DataSource = Drv1"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                    Default = \"{3 6 9 12 15 18}\""
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = BrokerIDataSourceTestHelper"
        "            Signals = {"
        "                Signal1A = {"
        "                    Type = uint32"
        "                }"
        "                Signal2 = {"
        "                    Type = float32"
        "                }"
        "                Signal3 = {"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                }"
        "            }"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMC :Functions.GAMA :Functions.GAMF}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMD :Functions.GAMB :Functions.GAME :Functions.GAMF}"
        "                }"
        "            }"
        "        }"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BrokerITest::TestConstructor() {
    ReferenceT<BrokerITestHelper> broker("BrokerITestHelper");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->GetNumberOfCopies() == 0;
    }
    return ret;
}

bool BrokerITest::TestInitFunctionPointers() {
    bool ret = InitialiseBrokerIEnviroment(testConfig1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> broker;
    ReferenceContainer brokers;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetInputBrokers(brokers, "GAMA", (void *) 0xA0A0A0A0);
    }
    if (ret) {
        ret = (brokers.Size() > 0u);
    }
    if (ret) {
        broker = brokers.Get(0);
        ret = broker.IsValid();
    }

    return ret;
}

bool BrokerITest::TestGetNumberOfCopies() {
    bool ret = InitialiseBrokerIEnviroment(testConfig1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 numberOfCopiesInput[] = { 5, 1, 0, 0, 1, 0 };
    uint32 numberOfCopiesOutput[] = { 0, 0, 5, 1, 0, 1 };
    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 numberOfCopies = inputBroker->GetNumberOfCopies();
            ret = (numberOfCopies == numberOfCopiesInput[n]);
        }
        if (ret) {
            uint32 numberOfCopies = outputBroker->GetNumberOfCopies();
            ret = (numberOfCopies == numberOfCopiesOutput[n]);
        }
    }
    return ret;
}
#include <stdio.h>
bool BrokerITest::TestGetCopyByteSize() {
    bool ret = InitialiseBrokerIEnviroment(testConfig1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 5;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 copyByteSizeInput[][maxNumberOfCopyByteSize] = { { 4, 16, 4, 12, 4 }, { 4, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 4, 0, 0, 0, 0 }, {
            0, 0, 0, 0, 0 } };
    uint32 copyByteSizeOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 4, 4, 16, 4, 12 }, { 4, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, {
            24, 0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 byteSize;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                byteSize = inputBroker->GetCopyByteSize(c);
                ret = (byteSize == copyByteSizeInput[n][c]);
            }
        }
        if (ret) {
            uint32 byteSize;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                byteSize = outputBroker->GetCopyByteSize(c);
                ret = (byteSize == copyByteSizeOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetCopyOffset() {
    bool ret = InitialiseBrokerIEnviroment(testConfig1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 5;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 copyOffsetInput[][maxNumberOfCopyByteSize] = { { 0, 8, 36, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0,
            0, 0, 0, 0 } };
    uint32 copyOffsetOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 8, 36, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0,
            0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            uint32 offset;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                offset = inputBroker->GetCopyOffset(c);
                ret = (offset == copyOffsetInput[n][c]);
            }
        }
        if (ret) {
            uint32 offset;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                offset = outputBroker->GetCopyOffset(c);
                ret = (offset == copyOffsetOutput[n][c]);
            }
        }
    }
    return ret;
}

bool BrokerITest::TestGetFunctionPointer() {
    bool ret = InitialiseBrokerIEnviroment(testConfig1);
    ReferenceT<BrokerIDataSourceTestHelper> dataSource;
    ReferenceT<BrokerITestHelper> inputBroker;
    ReferenceT<BrokerITestHelper> outputBroker;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    uint32 numberOfFunctions = 6;
    const uint32 maxNumberOfCopyByteSize = 5;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };

    const char *basePointer = reinterpret_cast<const char *>(0xA);
    const char *pointerInput[][maxNumberOfCopyByteSize] = { { basePointer, basePointer + 4, basePointer + 20, basePointer + 24, basePointer + 40 }, { basePointer, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { basePointer, 0, 0, 0, 0 }, { 0,
            0, 0, 0, 0 } };

    const char *pointerOutput[][maxNumberOfCopyByteSize] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { basePointer, basePointer + 8, basePointer + 12, basePointer + 28, basePointer + 32 }, { basePointer, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { basePointer,
            0, 0, 0, 0 } };

    uint32 n;
    for (n = 0; (n < numberOfFunctions) && (ret); n++) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        if (ret) {
            ret = dataSource->GetInputBrokers(inputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = dataSource->GetOutputBrokers(outputBrokers, functionNames[n], (void *) 0xA);
        }
        if (ret) {
            ret = (inputBrokers.Size() > 0u);
        }
        if (ret) {
            ret = (outputBrokers.Size() > 0u);
        }
        if (ret) {
            inputBroker = inputBrokers.Get(0);
            ret = inputBroker.IsValid();
        }
        if (ret) {
            outputBroker = outputBrokers.Get(0);
            ret = outputBroker.IsValid();
        }
        if (ret) {
            void * functionPointer;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                functionPointer = inputBroker->GetFunctionPointer(c);
                ret = (functionPointer == pointerInput[n][c]);
            }
        }
        if (ret) {
            void * functionPointer;
            uint32 c;
            for (c = 0u; (c < maxNumberOfCopyByteSize) && (ret); c++) {
                functionPointer = outputBroker->GetFunctionPointer(c);
                ret = (functionPointer == pointerOutput[n][c]);
            }
        }
    }
    return ret;
}