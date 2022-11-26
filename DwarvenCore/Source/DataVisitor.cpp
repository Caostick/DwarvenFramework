#include <DwarvenCore/DataVisitor.h>

#include <DwarvenCore/Memory.h>

void df::DataVisitor::Visit(bool& value) {
    uint8 val = uint8(value);
    VisitBuffer(&val, 1);
    value = (val != 0);
}



df::WriteDataVisitor::WriteDataVisitor(void* data)
    : m_Data(reinterpret_cast<uint8*>(data)) {
}

void df::WriteDataVisitor::VisitBuffer(void* data, size_t size) {
    df::MemCpy(m_Data, data, size);
    m_Data += size;
}



df::ReadDataVisitor::ReadDataVisitor(void* data)
    : m_Data(reinterpret_cast<uint8*>(data)) {
}

void df::ReadDataVisitor::VisitBuffer(void* data, size_t size) {
    df::MemCpy(data, m_Data, size);
    m_Data += size;
}