#ifndef COURSEWORK_DATABASE_H
#define COURSEWORK_DATABASE_H
#include "../red_black_tree/red_black_tree.h"
#include "../allocator_1/allocator_1.h"
#include "../allocator_2/allocator_2.h"
#include "../allocator_3/allocator_3.h"
#include "../allocator_buddies/allocator_buddies.h"
#include "../avl_tree/avl_tree.h"


////TODO: SIMPLE INSERT/REMOVE/READ !!


class database final{

    ////* ------------------------------ ENUM FIELD ------------------------------ *////

public:

    enum class names_of_containers{
        avl_tree,
        red_black_tree
    };

    enum class names_of_allocators{
        global_heap,
        sorted_list,
        border_descriptor,
        buddies
    };

    enum class allocation_mode{
        first,
        best,
        worst,
        none
    };

    ////* ------------------------------ STRUCTS FIELD ------------------------------ *////

public:      ////TODO: TESTING, RETURN AFTER TEST!!

    struct pipeline_key final
    {
        int id;
        string build_version;
    };

    struct pipeline_commit_info final
    {
        string hash;
        string developer_login;
        string developer_email;

        bool operator==(const pipeline_commit_info& other) const {
            if(this->hash == other.hash && this->developer_login == other.developer_login && this->developer_email == other.developer_email) return true;
            else return false;
        }

        bool operator!=(const pipeline_commit_info& other) const {
            return !(*this == other);
        }
    };

    struct pipeline_value final
    {
        pipeline_commit_info commit_info;
        string assembly_script_path;
        string assembly_name;
        string build_errors;
        string static_code_analysis_errors;
        string autotest_run_errors;
        string assembly_artifacts_dir;

        bool operator==(const pipeline_value& other) const {
            if(this->commit_info == other.commit_info &&
               this->assembly_script_path == other.assembly_script_path &&
               this->assembly_name == other.assembly_name &&
               this->build_errors == other.build_errors &&
               this->static_code_analysis_errors == other.static_code_analysis_errors &&
               this->autotest_run_errors == other.autotest_run_errors &&
               this->assembly_artifacts_dir == other.assembly_artifacts_dir) return true;
            else return false;
        }

        bool operator!=(const pipeline_value& other) const {
            return !(*this == other);
        }
    };

    ////* ------------------------------ COMPARATORS FIELD ------------------------------ *////

private:

    class string_comparer final
    {
    public:
        string_comparer() = default;
        int operator()(const string& s1, const string& s2) const {
            return s1.compare(s2);
        }
    };

    class pipeline_key_comparer final
    {
    public:
        pipeline_key_comparer() = default;
        int operator()(const pipeline_key& k1, const pipeline_key& k2) const {
            int res = k1.id - k2.id;
            if(res != 0) return res;
            else return k1.build_version.compare(k2.build_version);
        }
    };

    ////* ------------------------------ DATABASE POINTERS FIELD ------------------------------ *////

private:

    red_black_tree <string,   //database
                    red_black_tree <string,   //pull
                                    red_black_tree <string,   //scheme
                                                    associative_container <pipeline_key, pipeline_value>*,   //collection
                                                   string_comparer>*,
                                   string_comparer>*,
                   string_comparer>*
    _database;

    logger* _logger;
    allocator_1* _database_allocator;
    red_black_tree<string, abstract_allocator*, string_comparer>* _pull_allocators;


    ////* ------------------------------ CONSTRUCTORS FIELD ------------------------------ *////

public:

    explicit database(logger* l):
        _logger(l)
    {
        _database_allocator = new allocator_1();
        _database = new red_black_tree
                                      <string,                                                 //database
                                      red_black_tree <string,                                  //pull
                                      red_black_tree <string,                                  //scheme
                                      associative_container <pipeline_key, pipeline_value>*,   //collection
                                      string_comparer>*,
                                      string_comparer>*,
                                      string_comparer>(_database_allocator, _logger);

        _pull_allocators = new red_black_tree<string, abstract_allocator*, string_comparer>(_database_allocator, _logger);
        _logger->log("--- DATABASE CREATED! ---", logger::severity::information);
    }

    database(const database& other) = delete;
    database(database&& other) = delete;
    database& operator=(const database& other) = delete;
    database& operator=(database&& other) = delete;

    ////* ------------------------------ DESTRUCTOR FIELD ------------------------------ *////

public:

    ~database(){

        auto iter = _database->begin_postf();
        auto end_iter = _database->end_postf();
        for(; iter != end_iter; ++iter){
            auto pull_name = get<0>(*iter);
            delete_pull(pull_name);
        }

        delete _pull_allocators;
        delete _database;
        delete _database_allocator;

        _logger->log("--- DATABASE DELETED! ---", logger::severity::information);
    }

    ////* ------------------------------ RUN_FILE_COMMANDS FIELD ------------------------------ *////

private:

    void parsing_command(const string& command){
        ////TODO: !!
    }

public:

    void run_file_commands(const string& path){
        ifstream fin;
        fin.open(path);
        if(!fin.is_open()){
            throw logic_error("Could not open a file!");
        }
        string line;
        while(getline(fin, line)){
            try{
                parsing_command(line);
            }catch(const logic_error& ex){
                _logger->log(ex.what(), logger::severity::information);
                break;
            }
        }
        fin.close();
        ////TODO:
    }

    ////* ------------------------------ DIALOG FIELD ------------------------------ *////

public:

    void start_dialog(){
        ////TODO: !!
    }

    ////* ------------------------------ COMMANDS FIELD ------------------------------ *////

public:   ////TODO: TESTING, RETURN AFTER TEST!!

    void insert(const pipeline_key& key, const pipeline_value& value, const string& pull_name, const string& scheme_name, const string& collection_name){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;
        red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>* scheme;
        associative_container<pipeline_key, pipeline_value>* collection;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            scheme = pull->get(scheme_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: scheme '";
            error_msg += scheme_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            collection = scheme->get(collection_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: collection '";
            error_msg += collection_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            collection->insert(key, value);
        }catch(const logic_error& ex){
            string error_msg = "Error: in collection '";
            error_msg += collection_name;
            error_msg += "': key is already exists!";
            throw logic_error(error_msg);
        }

        _logger->log("INSERT DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void read_key(const pipeline_key& key, pipeline_value& value, const string& pull_name, const string& scheme_name, const string& collection_name){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;
        red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>* scheme;
        associative_container<pipeline_key, pipeline_value>* collection;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            scheme = pull->get(scheme_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: scheme '";
            error_msg += scheme_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            collection = scheme->get(collection_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: collection '";
            error_msg += collection_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            value = collection->get(key);
        }catch(const logic_error& ex){
            string error_msg = "Error: in collection '";
            error_msg += collection_name;
            error_msg += "': key is not found!";
            throw logic_error(error_msg);
        }

        _logger->log("READ DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void read_range(const pipeline_key& a, const pipeline_key& b, vector<pipeline_value>& values){
        ////TODO:
    }

    void update_key(const pipeline_key& key, const pipeline_value& value, const string& pull_name, const string& scheme_name, const string& collection_name){
        try{
            remove(key, pull_name, scheme_name, collection_name);
            insert(key, value, pull_name, scheme_name, collection_name);
        }catch(const logic_error& ex){
            throw logic_error(ex.what());
        }

        _logger->log("UPDATE DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void remove(const pipeline_key& key, const string& pull_name, const string& scheme_name, const string& collection_name){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;
        red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>* scheme;
        associative_container<pipeline_key, pipeline_value>* collection;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            scheme = pull->get(scheme_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: scheme '";
            error_msg += scheme_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            collection = scheme->get(collection_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: collection '";
            error_msg += collection_name;
            error_msg += "' is not found!";
            throw logic_error(error_msg);
        }

        try{
            collection->remove(key);
        }catch(const logic_error& ex){
            string error_msg = "Error: in collection '";
            error_msg += collection_name;
            error_msg += "': key is not found!";
            throw logic_error(error_msg);
        }

        _logger->log("REMOVE DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void add_pull(const string& pull_name, names_of_allocators allocator_name, size_t size, allocation_mode mode = allocation_mode::none){
        bool flag = false;

        try{_database->get(pull_name);}catch(const logic_error& ex){flag = true;}
        if(!flag){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' is already exists!";
            throw logic_error(error_msg);
        }

        abstract_allocator* alloc;
        if(allocator_name == names_of_allocators::sorted_list){
            if(mode == allocation_mode::first) alloc = new allocator_2(size, allocator_2::mode::first);
            else if(mode == allocation_mode::best) alloc = new allocator_2(size, allocator_2::mode::best);
            else if(mode == allocation_mode::worst) alloc = new allocator_2(size, allocator_2::mode::worst);
            else alloc = new allocator_1();
        }else if(allocator_name == names_of_allocators::border_descriptor){
            if (mode == allocation_mode::first) alloc = new allocator_3(size, allocator_3::mode::first);
            else if (mode == allocation_mode::best) alloc = new allocator_3(size, allocator_3::mode::best);
            else if (mode == allocation_mode::worst) alloc = new allocator_3(size, allocator_3::mode::worst);
            else alloc = new allocator_1();
        }else if(allocator_name == names_of_allocators::buddies){
            alloc = new allocator_buddies(size);
        }else{
            alloc = new allocator_1();
        }

        auto* tree = reinterpret_cast<red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>*>(alloc->allocate(reinterpret_cast<size_t>(sizeof(red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>))));
        new (tree) red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>(alloc, _logger);

        _database->insert(pull_name, tree);

        _pull_allocators->insert(pull_name, alloc);

        _logger->log("ADD PULL DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void delete_pull(const string& pull_name){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        auto iter = pull->begin_postf();
        auto end_iter = pull->end_postf();
        for(; iter != end_iter; ++iter){
            auto scheme_name = get<0>(*iter);
            delete_scheme(scheme_name, pull_name);
        }

        auto* alloc = _pull_allocators->get(pull_name);
        pull->~red_black_tree();
        alloc->deallocate(reinterpret_cast<void*>(pull));

        _pull_allocators->remove(pull_name);
        delete alloc;

        _database->remove(pull_name);

        _logger->log("DELETE PULL DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void add_scheme(const string& scheme_name, const string& pull_name){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        bool flag = false;
        try{pull->get(scheme_name);}catch(const logic_error& ex){flag = true;}
        if(!flag){
            string error_msg = "Error: scheme '";
            error_msg += scheme_name;
            error_msg += "' is already exists!";
            throw logic_error(error_msg);
        }

        auto* alloc = _pull_allocators->get(pull_name);

        auto* tree = reinterpret_cast<red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*>(alloc->allocate(reinterpret_cast<size_t>(sizeof(red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>))));
        new (tree) red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>(alloc, _logger);

        pull->insert(scheme_name, tree);

        _logger->log("ADD SCHEME DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void delete_scheme(const string& scheme_name, const string& pull_name){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;
        red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>* scheme;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        try{
            scheme = pull->get(scheme_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: scheme '";
            error_msg += scheme_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        auto iter = scheme->begin_postf();
        auto end_iter = scheme->end_postf();
        for(; iter != end_iter; ++iter){
            auto collection_name = get<0>(*iter);
            delete_collection(collection_name, pull_name, scheme_name);
        }

        auto* alloc = _pull_allocators->get(pull_name);
        scheme->~red_black_tree();
        alloc->deallocate(reinterpret_cast<void*>(scheme));

        pull->remove(scheme_name);

        _logger->log("DELETE SCHEME DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void add_collection(const string& collection_name, const string& pull_name, const string& scheme_name, names_of_containers type){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;
        red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>* scheme;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        try{
            scheme = pull->get(scheme_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: scheme '";
            error_msg += scheme_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        bool flag = false;
        try{scheme->get(collection_name);}catch(const logic_error& ex){flag = true;}
        if(!flag){
            string error_msg = "Error: collection '";
            error_msg += collection_name;
            error_msg += "' is already exists!";
            throw logic_error(error_msg);
        }

        auto* alloc = _pull_allocators->get(pull_name);

        if(type == names_of_containers::avl_tree){
            auto* tree = reinterpret_cast<avl_tree<pipeline_key, pipeline_value, pipeline_key_comparer>*>(alloc->allocate(reinterpret_cast<size_t>(sizeof(avl_tree<pipeline_key, pipeline_value, pipeline_key_comparer>))));
            new (tree) avl_tree<pipeline_key, pipeline_value, pipeline_key_comparer>(alloc, _logger);

            scheme->insert(collection_name, tree);
        }else{
            auto* tree = reinterpret_cast<red_black_tree<pipeline_key, pipeline_value, pipeline_key_comparer>*>(alloc->allocate(reinterpret_cast<size_t>(sizeof(red_black_tree<pipeline_key, pipeline_value, pipeline_key_comparer>))));
            new (tree) red_black_tree<pipeline_key, pipeline_value, pipeline_key_comparer>(alloc, _logger);

            scheme->insert(collection_name, tree);
        }

        _logger->log("ADD COLLECTION DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

    void delete_collection(const string& collection_name, const string& pull_name, const string& scheme_name){
        red_black_tree<string, red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>*, string_comparer>* pull;
        red_black_tree<string, associative_container<pipeline_key, pipeline_value>*, string_comparer>* scheme;
        associative_container<pipeline_key, pipeline_value>* collection;

        try{
            pull = _database->get(pull_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: pull '";
            error_msg += pull_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        try{
            scheme = pull->get(scheme_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: scheme '";
            error_msg += scheme_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        try{
            collection = scheme->get(collection_name);
        }catch(const logic_error& ex){
            string error_msg = "Error: collection '";
            error_msg += collection_name;
            error_msg += "' not found!";
            throw logic_error(error_msg);
        }

        auto* alloc = _pull_allocators->get(pull_name);
        collection->~associative_container();
        alloc->deallocate(reinterpret_cast<void*>(collection));

        scheme->remove(collection_name);

        _logger->log("DELETE COLLECTION DONE", logger::severity::information);     ////TODO: TESTING, RETURN AFTER TEST!!
    }

};


#endif //COURSEWORK_DATABASE_H
