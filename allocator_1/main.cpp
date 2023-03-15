#include <iostream>
#include <string>
#include "allocator_1.h"

int main(){
    auto* builder = new logger_builder;
    logger* logger = builder->add_stream("console", logger::severity::debug)
                            ->build();

    memory alloc(logger);
    char* p = reinterpret_cast<char*>(alloc.allocate(12));
    int* r = reinterpret_cast<int*>(alloc.allocate(20));
    *p = 'h';
    *(p+1) = 'e';
    *(p+2) = 'y';
    *(p+3) = 0;
    string s = p;
    cout << s <<endl;
    alloc.deallocate(reinterpret_cast<void*>(p));

    delete logger;
    delete builder;
}