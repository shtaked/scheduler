//
//  ResourcesLine.cpp
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#include "ResourcesLine.h"

namespace communication {
    
    void ResourcesLine::onMessageParsed(const parser::IntTuple &message)
    {
        //TODO: check if valid
        
        common::Resource resource;
        ::std::tie(resource.units_count, resource.node_number) = message;
        
        for (auto &listener : listeners_) {
            listener->onResourceReceived(resource);
        }
    }
    
} //namespace communication