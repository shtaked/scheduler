//
//  FifoLine.ipp
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#include <string>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sstream>

namespace communication {
    
    template <typename MessageParserType>
    bool FifoLine<MessageParserType>::open(const ::std::string &id)
    {
        id_ = id;
        
        int res = mkfifo(id_.c_str(), 0777);
        if (res == -1 && errno != EEXIST) {
            ::std::ostringstream out_stream;
            out_stream << "Error opening fifo line (" << id_ << "): " << errno << ::std::endl;
            common::log(out_stream.str());

            return false;
        }
        
        Thread::start();
        return true;
    }
    
    template <typename MessageParserType>
    bool FifoLine<MessageParserType>::close()
    {
        return true;
    }
    
    template <typename MessageParserType>
    void FifoLine<MessageParserType>::run()
    {
        int fd;
        
        fd = ::open(id_.c_str(), O_RDONLY);
        if (fd == -1) {
            ::std::ostringstream out_stream;
            out_stream << "Error opening fifo line (" << id_ << "): " << errno << ::std::endl;
            common::log(out_stream.str());

            return;
        }
        
        ::std::ostringstream out_stream;
        out_stream << "Fifo line '" << id_ << "' was opened" << ::std::endl;
        common::log(out_stream.str());
        
        ::std::string current_message;
        
        char s = 0;
        bool expecting_opened = true;
        while (read(fd, &s, 1) != -1) {
            if (s == '(') {
                current_message.clear();
                expecting_opened = false;
            } else if (s == ')') {
                ::std::ostringstream out_stream;
                out_stream << "Fifo line '" << id_ << "' got message '" << current_message << "'" << ::std::endl;
                common::log(out_stream.str());
                MessageLine<MessageParserType>::onMessageReseived(current_message);
                expecting_opened = true;
            } else if (!expecting_opened) {
                current_message += s;
            }
        }
    }
    
}   //communication