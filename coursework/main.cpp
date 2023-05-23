#include <iostream>
#include "database.h"


    ////* ------------------------------ MAIN FIELD ------------------------------ *////

int main(int argc, char* argv[]){
    auto* builder = new logger_builder();
    logger* logger = builder->add_stream("console", logger::severity::warning)->build();

    auto* db = new database(logger);

//------------------------------------
//little tests

//    db->parsing_command("insert: [pool_name] [dadada]", "lol", "wow");


//    string s = "12332";
//    for(char c : s) if(!isdigit(c)) cout << "gay" << endl;
//    cout << "hope i dont gay" << endl;


//    database::pipeline_value v = db->parsing_value("value: [1234!] [eflerr!] [lol@mail.ru] [assembly path!] [assembly name!] [build errors!] [static!] [autotest!] [assembly dir!]");
//    cout << v.commit_info.developer_login << " " << v.build_errors << " " << v.assembly_artifacts_dir << endl;


//    try{ db->add_pool("mypool", database::names_of_allocators::buddies, 50000); } catch(const logic_error& ex){ cout << ex.what() << endl; }
//    try{ db->add_scheme("my scheme", "mypool"); } catch(const logic_error& ex){ cout << ex.what() << endl; }
//    try{ db->add_collection("Collection", "mypool", "my scheme", database::names_of_containers::red_black_tree); } catch(const logic_error& ex){ cout << ex.what() << endl; }
//
//    try{ db->run_file_commands("D:\\JetBrains\\tester_files_(yan)\\coursework_commands.txt"); } catch(const logic_error& ex){ cout << ex.what() << endl; }
//
//    database::pipeline_key k = {44, "ny tipa build"};
//    database::pipeline_value v;
//    try{ db->read_key(k, v, "mypool", "my scheme", "Collection"); } catch(const logic_error& ex){ cout << ex.what() << endl; }
//
//    cout << v.commit_info.developer_login << " " << v.static_code_analysis_errors << " " << v.assembly_artifacts_dir << endl;

//-----------------------------------
//testing methods

    try{ db->run_file_commands("D:\\JetBrains\\tester_files_(yan)\\coursework_commands.txt"); } catch(const logic_error& ex){ cout << ex.what() << endl; }


//------------------------------------
//testing commands

//    try{
//        db->add_pool("my pool", database::names_of_allocators::border_descriptor, 5000, database::allocation_mode::best);
//        cout << "add pull done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//    try{
//        db->add_scheme("my scheme", "my pool");
//        cout << "add scheme done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//    try{
//        db->add_collection("MY COLLECTION", "my pool", "my scheme", database::names_of_containers::avl_tree);
//        cout << "add collection done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//
//
//    database::pipeline_key k = {44, "version 44.02"};
//    database::pipeline_commit_info i = {
//            "4q5hgwrthwrth",
//            "eflerrr",
//            "yanchello@mail.ru"
//
//    };
//    database::pipeline_value v = {
//            i,
//            "R//some_path",
//            "LISABETH",
//            "wrong name!",
//            "wrong code!",
//            "WRONG AUTOMOBILE!",
//            "R//directory_wow"
//
//    };
//
//    try{
//        db->insert(k, v, "my pool", "my scheme", "MY COLLECTION");
//        cout << "insert done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//
//
//    database::pipeline_key k2 = {58, "version 58.01"};
//    database::pipeline_commit_info i2 = {
//            "2__adfgahdfgdfga",
//            "2__eflerrr",
//            "2__yanchello@mail.ru"
//
//    };
//    database::pipeline_value v2 = {
//            i2,
//            "2__R//some_path",
//            "2__LISABETH",
//            "2__wrong name!",
//            "2__wrong code!",
//            "2__WRONG AUTOMOBILE!",
//            "2__R//directory_wow"
//
//    };
//
//    try{
//        db->insert(k2, v2, "my pool", "my scheme", "MY COLLECTION");
//        cout << "insert done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//
//    database::pipeline_value v3;
//
//    try{
//        db->read_key(k, v3, "my pool", "my scheme", "MY COLLECTION");
//        cout << "read key done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//    cout << v3.commit_info.developer_login << " " << v3.commit_info.developer_email << " " << v3.assembly_artifacts_dir << endl;
//
//
//    try{
//        db->update_key(k, v2, "my pool", "my scheme", "MY COLLECTION");
//        cout << "update key done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//    try{
//        db->read_key(k, v3, "my pool", "my scheme", "MY COLLECTION");
//        cout << "read key done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//    cout << v3.commit_info.developer_login << " " << v3.commit_info.developer_email << " " << v3.assembly_artifacts_dir << endl;
//
//
//    try{
//        db->remove(k, "my pool", "my scheme", "MY COLLECTION");
//        cout << "remove done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//
//    try{
//        db->read_key(k, v3, "my pool", "my scheme", "MY COLLECTION");
//        cout << "read key done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }
//
//
//    try{
//        db->delete_collection("MY COLLECTION", "my pool", "my scheme");
//        cout << "delete collection done" << endl;
//    }catch(const logic_error& ex){
//        cout << ex.what() << endl;
//    }



    delete db;
    delete logger;
    delete builder;
}