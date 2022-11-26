#pragma once

#include <DwarvenCore/Types.h>

namespace df {
    class DataVisitor {
    public:
        void Visit(bool& value);

        template<typename T>
        void Visit(T& value);

        virtual void VisitBuffer(void* data, size_t size) = 0;
    };

    class ReadDataVisitor : public DataVisitor {
    public:
        ReadDataVisitor(void* data);

        void VisitBuffer(void* data, size_t size) override;

    private:
        uint8* m_Data;
    };

    class WriteDataVisitor : public DataVisitor {
    public:
        WriteDataVisitor(void* data);

        void VisitBuffer(void* data, size_t size) override;
        
    private:
        uint8* m_Data;
    };

    template<typename>
    struct BufferVisitLogic;
}

#include <DwarvenCore/DataVisitor.inl>