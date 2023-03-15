#include <iostream>
#include "logger_builder.h"

int main(){

    logger* logger_1 = nullptr;
    logger* logger_2 = nullptr;
    logger* logger_3 = nullptr;

    auto* builder = new logger_builder();
    builder->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger1.txt", logger::severity::information)
            ->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger2.txt", logger::severity::critical)
            ->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger3.txt", logger::severity::trace);
    try{
        logger_1 = builder->build();
    }catch(const logic_error& ex){
        cout << ex.what() << endl;
        delete builder;
        return 0;
    }

    try{
        logger_2 = builder
                ->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger4.txt", logger::severity::trace)
                ->add_stream("console", logger::severity::debug)
                ->build();
    }catch(const logic_error& ex){
        cout << ex.what() << endl;
        delete logger_1;
        delete builder;
        return 0;
    }


    auto* config = new logger_config();
    try{
        logger_3 = config->configurate("D:\\JetBrains\\tester_files_(yan)\\logger_config_1.txt");
    }catch(const logic_error& ex){
        cout << ex.what() << endl;
        delete logger_1;
        delete logger_2;
        delete builder;
        delete config;
        return 0;
    }

    logger_1->log("darova", logger::severity::information);
    logger_2->log("nihua sebe", logger::severity::debug);
    logger_3->log("dada ya!", logger::severity::warning);

    delete logger_1;
    delete logger_2;
    delete logger_3;
    delete builder;
    delete config;
}