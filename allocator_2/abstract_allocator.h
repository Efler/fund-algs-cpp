#include <iostream>
using namespace std;

class abstract_allocator
{

public:

    virtual void* allocate(size_t target_size) const = 0;

    virtual void deallocate(void* target_to_dealloc) const = 0;

    virtual ~abstract_allocator() = default;

};