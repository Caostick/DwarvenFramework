#include <catch2/catch.hpp>

#include <DwarvenCore/DataVisitor.h>
#include <DwarvenCore/New.h>
#include <DwarvenCore/String.h>

namespace { 
    struct TestStructure {
        bool m_A;
        int m_B;
        df::String m_String;
    };
}

namespace df {
    template<>
    struct BufferVisitLogic<String> {
        static void Visit(DataVisitor* visitor, String& value) {
            size_t size = value.size();
            visitor->Visit(size);
            value.resize(size);
            visitor->VisitBuffer((uint8*)value.data(), size);
        }
    };

    template<>
    struct BufferVisitLogic<TestStructure> {
        static void Visit(DataVisitor* visitor, TestStructure& value) {
            visitor->Visit(value.m_A);
            visitor->Visit(value.m_B);
            visitor->Visit(value.m_String);
        }
    };
}

TEST_CASE("DataVisitor") {

    uint8* data = DFNew uint8[1024];

    bool boolA = true;
    unsigned int intA = 6;
    TestStructure structA;
    structA.m_A = true;
    structA.m_B = 123;
    structA.m_String = "Hello, test!";

    {
        df::WriteDataVisitor visitor(data);
        visitor.Visit(boolA);
        visitor.Visit(intA);
        visitor.Visit(structA);
    }


    bool boolB = false;
    unsigned int intB = 0;
    TestStructure structB;

    {
        df::ReadDataVisitor visitor(data);
        visitor.Visit(boolB);
        visitor.Visit(intB);
        visitor.Visit(structB);
    }

    REQUIRE(boolA == boolB);
    REQUIRE(intA == intB);
    REQUIRE(structA.m_A == structB.m_A);
    REQUIRE(structA.m_B == structB.m_B);
    REQUIRE(structA.m_String == structB.m_String);
}