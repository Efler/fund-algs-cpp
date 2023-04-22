#ifndef ALLOCATOR_2_ABSTRACT_ALLOCATOR_H
#define ALLOCATOR_2_ABSTRACT_ALLOCATOR_H

#include <iostream>
using namespace std;


class abstract_allocator
{

public:

    virtual void* allocate(size_t target_size) const = 0;

    virtual void deallocate(void* target_to_dealloc) const = 0;

    virtual ~abstract_allocator() = default;

};


#endif //ALLOCATOR_2_ABSTRACT_ALLOCATOR_H