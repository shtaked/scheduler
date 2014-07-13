//
//  main.cpp
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//
#include <memory>

#include "Scheduler.h"

int main(int argc, const char * argv[])
{
    using namespace scheduler;
    
    ::std::unique_ptr<Scheduler> scheduler(new Scheduler());
    
    scheduler->start();
    
    return 0;
}

