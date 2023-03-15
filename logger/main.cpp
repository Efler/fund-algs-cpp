#include <iostream>
#include "logger_builder.h"

int main(){
    auto* builder = new logger_builder();
    builder->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger1.txt", logger::severity::information)
            ->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger2.txt", logger::severity::critical)
            ->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger3.txt", logger::severity::trace);
    logger* logger_1 = builder->build();
    logger* logger_2 = builder
            ->add_stream("D:\\JetBrains\\tester_files_(yan)\\logger4.txt", logger::severity::trace)
            ->add_stream("console", logger::severity::debug)
            ->build();
    logger_1->log("darova", logger::severity::information);
    logger_2->log("nihua sebe", logger::severity::debug);

    delete logger_1;
    delete logger_2;
    delete builder;
}