//
//  ResourcesLine.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#ifndef RESOURCE_LINE
#define RESOURCE_LINE

#include <set>

#include "Common.h"

#include "FifoLine.h"
#include "IntTupleMessageParser.h"

namespace communication {
    
    //Listener base class that receives resource messages
    class ResourcesLineListener
    {
    public:
        virtual void onResourceReceived(const common::Resource &resource) = 0;
    };
    
    //communication line for receiving resources
    //uses FifoLine with IntTupleParser implementations
    class ResourcesLine : public FifoLine<parser::IntTupleMessageParser>
    {
    public:
        void addListener(ResourcesLineListener *listener) { listeners_.insert(listener); }
        void removeListener(ResourcesLineListener *listener) { listeners_.erase(listener); }
     
    private:
        void onMessageParsed(const parser::IntTuple &message);
        
    private:
        ::std::set<ResourcesLineListener*> listeners_;
    };
    
}   //namespace communication

#endif // RESOURCE_LINE
