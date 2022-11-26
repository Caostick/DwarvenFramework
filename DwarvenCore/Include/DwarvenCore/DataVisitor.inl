#include <type_traits>

template<typename T>
void df::DataVisitor::Visit(T& value) {
    if constexpr (std::is_class<T>::value) {
        df::BufferVisitLogic<T>::Visit(this, value);
    } else {
        VisitBuffer((uint8*)(&value), sizeof(T));
    }
}