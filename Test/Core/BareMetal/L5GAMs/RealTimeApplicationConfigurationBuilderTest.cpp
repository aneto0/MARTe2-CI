/**
 * @file RealTimeApplicationConfigurationBuilderTest.cpp
 * @brief Source file for class RealTimeApplicationConfigurationBuilderTest
 * @date 13/lug/2016
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
 * the class RealTimeApplicationConfigurationBuilderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplicationConfigurationBuilderTest.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "ErrorManagement.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static void MyErrorProcessFunction(const ErrorManagement::ErrorInformation &errorInfo,
                                   const char8 * const errorDescription) {
    printf("---->>%s\n", errorDescription);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
RealTimeApplicationConfigurationBuilderTest::RealTimeApplicationConfigurationBuilderTest() {
    ErrorManagement::SetErrorProcessFunction(&MyErrorProcessFunction);
}

bool RealTimeApplicationConfigurationBuilderTest::TestInitialiseSignalsDatabase() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestFlattenSignalsDatabases() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;

}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveDataSources() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveDataSourceFalse_TypeMismatch() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = A"
            "                    MemberAliases = {"
            "                        Signal1.a2 = SharedVar"
            "                    }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());
    return (!rtAppBuilder.ResolveDataSources());

}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveDataSourceFalse_NElementsMismatch() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    NumberOfDimensions = 2"
            "                    NumberOfElements = 1"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());
    return (!rtAppBuilder.ResolveDataSources());
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveDataSourceFalse_NDimensionsMismatch() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    NumberOfDimensions = 1"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    NumberOfDimensions = 0"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());
    return (!rtAppBuilder.ResolveDataSources());
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyDataSourcesSignals() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyDataSourcesSignals_PartialInPredefinedDsNoType() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = A"
            "                    Alias = Predefined"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = A"
            "                    Alias = Predefined"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
            "            Signals = {"
            "                Predefined = {"
            "                    NumberOfElements = 3"
            "                }"
            "            }"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());
    return true; //
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyDataSourcesSignalsFalse_NoTypeInPredefinedDs() {
    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = A"
            "                    Alias = Predefined"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = A"
            "                    Alias = Predefined"
            "                } "
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
            "            Signals = {"
            "                Empty = {"
            "                    NumberOfElements = 3"
            "                }"
            "            }"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());
    return (!rtAppBuilder.VerifyDataSourcesSignals());
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyDataSourcesSignalsFalse_PartialInPredefinedDsWithType() {
    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = A"
            "                    Alias = Shared"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = Shared"
            "                } "
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());
    return (!rtAppBuilder.VerifyDataSourcesSignals());
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveFunctionSignals() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveFunctionSignals2() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());
    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveFunctionSignalsFalse_NoTypeInGAMSignal() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Alias = X"
            "                    Type = int32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            // no add in resolveds phase then in resolvefn the signal is not found
            "                    DataSource = DDB1"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return (!rtAppBuilder.ResolveFunctionSignals());
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyFunctionSignals() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveStates() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveStatesFalse_SameGAMIn2Threads() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Alias = X"
            "                    Type = int32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {:Functions.GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return (!rtAppBuilder.ResolveStates());
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveStatesFalse_MoreThanOneSyncInAThread_SameGAM() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Alias = X"
            "                    Type = int32"
            "                    Frequency = 10"
            "                }"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Alias = Y"
            "                    Type = int32"
            "                    Frequency = 10"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Alias = X"
            "                    Type = int32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {:Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return (!rtAppBuilder.ResolveStates());
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveConsumersAndProducers() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyConsumersAndProducers() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyConsumersAndProducersFalse_TwoProducers() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return (!rtAppBuilder.VerifyConsumersAndProducers());
}

bool RealTimeApplicationConfigurationBuilderTest::TestVerifyConsumersAndProducersFalse_MemoryOverlap() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 32"
            "                    Ranges = {{0 10}{15 20}}"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 32"
            "                    Ranges = {{11 14}{20 31}}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return (!rtAppBuilder.VerifyConsumersAndProducers());
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveFunctionSignalsMemorySize() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignalsMemorySize()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxMin() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 32"
            "                    Ranges = {{0 10}{15 20}}"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 32"
            "                    Ranges = {{14 11}{21 31}}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return !rtAppBuilder.ResolveFunctionSignalsMemorySize();
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxNElements() {

    static StreamString configLocal = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 32"
            "                    Ranges = {{0 10}{15 20}}"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = int32"
            "                    Alias = SharedVar"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 32"
            "                    Ranges = {{11 14}{21 32}}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    configLocal.Seek(0);
    StandardParser parser(configLocal, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return !rtAppBuilder.ResolveFunctionSignalsMemorySize();
}

bool RealTimeApplicationConfigurationBuilderTest::TestResolveFunctionsMemory() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignalsMemorySize()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionsMemory()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestCalculateFunctionsMemory() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignalsMemorySize()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionsMemory()) {
        return false;
    }

    if (!rtAppBuilder.CalculateFunctionsMemory()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestAssignFunctionsMemoryToDataSource() {
    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignalsMemorySize()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionsMemory()) {
        return false;
    }

    if (!rtAppBuilder.CalculateFunctionsMemory()) {
        return false;
    }

    if (!rtAppBuilder.AssignFunctionsMemoryToDataSource()) {
        return false;
    }

    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestAssignBrokersToFunctions() {

    static StreamString config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = A"
            "                    MemberAliases = {"
            "                        Signal1.a1 = E"
            "                    }"
            "                    Defaults = {"
            "                        Signal1.a1.b1 = 1"
            "                        Signal1.a2 = 0.5"
            "                    }"
            "                }"
            "                Signal2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Samples = 1"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2.DDB1"
            "                }"
            "            }"
            "        }"
            "        +GAMC = {"
            "            Class = GAM1"
            "            OutputSignals = {"
            "                ToBoard1 = {"
            "                    Signal1 = {"
            "                        DataSource = DDB1"
            "                        Type = uint32"
            "                        Alias = SharedVar"
            "                        NumberOfDimensions = 2"
            "                        NumberOfElements = 32"
            "                        Ranges = {{0 0},{3 5}}"
            "                    }"
            "                }"
            "            }"
            "            +GAMA = {"
            "                Class = GAM1"
            "                OutputSignals = {"
            "                    Signal1 = {"
            "                        DataSource = DDB1"
            "                        Type = uint32"
            "                        Alias = SharedVar"
            "                        NumberOfDimensions = 2"
            "                        NumberOfElements = 32"
            "                        Ranges = {{1 2},{6 31}}"
            "                    }"
            "                }"
            "                InputSignals = {"
            "                    Signal2 = {"
            "                        DataSource = DDB2"
            "                        Alias = PredefinedSignal"
            "                    }"
            "                    Signal3 = {"
            "                        DataSource = DDB2"
            "                        Alias = PredefinedSignal.a1"
            "                    }"
            "                    Signal4 = {"
            "                        DataSource = DDB2"
            "                        Alias = PredefinedSignal.a2"
            "                    }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = DS1"
            "        }"
            "        +DDB2 = {"
            "            Class = DS1"
            "            +DDB1 = {"
            "                Class = DS1"
            "                Signals = {"
            "                    Signal1 = {"
            "                        Type = int32"
            "                        NumberOfDimensions = 1"
            "                        NumberOfElements = 32"
            "                    }"
            "                }"
            "            }"
            "            Signals = {"
            "                PredefinedSignal = {"
            "                    Type = A"
            "                }"
            "            }"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB, :Functions.GAMC}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {:Functions.GAMC.GAMA}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {:Functions.GAMA,:Functions.GAMC.GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAMC, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }
    RealTimeApplicationConfigurationBuilder rtAppBuilder(application, "DDB1");

    if (!rtAppBuilder.InitialiseSignalsDatabase()) {
        return false;
    }

    if (!rtAppBuilder.FlattenSignalsDatabases()) {
        return false;
    }

    if (!rtAppBuilder.ResolveDataSources()) {
        return false;
    }

    if (!rtAppBuilder.VerifyDataSourcesSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.VerifyFunctionSignals()) {
        return false;
    }

    if (!rtAppBuilder.ResolveStates()) {
        return false;
    }

    if (!rtAppBuilder.ResolveConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.VerifyConsumersAndProducers()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionSignalsMemorySize()) {
        return false;
    }

    if (!rtAppBuilder.ResolveFunctionsMemory()) {
        return false;
    }

    if (!rtAppBuilder.CalculateFunctionsMemory()) {
        return false;
    }

    if (!rtAppBuilder.AssignFunctionsMemoryToDataSource()) {
        return false;
    }

    if (!rtAppBuilder.AssignBrokersToFunctions()) {
        return false;
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;
    if (!rtAppBuilder.Copy(fcdb, dcdb)) {
        return false;
    }

    StreamString fDisplay;
    StreamString dDisplay;

    fDisplay.Printf("%!", fcdb);
    dDisplay.Printf("%!", dcdb);

    fDisplay.Seek(0);
    dDisplay.Seek(0);

    printf("\n%s", fDisplay.Buffer());
    printf("\n%s", dDisplay.Buffer());

    return true;
}

bool RealTimeApplicationConfigurationBuilderTest::TestAllocateGAMMemory() {

    static StreamString config1 = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +DDB2 = {"
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
            "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    config1.Seek(0);
    StandardParser parser(config1, cdb);

    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Fibonacci");
    if (!application.IsValid()) {
        return false;
    }

    if (!application->ConfigureApplication()) {
        return false;
    }
    if (!application->AllocateGAMMemory()) {
        return false;
    }
    if (!application->AllocateDataSourceMemory()) {
        return false;
    }

    if (!application->AddBrokersToFunctions()) {
        return false;
    }

    if (!application->PrepareNextState("State1")) {
        return false;
    }

    application->StartExecution();

    ReferenceT<GAM> gam1 = application->Find("Functions.GAMA");

    ReferenceT<GAM> gam2 = application->Find("Functions.GAMB");

    gam1->Execute();

    gam2->Execute();


    gam1->Execute();

    gam2->Execute();

    return true;
}

