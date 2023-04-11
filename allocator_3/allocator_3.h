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
        string s;
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

    size_t memory_size() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<abstract_allocator**>(p_memory) + 1);
        return *reinterpret_cast<size_t*>(p_memory);
    }

    size_t* memory_size_pointer() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<abstract_allocator**>(p_memory) + 1);
        return reinterpret_cast<size_t*>(p_memory);
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
        return *reinterpret_cast<size_t*>(p + 2);
    }

    size_t* block_size_pointer(void** pointer) const {
        void** p = pointer;
        return reinterpret_cast<size_t*>(p + 2);
    }

    void* block_pool(void** pointer) const {
        void* p = reinterpret_cast<void*>(pointer + 2);
        p = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p) + 1);
        return p;
    }

    void** next(void** pointer) const {
        return reinterpret_cast<void**>(*(pointer + 1));
    }

    void** next_pointer(void** pointer)const {
        return pointer + 1;
    }

    abstract_allocator* memory_allocator() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        return *reinterpret_cast<abstract_allocator**>(p_memory);
    }

    void** space_after_block(void** p) const {
        return reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(reinterpret_cast<size_t*>(p + 2) + 1) + block_size(p));
    }

    size_t count_free_space(void** p) const {
        if(*p == nullptr){
            return (unsigned char*)p - (unsigned char*)(memory_size_pointer() + 1);
        }else{
            return (unsigned char*)p - (unsigned char*)(reinterpret_cast<unsigned char*>(block_pool(reinterpret_cast<void**>(*p))) + block_size(reinterpret_cast<void**>(*p)));
        }
    }

    size_t count_end_space(void** p) const {
        unsigned char* a = reinterpret_cast<unsigned char*>(reinterpret_cast<size_t*>(p + 2) + 1) + block_size(p);
        unsigned char* b = reinterpret_cast<unsigned char*>(memory_size_pointer() + 1) + memory_size();
        return b - a;
    }

public:

    explicit allocator_3(size_t size, mode alloc_mode = mode::first, logger* alloc_logger = nullptr, abstract_allocator* alloc = nullptr){
        try{
            if(alloc != nullptr){
                _memory = alloc->allocate(size + (sizeof(mode) + sizeof(logger*) + sizeof(void*) + sizeof(abstract_allocator*) + sizeof(size_t)));
            }else{
                _memory = ::operator new(size + (sizeof(mode) + sizeof(logger*) + sizeof(void*) + sizeof(abstract_allocator*) + sizeof(size_t)));
            }
        }catch(const logic_error& ex){
            throw logic_error("Bad Allocation!");
        }

        size_t* mem_size;
        void* p_memory = _memory;

        *(reinterpret_cast<mode*>(p_memory)) = alloc_mode;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<mode*>(p_memory) + 1);
        *(reinterpret_cast<logger**>(p_memory)) = alloc_logger;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        *(reinterpret_cast<void**>(p_memory)) = nullptr;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *(reinterpret_cast<abstract_allocator**>(p_memory)) = alloc;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<abstract_allocator**>(p_memory) + 1);
        *reinterpret_cast<size_t*>(p_memory) = 0;
        mem_size = reinterpret_cast<size_t*>(p_memory);
        *mem_size = size;

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
        if(memory_allocator() == nullptr){
            ::operator delete(_memory);
        }else{
            memory_allocator()->deallocate(_memory);
        }
    }

    allocator_3(const allocator_3& alloc) = delete;

    allocator_3(allocator_3&& alloc) = delete;

    allocator_3& operator=(const allocator_3& alloc) = delete;

    allocator_3& operator=(allocator_3&& alloc) = delete;

    void* allocate(size_t target_size) const override{
        int flag = 0;
        int end_flag = 0;
        void** p = memory_first_block();
        void** p_best = nullptr;
        void** p_target;

        if(p == nullptr){
            if(target_size + 2 * sizeof(void*) + sizeof(size_t) > memory_size()){
                string message = "size: ";
                message += to_string(target_size);
                message += ", Bad Allocation!!\n";
                throw logic_error(message);
            }
            p = reinterpret_cast<void**>(memory_size_pointer() + 1);
            *memory_first_block_pointer() = reinterpret_cast<void**>(p);
            *p = nullptr;
            *next_pointer(p) = nullptr;
            *block_size_pointer(p) = target_size;

            if(memory_logger() != nullptr){
                string msg = "Allocation complete, address: ";
                msg += address_to_hex(block_pool(p));
                msg += ", size: ";
                msg += to_string(target_size);
                msg += "\n";
                memory_logger()->log(msg, logger::severity::debug);
            }

            return block_pool(p);

        }else{

            while(next(p) != nullptr){
                if (count_free_space(p) >= (target_size + 2 * sizeof(void*) + sizeof(size_t))){
                    flag++;
                    if(memory_mode() == mode::first){
                        break;
                    }else if(memory_mode() == mode::best){
                        if(p_best == nullptr){
                            p_best = p;
                        }else{
                            if((count_free_space(p) - (target_size + 2 * sizeof(void*) + sizeof(size_t))) < (count_free_space(p_best) - (target_size + 2 * sizeof(void*) + sizeof(size_t)))){
                                p_best = p;
                            }
                        }
                    }else if(memory_mode() == mode::worst){
                        if(p_best == nullptr){
                            p_best = p;
                        }else{
                            if((count_free_space(p) - (target_size + 2 * sizeof(void*) + sizeof(size_t))) > (count_free_space(p_best) - (target_size + 2 * sizeof(void*) + sizeof(size_t)))){
                                p_best = p;
                            }
                        }
                    }
                }
                p = next(p);
            }

            if(memory_mode() == mode::first && flag == 0){
                if(count_free_space(p) >= (target_size + 2 * sizeof(void*) + sizeof(size_t))){
                    flag++;
                }else if(count_end_space(p) >= (target_size + 2 * sizeof(void*) + sizeof(size_t))){
                    flag++;
                    end_flag++;
                }
            }else if(memory_mode() == mode::best){
                if(p_best != nullptr){
                    if((count_end_space(p) - (target_size + 2 * sizeof(void*) + sizeof(size_t))) < (count_free_space(p_best) - (target_size + 2 * sizeof(void*) + sizeof(size_t)))){
                        p_best = p;
                        end_flag++;
                    }
                }else{
                    if(count_end_space(p) >= (target_size + 2 * sizeof(void*) + sizeof(size_t))){
                        flag++;
                        end_flag++;
                        p_best = p;
                    }
                }
            }else if(memory_mode() == mode::worst){
                if(p_best != nullptr){
                    if((count_end_space(p) - (target_size + 2 * sizeof(void*) + sizeof(size_t))) > (count_free_space(p_best) - (target_size + 2 * sizeof(void*) + sizeof(size_t)))){
                        p_best = p;
                        end_flag++;
                    }
                }else{
                    if(count_end_space(p) >= (target_size + 2 * sizeof(void*) + sizeof(size_t))){
                        flag++;
                        end_flag++;
                        p_best = p;
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

            if(p_best != nullptr) p = p_best;

            if(end_flag == 1){
                p_target = space_after_block(p);
                *p_target = reinterpret_cast<void**>(p);
                *next_pointer(p_target) = nullptr;
                *next_pointer(p) = reinterpret_cast<void**>(p_target);
                *block_size_pointer(p_target) = target_size;
            }else{
                if(*p != nullptr){
                    p_target = space_after_block(reinterpret_cast<void**>(*p));
                    *p_target = reinterpret_cast<void**>(*p);
                    *next_pointer(reinterpret_cast<void**>(*p)) = p_target;
                    *next_pointer(p_target) = p;
                    *p = reinterpret_cast<void**>(p_target);
                    *block_size_pointer(p_target) = target_size;
                }else{
                    p_target = reinterpret_cast<void**>(memory_size_pointer() + 1);
                    *p_target = nullptr;
                    *memory_first_block_pointer() = p_target;
                    *next_pointer(p_target) = p;
                    *p = reinterpret_cast<void**>(p_target);
                    *block_size_pointer(p_target) = target_size;
                }
            }

            if(memory_logger() != nullptr){
                string msg = "Allocation complete, address: ";
                msg += address_to_hex(block_pool(p_target));
                msg += ", size: ";
                msg += to_string(target_size);
                msg += "\n";
                memory_logger()->log(msg, logger::severity::debug);
            }

            return block_pool(p_target);

        }
    }

    void deallocate(void* target_to_dealloc) const override {
        void** p = reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_to_dealloc) - 1);
        p = reinterpret_cast<void**>(p - 2);
        size_t dump_size = block_size(p);

        if(*p != nullptr){
            if(next(p) != nullptr){
                *next_pointer(reinterpret_cast<void**>(*p)) = next(p);
                *next(p) = reinterpret_cast<void**>(*p);
            }else{
                *next_pointer(reinterpret_cast<void**>(*p)) = next(p);
            }
        }else{
            if(next(p) != nullptr){
                *next(p) = *p;
                *memory_first_block_pointer() = next(p);
            }else{
                *memory_first_block_pointer() = nullptr;
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
