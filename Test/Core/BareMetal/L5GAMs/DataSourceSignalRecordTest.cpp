/**
 * @file DataSourceSignalRecordTest.cpp
 * @brief Source file for class DataSourceSignalRecordTest
 * @date 13/apr/2016
 * @author pc
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
 * the class DataSourceSignalRecordTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceSignalRecordTest.h"
#include "GAMTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool DataSourceSignalRecordTest::TestConstructor() {
    DataSourceSignalRecord record;
    Reference consumers = record.Find("Consumers");
    Reference producers = record.Find("Producers");

    return consumers.IsValid() && producers.IsValid();
}


bool DataSourceSignalRecordTest::TestAddConsumer() {
    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam1->SetName("gam1");

    DataSourceSignalRecord record;
    if (!record.AddConsumer(gam1)) {
        return false;
    }

    return (record.GetConsumers()->Size() == 1) && (record.GetConsumers()->Get(0) == gam1);
}

bool DataSourceSignalRecordTest::TestAddProducer() {
    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam1->SetName("gam1");

    DataSourceSignalRecord record;
    if (!record.AddProducer(gam1)) {
        return false;
    }

    return (record.GetProducers()->Size() == 1) && (record.GetProducers()->Get(0) == gam1);

}

bool DataSourceSignalRecordTest::TestGetNumberOfConsumers(uint32 nConsumers) {
    DataSourceSignalRecord record;
    for (uint32 i = 0u; i < nConsumers; i++) {
        ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        record.AddConsumer(gam);
    }
    return record.GetNumberOfConsumers() == nConsumers;

}

bool DataSourceSignalRecordTest::TestGetNumberOfProducers(uint32 nProducers) {
    DataSourceSignalRecord record;
    for (uint32 i = 0u; i < nProducers; i++) {
        ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        record.AddProducer(gam);
    }
    return record.GetNumberOfProducers() == nProducers;
}

bool DataSourceSignalRecordTest::TestGetConsumers() {
    DataSourceSignalRecord record;
    const uint32 size = 32;
    ReferenceT<PIDGAM> gam[size];
    for (uint32 i = 0u; i < size; i++) {
        gam[i] = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        record.AddConsumer(gam[i]);
    }

    ReferenceT<ReferenceContainer> consumers = record.GetConsumers();

    for (uint32 i = 0u; i < size; i++) {
        if (consumers->Get(i) != gam[i]) {
            return false;
        }
    }
    return true;
}

bool DataSourceSignalRecordTest::TestGetProducers() {
    DataSourceSignalRecord record;
    const uint32 size = 32;
    ReferenceT<PIDGAM> gam[size];
    for (uint32 i = 0u; i < size; i++) {
        gam[i] = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        record.AddProducer(gam[i]);
    }

    ReferenceT<ReferenceContainer> producers = record.GetProducers();

    for (uint32 i = 0u; i < size; i++) {
        if (producers->Get(i) != gam[i]) {
            return false;
        }
    }
    return true;
}