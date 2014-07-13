//
//  MessageParser.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#ifndef SHEDULER_MESSAGE_PARSER_H
#define SHEDULER_MESSAGE_PARSER_H

namespace parser {
    
    //virtual base class for parsing messages
    template<typename MessageType>
    class MessageParser
    {
        
    protected:
        virtual ~MessageParser() { }
        
        virtual bool parse(const ::std::string &str_message) = 0;
        virtual void onMessageParsed(const MessageType &message) = 0;
    };
    
}

#endif  //SHEDULER_MESSAGE_PARSER_H
