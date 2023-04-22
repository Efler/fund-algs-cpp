#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;


class logger final
{
    friend class logger_builder;
    friend class logger_config;

public:

    enum class severity{
        trace,
        debug,
        information,
        warning,
        error,
        critical
    };

    const map<severity, string> str_severity = {
            {severity::trace, "trace"},
            {severity::debug, "debug"},
            {severity::information, "information"},
            {severity::warning, "warning"},
            {severity::error, "error"},
            {severity::critical, "critical"},
    };

private:

    map<string, pair<ofstream*, severity>> _logger_streams;
    static map<string, pair<ofstream*, size_t>> _streams;

public:

    explicit logger(const map<string, severity>& streams)
    {
        for(pair<string, severity> st : streams){
            if(_streams.empty()){
                auto* stream = new ofstream;
                if(st.first != "console"){
                    stream->open(st.first, ofstream::app);
                    if(!stream->is_open()){
                        throw logic_error("hi");
                    }
                }else{
                    stream = nullptr;
                }
                _streams[st.first] = {stream, 1};
                _logger_streams[st.first] = {stream, st.second};
            }else{
                auto iter = _streams.find(st.first);
                if(iter != _streams.end()){
                    iter->second.second++;
                    _logger_streams[st.first] = {iter->second.first, st.second};
                }else{
                    auto* stream = new ofstream;
                    if(st.first != "console"){
                        stream->open(st.first, ofstream::app);
                        if(!stream->is_open()){
                            throw logic_error("Could not open a file!");
                        }
                    }else{
                        stream = nullptr;
                    }
                    _streams[st.first] = {stream, 1};
                    _logger_streams[st.first] = {stream, st.second};
                }
            }
        }
    }

    explicit logger(const string& config){
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
            if(ch == EOF || ch == ' ' || ch == '\n'){
                if(flag == 0){
                    flag++;
                }else{
                    flag = 0;

                }
            }else{
                if(flag == 0){
                    path += ch;
                }else{
                    severity += ch;
                }
            }
        }
    }

    logger(logger const & other) = delete;

    logger(logger && other) = delete;

    logger& operator=(logger const & other) = delete;

    logger& operator=(logger && other) = delete;

    ~logger(){
        for(pair <string, pair<ofstream*, severity>> st : _logger_streams){
            if(_streams[st.first].second > 1){
                _streams[st.first].second--;
            }else{
                if(_streams[st.first].first != nullptr){
                    _streams[st.first].first->close();
                }
                delete _streams[st.first].first;
                _streams.erase(st.first);
            }
        }
    }

    static string current_datetime_to_string()
    {
        auto time = std::time(nullptr);

        std::ostringstream result_stream;
        result_stream << put_time(localtime(&time), "%d.%m.%Y %H:%M:%S");

        return result_stream.str();
    }

    logger const* log(const string &to_log, severity severity) const{
        for(auto& st : _logger_streams){
            if (st.second.second > severity){
                continue;
            }
            if (st.second.first == nullptr)
            {
                cout << "[" << current_datetime_to_string() << "]" << "[" << str_severity.at(severity) << "] " << to_log << endl;
            }
            else
            {
                (*st.second.first) << "[" << current_datetime_to_string() << "]" << "[" << str_severity.at(severity) << "] " << to_log << endl;
            }
        }
        return this;
    }

};

map<string, pair<ofstream*, size_t>> logger::_streams = map<string, pair<ofstream*, size_t>>();


#endif //LOGGER_LOGGER_H