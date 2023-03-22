#include <iostream>
#include <cmath>
#include "abstract_allocator.h"
#include "..\logger\logger_builder.h"

class allocator_2 final : public abstract_allocator
{
public:

    enum class mode{
        first,
        best,
        worst
    };

private:

    void* _memory;

private:

    size_t memory_size() const {
        return *reinterpret_cast<size_t*>(_memory);
    }

    mode memory_mode() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        return *reinterpret_cast<mode*>(p_memory);
    }

    shared_ptr<logger> memory_logger() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        return *reinterpret_cast<shared_ptr<logger>*>(p_memory);
    }

    void** memory_first_block() const {
        void* p_memory = _memory;
        void** first;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<shared_ptr<logger>*>(p_memory) + 1);
        first = reinterpret_cast<void**>(*(reinterpret_cast<void**>(p_memory)));
        return first;
    }

    void** memory_first_block_pointer() const {
        void* p_memory = _memory;
        void** first;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<shared_ptr<logger>*>(p_memory) + 1);
        return reinterpret_cast<void**>(p_memory);
    }

    size_t block_size(void** pointer) const {
        void** p = pointer;
        return *reinterpret_cast<size_t*>(++p);
    }

    size_t* block_size_pointer(void** pointer) const {
        void** p = pointer;
        return reinterpret_cast<size_t*>(++p);
    }

    void* block_pool(void** pointer) const {
        void* p = reinterpret_cast<void*>((pointer) + 1);
        p = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p) + 1);
        return p;
    }

public:

    allocator_2(size_t size, mode alloc_mode, shared_ptr<logger> alloc_logger, const abstract_allocator& alloc){
        try{
            _memory = alloc.allocate(size);
        }catch(const logic_error& ex){
            throw logic_error("Bad Allocation!");
        }

        *(reinterpret_cast<size_t*>(_memory)) = size;
        _memory = reinterpret_cast<void*>(_memory);
        void* p_memory = _memory;
        void** first;

        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        *(reinterpret_cast<mode*>(p_memory)) = alloc_mode;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        *(reinterpret_cast<shared_ptr<logger>*>(p_memory)) = std::move(alloc_logger);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<shared_ptr<logger>*>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        first = reinterpret_cast<void**>(p_memory);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        *first = p_memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<size_t*>(p_memory)) = size - (sizeof(size_t) + sizeof(mode) + sizeof(shared_ptr<logger>) + sizeof(void*)+ sizeof(void*) + sizeof(size_t));
    }

    allocator_2(size_t size, mode alloc_mode, shared_ptr<logger> alloc_logger){
        try{
            _memory = ::operator new(size);
        }catch(const bad_alloc &ba){
            throw logic_error("Bad Allocation!");
        }

        *(reinterpret_cast<size_t*>(_memory)) = size;
        _memory = reinterpret_cast<void*>(_memory);
        void* p_memory = _memory;
        void** first;

        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        *(reinterpret_cast<mode*>(p_memory)) = alloc_mode;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        *(reinterpret_cast<shared_ptr<logger>*>(p_memory)) = std::move(alloc_logger);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<shared_ptr<logger>*>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        first = reinterpret_cast<void**>(p_memory);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        *first = p_memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<size_t*>(p_memory)) = size - (sizeof(size_t) + sizeof(mode) + sizeof(shared_ptr<logger>) + sizeof(void*)+ sizeof(void*) + sizeof(size_t));
    }

    ~allocator_2() override {
        ::operator delete(_memory);
    }

    allocator_2(const allocator_2& alloc) = delete;

    allocator_2(allocator_2&& alloc) = delete;

    allocator_2& operator=(const allocator_2& alloc) = delete;

    allocator_2& operator=(allocator_2&& alloc) = delete;

    void* allocate(size_t target_size) const override{

        ////debug------------------
//        printf("%lld\n", (long long)_memory);
//        printf("%lld\n", (long long)memory_first_block());
//        printf("%lld\n", (long long)memory_first_block_pointer());
//        printf("%lld\n --- \n", (long long)block_size(memory_first_block()));
        ////debug------------------

        int flag = 0;
        void** p = memory_first_block();
        void** p_best = nullptr;
        void** p_pr = nullptr;
        void** p_next;
        void** p_first = memory_first_block_pointer();

        do{
            if(memory_mode() == mode::first){
                if(block_size(p) >= (target_size + sizeof(void**) + sizeof(size_t))){
                    flag++;
                    break;
                }
            }else if(memory_mode() == mode::best){
                if(block_size(p) >= (target_size + sizeof(void**) + sizeof(size_t))){
                    flag++;
                    if(p_best == nullptr){
                        p_best = p;
                    }else{
                        if(abs((int)(target_size - block_size(p_best))) > abs((int)(target_size - block_size(p)))){
                            p_best = p;
                        }
                    }
                }
            }else if(memory_mode() == mode::worst){
                if(block_size(p) >= (target_size + sizeof(void**) + sizeof(size_t))){
                    flag++;
                    if(p_best == nullptr){
                        p_best = p;
                    }else{
                        if(abs((int)(target_size - block_size(p_best))) < abs((int)(target_size - block_size(p)))){
                            p_best = p;
                        }
                    }
                }
            }
            if(*p != nullptr){
                p_pr = p;
                p = reinterpret_cast<void**>(*p);
            }
        }while(*p != nullptr);

        if(!flag){
            //TODO: logger
            throw logic_error("Bad Allocation!");
        }
        if(p_best != nullptr){
            p = p_best;
        }

        if(block_size(p) == (target_size + sizeof(void**) + sizeof(size_t))){
            if(*p != nullptr){
                p_next = reinterpret_cast<void**>(*p);
                if(p_pr != nullptr){
                    *p_pr = p_next;
                }else{
                    *p_first = p_next;
                }
            }else{
                if(p_pr != nullptr) *p_pr = nullptr;
            }
        }
        else{
            if(*p != nullptr){
                p_next = reinterpret_cast<void**>((reinterpret_cast<char*>(block_pool(p))) + target_size);
                *p_next = *p;
                size_t* size_next = block_size_pointer(p_next);
                *size_next = block_size(p) - sizeof(void**) - sizeof(size_t) - target_size;
                if(p_pr != nullptr){
                    *p_pr = p_next;
                }else{
                    *p_first = p_next;
                }
            }
            else{
                p_next = reinterpret_cast<void**>((reinterpret_cast<char*>(block_pool(p))) + target_size);
                *p_next = nullptr;
                size_t* size_next = block_size_pointer(p_next);
                *size_next = block_size(p) - sizeof(void**) - sizeof(size_t) - target_size;
                if(p_pr != nullptr){
                    *p_pr = p_next;
                }else{
                    *p_first = p_next;
                }
            }
        }
        return block_pool(p);
    }

    void deallocate(void* target_to_dealloc) const override {

    }
};

//TODO:  1. filled block service memory ??
//       2. logger
//       3. dealloc :(