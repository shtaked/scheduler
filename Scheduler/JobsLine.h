//
//  JobsLine.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#ifndef JOBSLINE_H
#define JOBSLINE_H

#include <set>

#include "Common.h"

#include "FifoLine.h"
#include "IntTupleMessageParser.h"

namespace communication {
    
    //Listener base class that receives job messages
    class JobsLineListener
    {
    public:
        virtual void onJobReceived(const common::Job &job) = 0;
    };
    
    //communication line for receiving jobs
    //uses FifoLine with IntTupleParser implementations
    class JobsLine : public FifoLine<parser::IntTupleMessageParser>
    {
    public:
        void addListener(JobsLineListener *listener) { listeners_.insert(listener); }
        void removeListener(JobsLineListener *listener) { listeners_.erase(listener); }
        
    private:
        void onMessageParsed(const parser::IntTuple &message);
        
    private:
        ::std::set<JobsLineListener*> listeners_;
    };
    
}   //namespace communication

#endif  //JOBSLINE_H
