//
//  main.cpp
//  InputSimulator
//
//  Created by Евгений Койро on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <time.h>

#include <getopt.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Common.h"

const int kDefaultJobsCount = 10;
const int kDefaultJobsInterval = 500;
const int kDefaultResourceCount = 10;
const int kDefaultResourceInterval = 500;
const int kDefaultAvailableResources = 10;

int jobs_count = kDefaultJobsCount;
int jobs_interval = kDefaultJobsInterval;
int resources_count = kDefaultResourceCount;
int resources_interval = kDefaultResourceInterval;
int available_resources = kDefaultAvailableResources;

class JobsGenerator : public common::Thread
{
public:
    virtual void run() override {
        int res = mkfifo(common::kJobsLineId.c_str(), 0777);
        if (res == -1 && errno != EEXIST) {
            ::std::ostringstream out_stream;
            out_stream << "Error opening fifo line (" << common::kJobsLineId << "): " << errno << ::std::endl;
            common::log(out_stream.str());
            
            return;
        }
        
        int fd = ::open(common::kJobsLineId.c_str(), O_WRONLY);
        if (fd == -1) {
            ::std::ostringstream out_stream;
            out_stream << "Error opening fifo line (" << common::kJobsLineId << "): " << errno << ::std::endl;
            common::log(out_stream.str());
            
            return;
        }
        
        while (1) {
            common::Jobs jobs;
            for (int i = 0; i < jobs_count; ++i) {
                common::Job job;
                job.units_count = ::std::rand() % 10 + 1;
                job.steps_count = ::std::rand() % 100 + 1;
                jobs.push(job);
            }
            
            ::std::string message = serialize(jobs);
            ::std::ostringstream out_stream;
            out_stream << "Jobs message generated: " << message << ::std::endl;
            common::log(out_stream.str());
            
            if (write(fd, message.c_str(), message.length()) == -1) {
                out_stream.str("");
                out_stream << "Failed write to fifo line (" << common::kJobsLineId << "): " << errno << ::std::endl;
                common::log(out_stream.str());
            }
            
            out_stream.str("");
            out_stream << "Wrote to fifo line: " << message << ::std::endl;
            common::log(out_stream.str());
            
            usleep(jobs_interval * 1000);
        }
    }
    
private:
    ::std::string serialize(common::Jobs &jobs) {
        ::std::ostringstream message_stream;
        
        while (!jobs.empty()) {
            common::Job job = jobs.front();
            
            message_stream << "(" << job.units_count << " " << job.steps_count << ") ";
            
            jobs.pop();
        }
        return message_stream.str();
    }
};

class ResourcesGenerator : public common::Thread
{
public:
    virtual void run() override {
        int res = mkfifo(common::kResourceLineId.c_str(), 0777);
        if (res == -1 && errno != EEXIST) {
            ::std::ostringstream out_stream;
            out_stream << "Error opening fifo line (" << common::kResourceLineId << "): " << errno << ::std::endl;
            common::log(out_stream.str());
            
            return;
        }
        
        int fd = ::open(common::kResourceLineId.c_str(), O_WRONLY);
        if (fd == -1) {
            ::std::ostringstream out_stream;
            out_stream << "Error opening fifo line (" << common::kResourceLineId << "): " << errno << ::std::endl;
            common::log(out_stream.str());
            
            return;
        }
        
        while (1) {
            common::Resources resources;
            for (int i = 0; i < resources_count; ++i) {
                common::Resource resource;
                resource.units_count = 10 + 1;
                resource.node_number = ::std::rand() % kDefaultAvailableResources;
                resources.push(resource);
            }
            
            ::std::string message = serialize(resources);
            ::std::ostringstream out_stream;
            out_stream << "Resources message generated: " << message << ::std::endl;
            common::log(out_stream.str());
            
            if (write(fd, message.c_str(), message.length()) == -1) {
                out_stream.str("");
                out_stream << "Failed write to fifo line (" << common::kResourceLineId << "): " << errno << ::std::endl;
                common::log(out_stream.str());
            }
            
            out_stream.str("");
            out_stream << "Wrote to fifo line: " << message << ::std::endl;
            common::log(out_stream.str());
            
            usleep(resources_interval * 1000);
        }
    }
    
private:
    ::std::string serialize(common::Resources &resources) {
        ::std::ostringstream message_stream;
        
        while (!resources.empty()) {
            common::Resource resource = resources.front();
            
            message_stream << "(" << resource.units_count << " " << resource.node_number << ") ";
            
            resources.pop();
        }
        return message_stream.str();
    }
};

int main(int argc, const char * argv[])
{
    ::std::srand((unsigned)time(NULL));
    
    enum {
        kHelp = 'h',
        kVersion = 'v',
        kJobsCount = 'j',
        kJobsInterval = 'i',
        kResourceCount = 'r',
        kResourceInterval = 'k',
        kAvailableResources = 'a'
    };
    
    typedef enum {MODE_NONE} Mode;
    struct option longopts[] = {
        {"help", no_argument, 0, kHelp},
        {"version", no_argument, 0, kVersion},
        {"jobs count", required_argument, 0, kJobsCount},
        {"jobs interval", required_argument, 0, kJobsInterval},
        {"resource count", required_argument, 0, kResourceCount},
        {"resource interval", required_argument, 0, kResourceInterval},
        {"available resources", required_argument, 0, kAvailableResources},
        {0, 0, 0, 0}
    };
    
    char ch;
    int longind;
    while ((ch = ::getopt_long(argc, (char * const *)argv, "?hv:j:i:r:k:a", longopts, &longind)) != -1) {
        switch (ch) {
            case kHelp:
            case '?': {
                printf("%s ((-h | -v | -i jobs_count | -i jobs_interval | -r resource_count "
                       "| -k resource_interval | -a available_resources:\n", argv[0]);
                printf("\t--%s, -%c\tspecify sended jobs count per time (by default is %d)\t\n",
                       longopts[2].name, longopts[2].val, kDefaultJobsCount);
                printf("\t--%s, -%c\tspecify sended jobs time interval in milliseconds (by default is %d)\t\n",
                       longopts[3].name, longopts[3].val, kDefaultJobsInterval);
                printf("\t--%s, -%c\tspecify sended resources per time (by default is %d)\t\n",
                       longopts[4].name, longopts[4].val, kDefaultResourceCount);
                printf("\t--%s, -%c\tspecify sended resources time interval in milliseconds (by default is %d)\t\n",
                       longopts[5].name, longopts[5].val, kDefaultResourceInterval);
                printf("\t--%s, -%c\tspecify available resources count (by default is %d)\t\n",
                       longopts[5].name, longopts[6].val, kDefaultAvailableResources);
                
                printf("\t--%s, -%c\tprint version and exit\t\n", longopts[1].name, longopts[1].val);
                printf("\t--%s, -%c\tprint this message and exit\n", longopts[0].name, longopts[0].val);
                return 0;
            }
            case kVersion: {
                printf("Input simulator v 0.1\n");
                break;
            }
            case kJobsCount: {
                if (optarg) {
                    jobs_count = atoi(optarg);
                }
                break;
            }
            case kJobsInterval: {
                if (optarg) {
                    jobs_interval = atoi(optarg);
                }
                break;
            }
            case kResourceCount: {
                if (optarg) {
                    resources_count = atoi(optarg);
                }
                break;
            }
            case kResourceInterval: {
                if (optarg) {
                    resources_interval = atoi(optarg);
                }
                break;
            }
            case kAvailableResources: {
                if (optarg) {
                    available_resources = atoi(optarg);
                }
                break;
            }
        }
    }
    
    JobsGenerator jobs_generator;
    jobs_generator.start();
    
    ResourcesGenerator resources_generator;
    resources_generator.start();
    
    jobs_generator.wait();
    resources_generator.wait();
    
    return 0;
}

