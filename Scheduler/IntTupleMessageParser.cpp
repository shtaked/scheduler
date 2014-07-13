//
//  IntTupleMessageParser.cpp
//  Scheduler
//
//  Created by Евгений Койро on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#include "IntTupleMessageParser.h"

#include <sstream>
#include <queue>

namespace parser {
    
    bool IntTupleMessageParser::parse(const ::std::string &str_message)
    {
        ::std::queue<int> data;
        
        ::std::string temp_str;
        ::std::istringstream istream(str_message);
        
        while (!::std::getline(istream, temp_str, ' ').fail())
        {
            int element = 0;
            ::std::stringstream temp_stringtream;
            temp_stringtream.str(temp_str);
            temp_stringtream >> element;
            if (temp_stringtream.fail())
                return false;
            
            data.push(element);
        }
        
        if (data.size() == 2) {
            int first = data.front();
            data.pop();
            int second = data.front();
            //send notification to inherits about successfull parsed message
            onMessageParsed(IntTuple(first, second));
            return true;
        } else {
            return false;
        }
    }
    
} //namespace parser