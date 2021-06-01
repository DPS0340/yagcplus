#ifndef YAGCPLUS_H
#define YAGCPLUS_H

#include <algorithm>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace yagcplus {
class GarbageCollector;
class GarbageObject;

class GarbageCollector {
  private:
  public:
    GarbageCollector();
    GarbageObject alloc(const size_t size);
};
class GarbageObject {
  private:
  public:
    GarbageCollector &collector;

    void **ptr = nullptr;
    int **refCount = nullptr;
    GarbageObject();

    GarbageObject(GarbageCollector &collector, const size_t size);
    GarbageObject(const GarbageObject &obj);
    GarbageObject &operator=(const GarbageObject &obj);
    ~GarbageObject();

    bool operator==(const GarbageObject &rhs) const;
    void free();
    void *getPointer();
};

} // namespace yagcplus

#endif