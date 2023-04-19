#include "logger_config.h"

class logger_builder final
{

private:

    map <string, logger::severity> _streams_for_logger;

public:

    virtual ~logger_builder() = default;

    logger_builder* add_stream(const string& path, logger::severity severity){
        _streams_for_logger[path] = severity;
        return this;
    }

    logger* build() const{
        return new logger(_streams_for_logger);
    }

};