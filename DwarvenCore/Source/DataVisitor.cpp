#include <DwarvenCore/DataVisitor.h>

#include <DwarvenCore/Memory.h>

void df::DataVisitor::Visit(bool& value) {
    uint8 val = uint8(value);
    VisitBuffer(&val, 1);
    value = (val != 0);
}



df::WriteDataVisitor::WriteDataVisitor(uint8* data)
    : m_Data(data) {
}

void df::WriteDataVisitor::VisitBuffer(uint8* data, uint32 size) {
    df::MemCpy(m_Data, data, size);
    m_Data += size;
}



df::ReadDataVisitor::ReadDataVisitor(uint8* data)
    : m_Data(data) {
}

void df::ReadDataVisitor::VisitBuffer(uint8* data, uint32 size) {
    df::MemCpy(data, m_Data, size);
    m_Data += size;
}