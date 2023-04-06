#ifndef ALLOCATOR_3_ALLOCATOR_3_H
#define ALLOCATOR_3_ALLOCATOR_3_H


#include <iostream>
#include <cmath>
#include <memory>
#include "..\logger\logger_builder.h"
#include "..\allocator_2\abstract_allocator.h"


class allocator_3 final : public abstract_allocator
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

    string memory_dump(void* const p, const size_t t) const {
        auto* p2 = reinterpret_cast<unsigned char*>(p);
        string s = "";
        int dump;
        for(int k = 0; k < t; ++k){
            dump = (int)(*p2);
            s += to_string(dump);
            s += ' ';
            p2 = reinterpret_cast<unsigned char*>(p2) + 1;
        }
        return s;
    }

    string address_to_hex(void const * const pointer) const noexcept {
        char address_buf[(sizeof(void const * const) << 1) + 3];
        sprintf(address_buf, "%#p", pointer);
        return std::string { address_buf };
    }

    mode memory_mode() const {
        void* p_memory = _memory;
        return *reinterpret_cast<mode*>(p_memory);
    }

    logger* memory_logger() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        return *reinterpret_cast<logger**>(p_memory);
    }

    void** memory_first_block() const {
        void* p_memory = _memory;
        void** first;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        first = reinterpret_cast<void**>(*(reinterpret_cast<void**>(p_memory)));
        return first;
    }

    void** memory_first_block_pointer() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        return reinterpret_cast<void**>(p_memory);
    }

    size_t block_size(void** pointer) const {
        void** p = pointer;
        return *reinterpret_cast<size_t*>(p + 1);
    }

    size_t* block_size_pointer(void** pointer) const {
        void** p = pointer;
        return reinterpret_cast<size_t*>(p + 1);
    }

    void* block_pool(void** pointer) const {
        void* p = reinterpret_cast<void*>(pointer + 1);
        p = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p) + 1);
        return p;
    }

public:

    allocator_3(size_t size, mode alloc_mode, logger* alloc_logger, const abstract_allocator& alloc){
        try{
            _memory = alloc.allocate(size);
        }catch(const logic_error& ex){
            throw logic_error("Bad Allocation!");
        }

        void* p_memory = _memory;
        void** first;

        *(reinterpret_cast<mode*>(p_memory)) = alloc_mode;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        *(reinterpret_cast<logger**>(p_memory)) = alloc_logger;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        first = reinterpret_cast<void**>(p_memory);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        *first = p_memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<size_t*>(p_memory)) = size - (sizeof(mode) + sizeof(logger*) + sizeof(void*) + sizeof(void*) + sizeof(size_t));

        if(memory_logger() != nullptr){
            string msg = "ALLOCATOR CREATED (MEMORY SIZE: ";
            msg += to_string((int)size);
            msg += " BYTES)\n";
            memory_logger()->log(msg, logger::severity::debug);
        }
    }

    allocator_3(size_t size, mode alloc_mode, logger* alloc_logger){
        try{
            _memory = ::operator new(size);
        }catch(const bad_alloc &ba){
            throw logic_error("Bad Allocation!");
        }

        void* p_memory = _memory;
        void** first;

        *(reinterpret_cast<mode*>(p_memory)) = alloc_mode;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        *(reinterpret_cast<logger**>(p_memory)) = alloc_logger;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        first = reinterpret_cast<void**>(p_memory);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        *first = p_memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<size_t*>(p_memory)) = size - (sizeof(mode) + sizeof(logger*) + sizeof(void*) + sizeof(void*) + sizeof(size_t));

        if(memory_logger() != nullptr){
            string msg = "ALLOCATOR CREATED (MEMORY SIZE: ";
            msg += to_string((int)size);
            msg += " BYTES)\n";
            memory_logger()->log(msg, logger::severity::debug);
        }
    }

    ~allocator_3() override {
        if(memory_logger() != nullptr){
            string msg = "ALLOCATOR DELETED\n";
            memory_logger()->log(msg, logger::severity::debug);
        }
        ::operator delete(_memory);
    }

    allocator_3(const allocator_3& alloc) = delete;

    allocator_3(allocator_3&& alloc) = delete;

    allocator_3& operator=(const allocator_3& alloc) = delete;

    allocator_3& operator=(allocator_3&& alloc) = delete;

    void* allocate(size_t target_size) const override{

        int flag = 0;
        void** p = memory_first_block();
        void** p_best = nullptr;
        void** p_pr = nullptr;
        void** p_pr_best = nullptr;
        void** p_next;

        while(*p != nullptr){
            if(memory_mode() == mode::first){
                if(block_size(p) >= (target_size + sizeof(void*) + sizeof(size_t))){
                    flag++;
                    break;
                }
            }else if(memory_mode() == mode::best){
                if(block_size(p) >= (target_size + sizeof(void*) + sizeof(size_t))){
                    flag++;
                    if(p_best == nullptr){
                        p_best = p;
                        p_pr_best = p_pr;
                    }else{
                        if((block_size(p) - (target_size + sizeof(void*) + sizeof(size_t))) < (block_size(p_best) - (target_size + sizeof(void*) + sizeof(size_t)))){
                            p_best = p;
                            p_pr_best = p_pr;
                        }
                    }
                }
            }else if(memory_mode() == mode::worst){
                if(block_size(p) >= (target_size + sizeof(void*) + sizeof(size_t))){
                    flag++;
                    if(p_best == nullptr){
                        p_best = p;
                        p_pr_best = p_pr;
                    }else{
                        if((block_size(p) - (target_size + sizeof(void*) + sizeof(size_t))) > (block_size(p_best) - (target_size + sizeof(void*) + sizeof(size_t)))){
                            p_best = p;
                            p_pr_best = p_pr;
                        }
                    }
                }
            }
            p_pr = p;
            p = reinterpret_cast<void**>(*p);
        }

        if(memory_mode() == mode::first){
            if(block_size(p) >= (target_size + sizeof(void*) + sizeof(size_t))){
                flag++;
            }
        }else if(memory_mode() == mode::best){
            if(block_size(p) >= (target_size + sizeof(void*) + sizeof(size_t))){
                flag++;
                if(p_best == nullptr){
                    p_best = p;
                    p_pr_best = p_pr;
                }else{
                    if((block_size(p) - (target_size + sizeof(void*) + sizeof(size_t))) < (block_size(p_best) - (target_size + sizeof(void*) + sizeof(size_t)))){
                        p_best = p;
                        p_pr_best = p_pr;
                    }
                }
            }
        }else if(memory_mode() == mode::worst){
            if(block_size(p) >= (target_size + sizeof(void*) + sizeof(size_t))){
                flag++;
                if(p_best == nullptr){
                    p_best = p;
                    p_pr_best = p_pr;
                }else{
                    if((block_size(p) - (target_size + sizeof(void*) + sizeof(size_t))) > (block_size(p_best) - (target_size + sizeof(void*) + sizeof(size_t)))){
                        p_best = p;
                        p_pr_best = p_pr;
                    }
                }
            }
        }

        if(flag == 0){
            string msg;
            msg += "size: ";
            msg += to_string(target_size);
            msg += ", Bad Allocation!!\n";
            throw logic_error(msg);
        }

        if(p_best != nullptr){
            p = p_best;
            p_pr = p_pr_best;
        }

        if(block_size(p) == (target_size + sizeof(void*) + sizeof(size_t*))){
            if(*p != nullptr){
                p_next = reinterpret_cast<void**>(*p);
                if(p_pr != nullptr){
                    *p_pr = p_next;
                }else{
                    *memory_first_block_pointer() = p_next;
                }
            }else{
                if(p_pr != nullptr) *p_pr = nullptr;
                else *memory_first_block_pointer() = nullptr;
            }
        }
        else{
            if(*p != nullptr){
                p_next = reinterpret_cast<void**>((reinterpret_cast<char*>(block_pool(p))) + target_size);
                *p_next = reinterpret_cast<void**>(*p);
                size_t* size_next = block_size_pointer(p_next);
                *size_next = block_size(p) - sizeof(void*) - sizeof(size_t) - target_size;
                *(block_size_pointer(p)) = target_size;
                if(p_pr != nullptr){
                    *p_pr = p_next;
                }else{
                    *memory_first_block_pointer() = reinterpret_cast<void**>(p_next);
                }
            }
            else{
                p_next = reinterpret_cast<void**>((reinterpret_cast<char*>(block_pool(p))) + target_size);
                *p_next = nullptr;
                size_t* size_next = block_size_pointer(p_next);
                *size_next = block_size(p) - sizeof(void*) - sizeof(size_t) - target_size;
                *(block_size_pointer(p)) = target_size;
                if(p_pr != nullptr){
                    *p_pr = reinterpret_cast<void**>(p_next);
                }else{
                    *memory_first_block_pointer() = reinterpret_cast<void**>(p_next);
                }
            }
        }

        if(memory_logger() != nullptr){
            string msg = "Allocation complete, address: ";
            msg += address_to_hex(block_pool(p));
            msg += ", size: ";
            msg += to_string(target_size);
            msg += "\n";
            memory_logger()->log(msg, logger::severity::debug);
        }

        return block_pool(p);
    }

    void deallocate(void* target_to_dealloc) const override {
        void** p = reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_to_dealloc) - 1);
        p = reinterpret_cast<void**>(p - 1);

        size_t dump_size = block_size(p);
        void** p_pr = nullptr;
        void** p_next = memory_first_block();

        if(memory_first_block() == nullptr){
            *p = nullptr;
            *memory_first_block_pointer() = reinterpret_cast<void**>(p);

        }else{

            while((long long)p_next < (long long)p && *p_next != nullptr){
                p_pr = p_next;
                p_next = reinterpret_cast<void**>(*p_next);
            }

            if((long long)p_next < (long long)p && *p_next == nullptr){
                p_pr = p_next;
                p_next = nullptr;
            }

            if(p_next == nullptr){
                if( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p_pr + 1)) + 1) + block_size(p_pr)) == p){
                    *block_size_pointer(p_pr) = *block_size_pointer(p_pr) + *block_size_pointer(p) + sizeof(void*) + sizeof(size_t);
                }else{
                    *p = nullptr;
                    *p_pr = reinterpret_cast<void**>(p);
                }
            }else{

                if(*p_next == nullptr){
                    if(p_pr == nullptr){
                        if(reinterpret_cast<void**>(reinterpret_cast<char*>(reinterpret_cast<size_t*>(p + 1) + 1) + block_size(p)) == p_next) {
//                            cout << "1\n";
                            *block_size_pointer(p) = *block_size_pointer(p) + *block_size_pointer(p_next) + sizeof(void*) + sizeof(size_t);
                            *p = nullptr;
                            *memory_first_block_pointer() = reinterpret_cast<void**>(p);
                        }else{
//                            cout << "2\n";
                            *p = reinterpret_cast<void**>(p_next);
                            *memory_first_block_pointer() = reinterpret_cast<void**>(p);
                        }
                    }else{
                        if(( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p + 1)) + 1) + block_size(p)) ) == p_next){
                            if( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p_pr + 1)) + 1) + block_size(p_pr)) == p){
//                                cout << "3\n";
                                *block_size_pointer(p_pr) = *block_size_pointer(p_pr) + *block_size_pointer(p) + *block_size_pointer(p_next) + 2 * (sizeof(void*) + sizeof(size_t));
                                *p_pr = nullptr;
                            }else{
//                                cout << "4\n";
                                *block_size_pointer(p) = *block_size_pointer(p) + *block_size_pointer(p_next) + sizeof(void*) + sizeof(size_t);
                                *p_pr = reinterpret_cast<void**>(p);
                                *p = nullptr;
                            }
                        }else{
                            if( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p_pr + 1)) + 1) + block_size(p_pr)) == p){
//                                cout << "5\n";
                                *block_size_pointer(p_pr) = *block_size_pointer(p_pr) + *block_size_pointer(p) + sizeof(void*) + sizeof(size_t);
                            }else{
//                                cout << "6\n";
                                *p_pr = reinterpret_cast<void**>(p);
                                *p = reinterpret_cast<void**>(p_next);
                            }
                        }
                    }
                }else{
                    if(p_pr == nullptr){
                        if(( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p + 1)) + 1) + block_size(p)) ) == p_next){
//                            cout << "7\n";
                            *block_size_pointer(p) = *block_size_pointer(p) + *block_size_pointer(p_next) + sizeof(void*) + sizeof(size_t);
                            *p = reinterpret_cast<void**>(*p_next);
                            *memory_first_block_pointer() = reinterpret_cast<void**>(p);
                        }else{
//                            cout << "8\n";
                            *memory_first_block_pointer() = reinterpret_cast<void**>(p);
                            *p = reinterpret_cast<void**>(p_next);
                        }
                    }else{
                        if(( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p + 1)) + 1) + block_size(p)) ) == p_next){
                            if( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p_pr + 1)) + 1) + block_size(p_pr)) == p){
//                                cout << "9\n";
                                *p_pr = reinterpret_cast<void**>(*p_next);
                                *block_size_pointer(p_pr) = *block_size_pointer(p_pr) + *block_size_pointer(p) + *block_size_pointer(p_next) + 2 * (sizeof(void*) + sizeof(size_t));
                            }else{
//                                cout << "10\n";
                                *block_size_pointer(p) = *block_size_pointer(p) + *block_size_pointer(p_next) + sizeof(void*) + sizeof(size_t);
                                *p_pr = reinterpret_cast<void**>(p);
                                *p = reinterpret_cast<void**>(*p_next);
                            }
                        }else{
                            if( reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>(p_pr + 1)) + 1) + block_size(p_pr)) == p){
//                                cout << "11\n";
                                *block_size_pointer(p_pr) = *block_size_pointer(p_pr) + *block_size_pointer(p) + sizeof(void*) + sizeof(size_t);
                            }else{
//                                cout << "12\n";
                                *p_pr = reinterpret_cast<void**>(p);
                                *p = reinterpret_cast<void**>(p_next);
                            }
                        }
                    }
                }
            }
        }

        if(memory_logger() != nullptr){
            string msg = "Deallocation complete, address: ";
            msg += address_to_hex(target_to_dealloc);
            msg += ", size: ";
            msg += to_string(dump_size);
            msg += ", dump: ";
            msg += memory_dump(target_to_dealloc, dump_size);
            msg += "\n";
            memory_logger()->log(msg, logger::severity::debug);
        }
    }

};


#endif //ALLOCATOR_3_ALLOCATOR_3_H
