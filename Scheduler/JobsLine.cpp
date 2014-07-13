//
//  JobsLine.cpp
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#include "JobsLine.h"

namespace communication {
    
    void JobsLine::onMessageParsed(const parser::IntTuple &message)
    {
        //TODO: check if invalid
        
        static common::Job::JobId counter = 0;
        
        common::Job job = {++counter, 0, 0};
        ::std::tie(job.units_count, job.steps_count) = message;
        
        for (auto &listener : listeners_) {
            listener->onJobReceived(job);
        }
    }
    
} //namespace communication