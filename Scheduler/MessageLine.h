//
//  MessageLine.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#ifndef MESSAGELINE_H
#define MESSAGELINE_H

#include <string>
#include <tuple>
#include <sstream>

#include "Common.h"

namespace communication {

    //Base class for all comunication lines
    //Template argument used for parsing different kinds of messages
    template<typename MessageParserType>
    class MessageLine : public MessageParserType
    {
        
    public:
        virtual ~MessageLine() { }
        
        //every line use each own identificator
        virtual bool open(const ::std::string &id) = 0;
        virtual bool close() = 0;
        
        virtual ::std::string lineId() const = 0;
        
    protected:
        //this message should be called from classes that inherit this and implement
        //communication logic (FifoLine for example)
        virtual void onMessageReseived(const ::std::string &message) {
            if (!MessageParserType::parse(message)) {
                ::std::ostringstream out_stream;
                
                out_stream << "Can not parse input message '" << message
                << "' in '" << lineId() << "' line. Dropped." << ::std::endl;
                
                common::log(out_stream.str());
            }
        }
        
    };
    
} //namespace communication

#endif //MESSAGELINE_H
