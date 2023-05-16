#ifndef ALLOCATOR_BUDDIES_ALLOCATOR_BUDDIES_H
#define ALLOCATOR_BUDDIES_ALLOCATOR_BUDDIES_H
#include <iostream>
#include "..\logger\logger_builder.h"
#include "..\allocator_2\abstract_allocator.h"


class allocator_buddies final : public abstract_allocator
{
private:

    void* _memory;

private:

    static string memory_dump(void* const p, const size_t t) {
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

    static string address_to_hex(void const * const pointer) noexcept {
        char address_buf[(sizeof(void const * const) << 1) + 3];
        sprintf(address_buf, "%#p", pointer);
        return std::string { address_buf };
    }

    size_t memory_size() const {
        void* p_memory = _memory;
        return *reinterpret_cast<size_t*>(p_memory);
    }

    size_t* memory_size_pointer() const {
        void* p_memory = _memory;
        return reinterpret_cast<size_t*>(p_memory);
    }

    logger* memory_logger() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        return *reinterpret_cast<logger**>(p_memory);
    }

    abstract_allocator* memory_allocator() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        return *reinterpret_cast<abstract_allocator**>(p_memory);
    }

    void** memory_first_block() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<abstract_allocator**>(p_memory) + 1);
        return reinterpret_cast<void**>(*reinterpret_cast<void**>(p_memory));
    }

    void** memory_first_block_pointer() const {
        void* p_memory = _memory;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<abstract_allocator**>(p_memory) + 1);
        return reinterpret_cast<void**>(p_memory);
    }

    static size_t block_size_power(void** pointer) {
        void** p = pointer;
        return (size_t)(static_cast<int>(*reinterpret_cast<unsigned char*>(p + 2)) >> 1);
    }

    static unsigned char* block_size_power_pointer(void** pointer) {
        void** p = pointer;
        return reinterpret_cast<unsigned char*>(p + 2);
    }

    static bool block_freedom(void** pointer) {
        if((static_cast<int>(*block_size_power_pointer(pointer)) & 1) == 0) return true;
        else return false;
    }

    static void change_freedom(void** pointer) {
        if(block_freedom(pointer)) {
            *block_size_power_pointer(pointer) = static_cast<unsigned char>(static_cast<int>(*block_size_power_pointer(pointer)) | 1);
        }else {
            *block_size_power_pointer(pointer) = static_cast<unsigned char>((static_cast<int>(*block_size_power_pointer(pointer)) >> 1) << 1);
        }
    }

    static void* block_pool(void** pointer) {
        void* p = reinterpret_cast<void*>(pointer + 2);
        p = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(p) + 1);
        return p;
    }

    static void** next(void** pointer) {
        return reinterpret_cast<void**>(*(pointer + 1));
    }

    static void** next_pointer(void** pointer) {
        return pointer + 1;
    }

    void* splitting_block(void** p, size_t target_size, size_t& real_size, size_t& real_power) const {
        string debug_msg = "~SPLITTING DEBUG~ - TARGET BLOCK: ";
        debug_msg += address_to_hex(p);
        debug_msg += ", POWER: ";
        debug_msg += to_string(block_size_power(p));
        debug_msg += ", WANTED TO ALLOCATE: ";
        debug_msg += to_string(target_size + sizeof(void*) + sizeof(void*) + sizeof(unsigned char));
        debug_msg += "(";
        debug_msg += to_string(target_size);
        debug_msg += ") BYTES";
        memory_logger()->log(debug_msg, logger::severity::trace);
        debug_msg = "~SPLITTING DEBUG~ - HALF OF A BLOCK: ";
        debug_msg += to_string(1 << (block_size_power(p) - 1));

        if(1 << (block_size_power(p) - 1) >= target_size + sizeof(void*) + sizeof(void*) + sizeof(unsigned char)){
            debug_msg += " BYTES, SIZE FITS: SPLITTING...";
            memory_logger()->log(debug_msg, logger::severity::trace);

            void* p_new = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(block_pool(p)) + ((1 << (block_size_power(p) - 1)) - sizeof(void*) - sizeof(void*) - sizeof(unsigned char)));
            auto* p_new2 = p_new;
            *reinterpret_cast<void**>(p_new) = p;
            p_new  = reinterpret_cast<void*>(reinterpret_cast<void**>(p_new) + 1);
            *reinterpret_cast<void**>(p_new) = next(p);
            if(next(p) != nullptr) *reinterpret_cast<void**>(next(p)) = reinterpret_cast<void**>(p_new) - 1;
            p_new  = reinterpret_cast<void*>(reinterpret_cast<void**>(p_new) + 1);
            *reinterpret_cast<unsigned char*>(p_new) = static_cast<unsigned char>((block_size_power(p) - 1) << 1);
            *reinterpret_cast<void**>(next_pointer(p)) = reinterpret_cast<void**>(p_new2);

            size_t x = block_size_power(p);
            *block_size_power_pointer(p) = static_cast<unsigned char>((x - 1) << 1);
            return splitting_block(p, target_size, real_size, real_power);

        }else{
            debug_msg += "BYTES, SIZE IS INVALID: STOP SPLIT !!";
            memory_logger()->log(debug_msg, logger::severity::trace);

            if(*p == nullptr){
                if(next(p) == nullptr){
                    *memory_first_block_pointer() = nullptr;
                }else{
                    *memory_first_block_pointer() = next(p);
                    *(next(p)) = nullptr;
                }
            }else{
                if(next(p) == nullptr){
                    *next_pointer(reinterpret_cast<void**>(*p)) = nullptr;
                }else{
                    *next_pointer(reinterpret_cast<void**>(*p)) = next(p);
                    *(next(p)) = reinterpret_cast<void**>(*p);
                }
            }

            real_size = (1 << block_size_power(p));
            real_power = block_size_power(p);
            change_freedom(p);
            return block_pool(p);
        }
    }

    void** get_buddies(void* current_block) const {
        size_t block_size = (1 << block_size_power(reinterpret_cast<void**>(current_block)));

        if (block_size == (1 << memory_size())) return nullptr;

        auto* const start_allocated_memory = memory_first_block_pointer() + 1;

        size_t relative_address = (long long)(current_block) - (long long)(start_allocated_memory);
        size_t relative_address_buddies = relative_address ^ block_size;

        return reinterpret_cast<void**>((long long)start_allocated_memory + relative_address_buddies);
    }

    void** merging_blocks(void** p) const {
        string debug_msg = "~MERGING DEBUG~ - TRYING TO MERGE...";
        memory_logger()->log(debug_msg, logger::severity::trace);

        if(!block_freedom(p)) change_freedom(p);

        void** p_forward = nullptr;
        void** p_backward = nullptr;
        void** buddy = get_buddies(reinterpret_cast<void*>(p));
        if(buddy != nullptr){
            if((long long)buddy < (long long)p) p_backward = buddy;
            else p_forward = buddy;
        }

        if(p_forward != nullptr && (block_size_power(p_forward) == block_size_power(p) && block_freedom(p_forward))){
            debug_msg = "~MERGING DEBUG~ - BUDDY IN FRONT, POWER: ";
            debug_msg += to_string(block_size_power(p_forward));
            memory_logger()->log(debug_msg, logger::severity::trace);

            size_t x = block_size_power(p);
            *block_size_power_pointer(p) = static_cast<unsigned char>((x + 1) << 1);
            change_freedom(p_forward);

            if(*p_forward == nullptr){
                if(next(p_forward) == nullptr){
                    *memory_first_block_pointer() = nullptr;
                }else{
                    *memory_first_block_pointer() = next(p_forward);
                    *(next(p_forward)) = nullptr;
                }
            }else{
                if(next(p_forward) == nullptr){
                    *next_pointer(reinterpret_cast<void**>(*p_forward)) = nullptr;
                }else{
                    *next_pointer(reinterpret_cast<void**>(*p_forward)) = next(p_forward);
                    *(next(p_forward)) = reinterpret_cast<void**>(*p_forward);
                }
            }

            return merging_blocks(p);

        }else if(p_backward != nullptr && (block_size_power(p_backward) == block_size_power(p) && block_freedom(p_backward))){
            debug_msg = "~MERGING DEBUG~ - BUDDY IS BEHIND, POWER: ";
            debug_msg += to_string(block_size_power(p_backward));
            memory_logger()->log(debug_msg, logger::severity::trace);

            size_t x = block_size_power(p);
            *block_size_power_pointer(p_backward) = static_cast<unsigned char>((x + 1) << 1);
            change_freedom(p);

            if(*p_backward == nullptr){
                if(next(p_backward) == nullptr){
                    *memory_first_block_pointer() = nullptr;
                }else{
                    *memory_first_block_pointer() = next(p_backward);
                    *(next(p_backward)) = nullptr;
                }
            }else{
                if(next(p_backward) == nullptr){
                    *next_pointer(reinterpret_cast<void**>(*p_backward)) = nullptr;
                }else{
                    *next_pointer(reinterpret_cast<void**>(*p_backward)) = next(p_backward);
                    *(next(p_backward)) = reinterpret_cast<void**>(*p_backward);
                }
            }

            return merging_blocks(p_backward);

        }else{
            debug_msg = "~MERGING DEBUG~ - CANNOT MERGE, STOP MERGING!!";
            memory_logger()->log(debug_msg, logger::severity::trace);

            void** p_prev = nullptr;
            void** p_after = memory_first_block();

            while(p_after != nullptr){
                if((long long)(p_after) > (long long)(p)){
                    break;
                }
                p_prev = p_after;
                p_after = next(p_after);
            }

            if(p_prev == nullptr){
                *p = nullptr;
                *memory_first_block_pointer() = reinterpret_cast<void**>(p);
            }else{
                *p = reinterpret_cast<void**>(p_prev);
                *next_pointer(p_prev) = reinterpret_cast<void**>(p);
            }

            if(p_after == nullptr){
                *next_pointer(p) = nullptr;
            }else{
                *next_pointer(p) = reinterpret_cast<void**>(p_after);
                *p_after = reinterpret_cast<void**>(p);
            }

            return p;
        }
    }

public:

    explicit allocator_buddies(size_t size, logger* alloc_logger = nullptr, abstract_allocator* alloc = nullptr){
        size_t dividable_size = 0;
        int power = 0;
        while(size > dividable_size){
            if(dividable_size != 0){
                dividable_size = dividable_size << 1;
                power++;
                if(power > 127) throw logic_error("Constructor error: size is too big!");
            }
            else dividable_size++;
        }
        if(dividable_size < sizeof(void*) + sizeof(void*) + sizeof(unsigned char)) throw logic_error("Constructor error: size is too low!");
        try{
            if(alloc != nullptr){
                _memory = alloc->allocate(dividable_size + sizeof(size_t) + sizeof(logger*) + sizeof(abstract_allocator*) + sizeof(void*));
            }else{
                _memory = ::operator new(dividable_size + sizeof(size_t) + sizeof(logger*) + sizeof(abstract_allocator*) + sizeof(void*));
            }
        }catch(const logic_error& ex){
            throw logic_error("Bad Allocation!");
        }catch(const bad_alloc& ex){
            throw logic_error("Bad Allocation!");
        }

        void* p_memory = _memory;

        *(reinterpret_cast<size_t*>(p_memory)) = dividable_size;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<size_t*>(p_memory) + 1);
        *(reinterpret_cast<logger**>(p_memory)) = alloc_logger;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<logger**>(p_memory) + 1);
        *(reinterpret_cast<abstract_allocator**>(p_memory)) = alloc;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<abstract_allocator**>(p_memory) + 1);
        void** first = reinterpret_cast<void**>(p_memory);
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *first = reinterpret_cast<void**>(p_memory);
        *reinterpret_cast<void**>(p_memory) = nullptr;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *reinterpret_cast<void**>(p_memory) = nullptr;
        p_memory = reinterpret_cast<void*>(reinterpret_cast<void**>(p_memory) + 1);
        *reinterpret_cast<unsigned char*>(p_memory) = static_cast<unsigned char>(power << 1);

        if(memory_logger() != nullptr){
            string msg = "ALLOCATOR CREATED (MEMORY SIZE: ";
            msg += to_string((int)size);
            msg += " BYTES)";
            memory_logger()->log(msg, logger::severity::debug);
        }
    }

    ~allocator_buddies() override {
        void** p = memory_first_block();
        string debug_msg = "DESTRUCTOR DEBUG - FREE BLOCKS' POWERS OF 2: ";
        while(p != nullptr){
            debug_msg += address_to_hex(reinterpret_cast<void*>(p));
            debug_msg += ":[ ";
            debug_msg += to_string(block_size_power(p));
            debug_msg += " ] ";
            p = next(p);
        }
        memory_logger()->log(debug_msg, logger::severity::trace);

        if(memory_logger() != nullptr){
            string msg = "ALLOCATOR DELETED";
            memory_logger()->log(msg, logger::severity::debug);
        }
        if(memory_allocator() == nullptr){
            ::operator delete(_memory);
        }else{
            memory_allocator()->deallocate(_memory);
        }
    }

    allocator_buddies(const allocator_buddies& alloc) = delete;

    allocator_buddies(allocator_buddies&& alloc) = delete;

    allocator_buddies& operator=(const allocator_buddies& alloc) = delete;

    allocator_buddies& operator=(allocator_buddies&& alloc) = delete;

    void* allocate(size_t target_size) const override {
        void** p = memory_first_block();
        void** p_best = nullptr;

        size_t dividable_size = 0;
        int power = 0;
        while(target_size > dividable_size){
            if(dividable_size != 0){
                dividable_size = dividable_size << 1;
                power++;
                if(power > 127) throw logic_error("Bad Alloc: size is too big!");
            }
            else dividable_size++;
        }

        while(p != nullptr){
            if((1 << block_size_power(p)) - sizeof(void*) - sizeof(void*) - sizeof(unsigned char) >= (1 << power)){
                if(p_best == nullptr) p_best = p;
                else{
                    if(((1 << block_size_power(p)) - sizeof(void*) - sizeof(void*) - sizeof(unsigned char) - (1 << power)) < ((1 << block_size_power(p_best)) - sizeof(void*) - sizeof(void*) - sizeof(unsigned char) - (1 << power))) p_best = p;
                }
            }
            p = next(p);
        }

        if(p_best == nullptr) throw logic_error("Bad Alloc!!");
        p = p_best;
        size_t real_size = 0;
        size_t real_power = 0;

        void* result = splitting_block(p, target_size, real_size, real_power);

        string debug_msg = "ALLOCATION DEBUG - FREE BLOCKS' POWERS OF 2 (AFTER ALLOCATION): ";
        void** p1 = memory_first_block();
        while(p1 != nullptr){
            debug_msg += address_to_hex(reinterpret_cast<void*>(p1));
            debug_msg += ":[ ";
            debug_msg += to_string(block_size_power(p1));
            debug_msg += " ] ";
            p1 = next(p1);
        }
        memory_logger()->log(debug_msg, logger::severity::trace);

        if(memory_logger() != nullptr){
            string msg = "Allocation complete, address: ";
            msg += address_to_hex(result);
            msg += ", size: ";
            msg += to_string(target_size);
            msg += ", real size: ";
            msg += to_string(real_size);
            msg += ", power: ";
            msg += to_string(power);
            msg += ", real power: ";
            msg += to_string(real_power);
            memory_logger()->log(msg, logger::severity::debug);
        }

        return result;
    }

    void deallocate(void* target_to_dealloc) const override {
        void** p = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_to_dealloc) - 1) - 2;
        size_t dump_size = ((1 << block_size_power(p)) - sizeof(void*) - sizeof(void*) - sizeof(unsigned char));
        if(!block_freedom(p)) change_freedom(p);

        merging_blocks(p);

        string debug_msg = "DEALLOC DEBUG - FREE BLOCKS' POWERS OF 2 (AFTER DEALLOCATION): ";
        void** p1 = memory_first_block();
        while(p1 != nullptr){
            debug_msg += address_to_hex(reinterpret_cast<void*>(p1));
            debug_msg += ":[ ";
            debug_msg += to_string(block_size_power(p1));
            debug_msg += " ] ";
            p1 = next(p1);
        }
        memory_logger()->log(debug_msg, logger::severity::trace);

        if(memory_logger() != nullptr){
            string msg = "Deallocation complete, address: ";
            msg += address_to_hex(target_to_dealloc);
            msg += ", size: ";
            msg += to_string(dump_size);
            msg += ", dump: ";
            msg += memory_dump(target_to_dealloc, dump_size);
            memory_logger()->log(msg, logger::severity::debug);
        }
    }

};


#endif //ALLOCATOR_BUDDIES_ALLOCATOR_BUDDIES_H
