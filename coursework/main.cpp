#include <iostream>
#include "../allocator_1/allocator_1.h"
#include "../allocator_2/allocator_2.h"
#include "../allocator_3/allocator_3.h"
#include "../allocator_buddies/allocator_buddies.h"
#include "../red_black_tree/red_black_tree.h"


    ////* ------------------------------ STRUCTS FIELD ------------------------------ *////

struct pipeline_key final     ////TODO: STRUCTS ?
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

class string_comparer final
{
public:
    string_comparer() = default;
    int operator()(const string& s1, const string& s2) const {
        return s1.compare(s2);
    }
};

class pipeline_key_comparer final       ////TODO: LIKE THIS ?
{
public:
    pipeline_key_comparer() = default;
    int operator()(const pipeline_key& k1, const pipeline_key& k2) const {
        int res = k1.id - k2.id;
        if(res != 0) return res;
        else return k1.build_version.compare(k2.build_version);
    }
};

    ////* ------------------------------ MAIN FIELD ------------------------------ *////

int main(){     ////TODO: ARCHITECTURE ?

    auto* builder = new logger_builder();
    logger* logger = builder->add_stream("console", logger::severity::debug)->build();

    logger->log("----- STARTING DATABASE ------", logger::severity::information);  ////TODO: THROUGH LOGGER ?
    auto* database = new red_black_tree<string, red_black_tree< string, red_black_tree< string, red_black_tree< pipeline_key, pipeline_value, pipeline_key_comparer >*, string_comparer >*, string_comparer >*, string_comparer>;
    ////TODO: REDBLACK defining redunant ?

}