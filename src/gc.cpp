#include "gc.hpp"
#include <algorithm>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace yagcplus {
using namespace yagcplus;
GarbageCollector::GarbageCollector() {}
GarbageObject GarbageCollector::alloc(const size_t size) {
    GarbageObject result = GarbageObject(*this, size);
    *result.refCount = 1;
    return result;
}
GarbageObject::GarbageObject(GarbageCollector &collector, const size_t size)
    : collector(collector) {
    void *allocated = std::malloc(size);
    this->ptr = allocated;
    refCount = (int *)std::malloc(sizeof(int));
}
GarbageObject::GarbageObject(const GarbageObject &obj)
    : collector(obj.collector) {
    ptr = obj.ptr;
    refCount = obj.refCount;
    if (refCount != nullptr) {
        *refCount += 1;
    }
}
GarbageObject &GarbageObject::operator=(const GarbageObject &obj) {
    collector = obj.collector;
    ptr = obj.ptr;
    refCount = obj.refCount;
    if (refCount != nullptr) {
        *refCount += 1;
    }
    return *this;
}
GarbageObject::~GarbageObject() {
    *refCount -= 1;
    if (*refCount <= 0) {
        this->free();
    }
}
bool GarbageObject::operator==(const GarbageObject &rhs) const {
    return ptr == rhs.ptr && refCount == rhs.refCount;
}
void GarbageObject::free() {
    std::free(ptr);
    std::free(refCount);
}
void *GarbageObject::getPointer() { return ptr; }

} // namespace yagcplus