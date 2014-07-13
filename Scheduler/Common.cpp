//
//  Common.cpp
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#include <string>
#include <iostream>
#include <pthread.h>

namespace common {
    
extern const ::std::string kJobsLineId = "jobs_line";
extern const ::std::string kResourceLineId = "resource_line";
    
void log(const ::std::string &message)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    
    ::std::cout << message;
    
    pthread_mutex_unlock(&mutex);
}

}   //namespace common