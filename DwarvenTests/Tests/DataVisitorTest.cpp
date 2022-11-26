#include <catch2/catch.hpp>

#include <DwarvenCore/DataVisitor.h>
#include <DwarvenCore/New.h>

namespace { 
    struct TestStructure {
        bool m_A;
        int m_B;
    };
}

namespace df {
    template<>
    struct BufferVisitLogic<TestStructure> {
        static void Visit(DataVisitor* visitor, TestStructure& value) {
            visitor->Visit(value.m_A);
            visitor->Visit(value.m_B);
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
}