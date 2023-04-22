#ifndef LOGGER_LOGGER_CONFIG_H
#define LOGGER_LOGGER_CONFIG_H
#include "logger.h"

class logger_config final
{

private:

    map <string, logger::severity> _streams_logger;

public:

    const map<string, logger::severity> sev_severity = {
            {"trace", logger::severity::trace},
            {"debug", logger::severity::debug},
            {"information", logger::severity::information},
            {"warning", logger::severity::warning},
            {"error", logger::severity::error},
            {"critical", logger::severity::critical},
    };

    virtual ~logger_config() = default;

    logger* configurate(const string& config){
        ifstream conf;
        size_t flag = 0;
        string path = "";
        string severity = "";
        char ch = '@';
        conf.open(config);
        if(!conf.is_open()){
            throw logic_error("Could not open a file!");
        }
        while(!conf.eof()){
            conf.get(ch);
            if(ch == ' ' || ch == '\n'){
                if(flag == 0){
                    flag++;
                }else{
                    flag = 0;
                    try{
                        _streams_logger[path] = sev_severity.at(severity);
                    }catch(std::out_of_range& ex){
                        conf.close();
                        throw logic_error("Wrong severity!");
                    }
                    path = "";
                    severity = "";
                    ch = '@';
                }
            }else{
                if(flag == 0){
                    path += ch;
                }else{
                    severity += ch;
                }
            }
        }
        conf.close();
        return new logger(_streams_logger);
    }

};


#endif //LOGGER_LOGGER_CONFIG_H