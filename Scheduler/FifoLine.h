//
//  FifoLine.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#ifndef SHEDULER_FIFO_LINE_H
#define SHEDULER_FIFO_LINE_H

#include <string>

#include "MessageLine.h"

namespace communication {
    
    //This class implements posix fifo (named pipe) communication
    //Works in separate thread
    template<typename MessageParserType>
    class FifoLine :    public MessageLine<MessageParserType>,
                        public common::Thread
    {
    public:
        virtual bool open(const ::std::string &id) override;
        virtual bool close() override;
        
        virtual ::std::string lineId() const override { return id_; }
        
    private:
        virtual void run() override;
        
    private:
        pthread_t thread_;
        ::std::string id_;
    };
    
}   //communication

#include "FifoLine.ipp"

#endif //SHEDULER_FIFO_LINE_H
