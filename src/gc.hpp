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
    std::vector<GarbageObject> &objects;

    GarbageCollector(std::vector<GarbageObject> &objects);

    GarbageCollector(const GarbageCollector &collector);
    GarbageCollector &operator=(const GarbageCollector &obj);

    const GarbageObject &alloc(const size_t size);
    const bool free(const GarbageObject &garbage);
    const int find(const GarbageObject &obj);
    const int getObjectCounts();
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

    void free();
    bool operator==(const GarbageObject &rhs) const;
    void explicitDelete();
    void *getPointer();
};

} // namespace yagcplus

#endif