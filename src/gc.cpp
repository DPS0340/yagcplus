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
GarbageCollector::GarbageCollector(std::vector<GarbageObject> &objects)
    : objects(objects){};
const GarbageObject &GarbageCollector::alloc(const size_t size) {
    objects.push_back(GarbageObject(*this, size));
    auto &result = objects.back();
    **result.refCount = 1;
    return result;
}

GarbageCollector::GarbageCollector(const GarbageCollector &collector)
    : objects(collector.objects) {}

GarbageCollector &
GarbageCollector::operator=(const GarbageCollector &collector) {
    objects = collector.objects;
}

const int GarbageCollector::find(const GarbageObject &obj) {
    for (int i = 0, size = objects.size(); i < size; i++) {
        if (obj == objects[i]) {
            return i;
        }
    }
    return -1;
}

const bool GarbageCollector::free(const GarbageObject &obj) {
    const int index = find(obj);
    if (index == -1) {
        return false;
    }
    objects.erase(objects.begin() + index);
    return true;
}

const int GarbageCollector::getObjectCounts() { return objects.size(); }

GarbageObject::GarbageObject(GarbageCollector &collector, const size_t size)
    : collector(collector) {
    void *allocated = std::malloc(size);
    void **ptr = (void **)std::malloc(sizeof(void *));
    *ptr = allocated;
    this->ptr = ptr;
    int *ref = (int *)std::malloc(sizeof(int));
    refCount = (int **)std::malloc(sizeof(int *));
    *refCount = ref;
    **refCount = 1;
}
GarbageObject::GarbageObject(const GarbageObject &obj)
    : collector(obj.collector) {
    ptr = obj.ptr;
    refCount = obj.refCount;
    if (refCount != nullptr) {
        **refCount += 1;
    }
}
GarbageObject &GarbageObject::operator=(const GarbageObject &obj) {
    collector = obj.collector;
    ptr = obj.ptr;
    refCount = obj.refCount;
    if (refCount != nullptr) {
        **refCount += 1;
    }
}
void GarbageObject::free() {
    collector.free(*this);
    explicitDelete();
    std::free(ptr);
    std::free(refCount);
}
GarbageObject::~GarbageObject() {
    const int index = collector.find(*this);
    if (collector.find(*this) == -1) {
        return;
    }
    **refCount -= 1;
    if (**refCount == 0) {
        this->free();
    }
}
bool GarbageObject::operator==(const GarbageObject &rhs) const {
    return ptr == rhs.ptr && refCount == rhs.refCount;
}
void GarbageObject::explicitDelete() {
    if (*ptr != nullptr) {
        std::free(*ptr);
        *ptr = nullptr;
    }
    if (*refCount != nullptr) {
        std::free(*refCount);
        *refCount = nullptr;
    }
}
void *GarbageObject::getPointer() { return *ptr; }

} // namespace yagcplus