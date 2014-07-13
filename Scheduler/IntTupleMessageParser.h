//
//  IntTupleMessageParser.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#ifndef SCHEDULER_INTTUPLEMESSAGEPARSER_H
#define SCHEDULER_INTTUPLEMESSAGEPARSER_H

#include <string>
#include <tuple>

#include "MessageParser.h"

namespace parser {
    
    typedef ::std::tuple<int, int> IntTuple;
    
    //implements parsing of int tuple messages
    //(7 6) (8 9) (20 1)
    class IntTupleMessageParser : public MessageParser< IntTuple >
    {
    protected:
        virtual bool parse(const ::std::string &str_message) override;
    };
    
} //namespace parser

#endif //SCHEDULER_INTTUPLEMESSAGEPARSER_H
