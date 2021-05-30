#include "gc.hpp"
#include <iostream>
#include <vector>

void go() {
    std::vector<yagcplus::GarbageObject> objects;
    yagcplus::GarbageCollector gc(objects);
    const int loop = 10;
    std::cout << "Running " << loop << " loop\n";

    for (int i = 0; i < loop; i++) {
        auto newObject = gc.alloc(sizeof(int));
        std::cout << "ref Count " << **newObject.refCount << "\n";
        std::cout << "object counts: " << gc.getObjectCounts() << "\n";
    }
}

int main(void) { go(); }