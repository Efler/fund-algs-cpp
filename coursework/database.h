#ifndef COURSEWORK_DATABASE_H
#define COURSEWORK_DATABASE_H
#include "../red_black_tree/red_black_tree.h"
#include "../allocator_1/allocator_1.h"
#include "../allocator_2/allocator_2.h"
#include "../allocator_3/allocator_3.h"
#include "../allocator_buddies/allocator_buddies.h"
#include "../avl_tree/avl_tree.h"


class database final{

    ////* ------------------------------ STRUCTS FIELD ------------------------------ *////

private:

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
    allocator_1* _allocator;

    ////* ------------------------------ CONSTRUCTORS FIELD ------------------------------ *////

public:

    explicit database(logger* l):
        _logger(l)
    {
        _allocator = new allocator_1();
        _database = new red_black_tree
                                      <string,                                                 //database
                                      red_black_tree <string,                                  //pull
                                      red_black_tree <string,                                  //scheme
                                      associative_container <pipeline_key, pipeline_value>*,   //collection
                                      string_comparer>*,
                                      string_comparer>*,
                                      string_comparer>(_allocator, _logger);

        _logger->log("--- DATABASE CREATED! ---", logger::severity::information);
    }

    database(const database& other) = delete;
    database(database&& other) = delete;
    database& operator=(const database& other) = delete;
    database& operator=(database&& other) = delete;

    ////* ------------------------------ DESTRUCTOR FIELD ------------------------------ *////

public:

    ~database(){
        auto begin_database_it = _database->begin_postf();
        auto end_database_it = _database->end_postf();
        for(; begin_database_it != end_database_it; ++begin_database_it){

            auto* pull = std::get<1>(*begin_database_it);
            auto begin_pull_it = pull->begin_postf();
            auto end_pull_it = pull->end_postf();
            for(; begin_pull_it != end_pull_it; ++begin_pull_it){

                auto* scheme = std::get<1>(*begin_pull_it);
                auto begin_scheme_it = scheme->begin_postf();
                auto end_scheme_it = scheme->end_postf();
                for(; begin_scheme_it != end_scheme_it; ++begin_scheme_it){

                    auto* collection = std::get<1>(*begin_scheme_it);
                    delete collection;
                    string msg = "COLLECTION '";
                    msg += std::get<0>(*begin_scheme_it);
                    msg += "' DELETED!";
                    _logger->log(msg, logger::severity::information);
                }
                delete scheme;
                string msg = "SCHEME '";
                msg += std::get<0>(*begin_pull_it);
                msg += "' DELETED!";
                _logger->log(msg, logger::severity::information);
            }
            delete pull;
            string msg = "PULL '";
            msg += std::get<0>(*begin_database_it);
            msg += "' DELETED!";
            _logger->log(msg, logger::severity::information);
        }

        delete _database;
        delete _allocator;
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

private:

    void insert(const pipeline_key& key, const pipeline_value& value){
        ////TODO:
    }

    void read_key(const pipeline_key& key, pipeline_value& value){
        ////TODO:
    }

    void read_range(const pipeline_key& a, const pipeline_key& b, vector<pipeline_value>& values){
        ////TODO:
    }

    void update_key(const pipeline_key& key, const pipeline_key& value){
        ////TODO:
    }

    void remove(const pipeline_key& key){
        ////TODO:
    }

    void add_pull(const string& name){
        ////TODO:
    }

    void delete_pull(const string& name){
        ////TODO:
    }

    void add_scheme(const string& name, const string& pull_name){
        ////TODO:
    }

    void delete_scheme(const string& name, const string& pull_name){
        ////TODO:
    }

    void add_collection(const string& name, const string& pull_name, const string& scheme_name){
        ////TODO:
    }

    void delete_collection(const string& name, const string& pull_name, const string& scheme_name){
        ////TODO:
    }

};


#endif //COURSEWORK_DATABASE_H
