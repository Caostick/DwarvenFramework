#pragma once

#include <DwarvenCore/Types.h>

namespace df {
    class DataVisitor {
    public:
        void Visit(bool& value);

        template<typename T>
        void Visit(T& value);

    protected:
        virtual void VisitBuffer(uint8* data, uint32 size) = 0;
    };

    class ReadDataVisitor : public DataVisitor {
    public:
        ReadDataVisitor(uint8* data);

    protected:
        void VisitBuffer(uint8* data, uint32 size) override;

    private:
        uint8* m_Data;
    };

    class WriteDataVisitor : public DataVisitor {
    public:
        WriteDataVisitor(uint8* data);

    protected:
        void VisitBuffer(uint8* data, uint32 size) override;
        
    private:
        uint8* m_Data;
    };

    template<typename>
    struct BufferVisitLogic;
}

#include <DwarvenCore/DataVisitor.inl>