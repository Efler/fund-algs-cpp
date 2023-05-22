#include <iostream>
#include "database.h"


    ////* ------------------------------ MAIN FIELD ------------------------------ *////

int main(int argc, char* argv[]){
    auto* builder = new logger_builder();
    logger* logger = builder->add_stream("console", logger::severity::debug)->build();

    auto* db = new database(logger);
    delete db;

    delete logger;
    delete builder;
}