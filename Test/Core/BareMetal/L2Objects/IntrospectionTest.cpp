/**
 * @file IntrospectionTest.cpp
 * @brief Source file for class IntrospectionTest
 * @date 18/01/2016
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
 * the class IntrospectionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IntrospectionTest.h"
#include "StringHelper.h"
#include "Object.h"
#include "ClassRegistryDatabase.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct TestIntrospectionNestedStructure {
    uint32 nestedMember1;
};

struct TestIntrospectionStructure {
    uint32 member1;
    float32 *member2;
    float64 member3[32];
    const char8 * const member4;
    TestIntrospectionNestedStructure member5;
};

static Introspection &Init() {

    DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructure, nestedMember1, uint32, false, "");

    static const IntrospectionEntry* nestedFields[] = { &TestIntrospectionNestedStructure_nestedMember1_introspectionEntry, 0 };

    static Introspection nestedIntrospection(nestedFields);
    INTROSPECTION_CLASS_REGISTER(TestIntrospectionNestedStructure, "1.0", nestedIntrospection)

    static IntrospectionEntry member1Field("member1", "uint32", false, "", introspectionMemberSize(TestIntrospectionStructure, member1),
                                           introspectionMemberIndex(TestIntrospectionStructure, member1));

    DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member2, float32, false, "*");

    DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member3, float64, false, "32");

    DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member4, string, true, "C");

    DECLARE_CLASS_MEMBER(TestIntrospectionStructure, member5, TestIntrospectionNestedStructure, false, "");

    static const IntrospectionEntry* fields[] = { &member1Field, &TestIntrospectionStructure_member2_introspectionEntry,
            &TestIntrospectionStructure_member3_introspectionEntry, &TestIntrospectionStructure_member4_introspectionEntry,
            &TestIntrospectionStructure_member5_introspectionEntry, 0 };

    static Introspection testIntrospection(fields);
    INTROSPECTION_CLASS_REGISTER(TestIntrospectionStructure, "1.0", testIntrospection)

    return testIntrospection;
}

class Initializer {
public:
    Initializer() {
        Init();
    }
};

static Initializer initializer;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool IntrospectionTest::TestConstructor() {

    ////// check member 1

    const IntrospectionEntry member1Copy = Init()[0];
    if (StringHelper::Compare(member1Copy.GetMemberName(), "member1") != 0) {
        return false;
    }

    if (member1Copy.IsConstant()) {
        return false;
    }

    if (member1Copy.GetMemberTypeDescriptor().type != UnsignedInteger) {
        return false;
    }

    if (member1Copy.GetMemberTypeDescriptor().numberOfBits != 32) {
        return false;
    }

    if (StringHelper::Compare(member1Copy.GetMemberModifiers(), "") != 0) {
        return false;
    }

    if (member1Copy.GetMemberByteOffset() != 0) {
        return false;
    }

    if (member1Copy.GetMemberSize() != 4) {
        return false;
    }

    //////// check member2

    const IntrospectionEntry member2Copy = Init()[1];
    if (StringHelper::Compare(member2Copy.GetMemberName(), "member2") != 0) {
        printf("\n1\n");
        return false;
    }

    if (member2Copy.IsConstant()) {
        printf("\n2\n");
        return false;
    }

    if (member2Copy.GetMemberTypeDescriptor().type != Float) {
        printf("\n3\n");
        return false;
    }

    if (member2Copy.GetMemberTypeDescriptor().numberOfBits != 32) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(member2Copy.GetMemberModifiers(), "*") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member2Copy.GetMemberSize() != sizeof(float32*)) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member2Copy.GetMemberByteOffset() != sizeof(float32*)) {
        printf("\n6 %d\n", member2Copy.GetMemberByteOffset());
        return false;
    }
    return true;
}

bool IntrospectionTest::TestPositionOperator() {

    //////// check member3
    const IntrospectionEntry member3Copy = Init()[2];
    if (StringHelper::Compare(member3Copy.GetMemberName(), "member3") != 0) {
        printf("\n1\n");
        return false;
    }

    if (member3Copy.IsConstant()) {
        printf("\n2\n");
        return false;
    }

    if (member3Copy.GetMemberTypeDescriptor().type != Float) {
        printf("\n3\n");
        return false;
    }

    if (member3Copy.GetMemberTypeDescriptor().numberOfBits != 64) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(member3Copy.GetMemberModifiers(), "32") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member3Copy.GetMemberSize() != sizeof(float64) * 32) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member3Copy.GetMemberByteOffset() != 2 * sizeof(float32*)) {
        printf("\n6 %d\n", member3Copy.GetMemberByteOffset());
        return false;
    }
    return true;
}

bool IntrospectionTest::TestMacroToAddBasicInClassRegistryDatabase() {
    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("TestIntrospectionStructure");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry member4Copy = (*introInfo->GetIntrospection())[3];
    if (StringHelper::Compare(member4Copy.GetMemberName(), "member4") != 0) {
        printf("\n1\n");
        return false;
    }

    if (!member4Copy.IsConstant()) {
        printf("\n2\n");
        return false;
    }

    if (member4Copy.GetMemberTypeDescriptor().type != CCString) {
        printf("\n3\n");
        return false;
    }

    if (member4Copy.GetMemberTypeDescriptor().numberOfBits != sizeof(const char8*) * 8) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(member4Copy.GetMemberModifiers(), "C") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member4Copy.GetMemberSize() != sizeof(const char8*)) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member4Copy.GetMemberByteOffset() != 2 * sizeof(float32*) + 32 * sizeof(float64)) {
        printf("\n6 %d\n", member4Copy.GetMemberByteOffset());
        return false;
    }
    return true;

}

bool IntrospectionTest::TestMacroToAddStructuredInClassRegistryDatabase() {

    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("TestIntrospectionStructure");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry member4Copy = (*introInfo->GetIntrospection())[4];
    if (StringHelper::Compare(member4Copy.GetMemberName(), "member5") != 0) {
        printf("\n1\n");
        return false;
    }

    if (member4Copy.IsConstant()) {
        printf("\n2\n");
        return false;
    }

    if (!member4Copy.GetMemberTypeDescriptor().isStructuredData) {
        printf("\n3\n");
        return false;
    }

    if (StringHelper::Compare(member4Copy.GetMemberModifiers(), "") != 0) {
        printf("\n5\n");
        return false;
    }

    if (member4Copy.GetMemberSize() != sizeof(TestIntrospectionNestedStructure)) {
        printf("\n5a\n");
        return false;
    }

    // the members are aligned !
    if (member4Copy.GetMemberByteOffset() != 2 * sizeof(float32*) + 32 * sizeof(float64) + sizeof(const char8*)) {
        printf("\n6 %d\n", member4Copy.GetMemberByteOffset());
        return false;
    }
    return true;

}