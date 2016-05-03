//
//  Tower.cpp
//  AOIs
//
//  Created by zklgame on 4/13/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "Tower.hpp"

bool Tower::addPublisher(GameObject *obj) {
    this -> publishers[obj -> id] = obj;
    return true;
}

bool Tower::removePublisher(GameObject *obj) {
    this -> publishers . erase(obj -> id);
    return true;
}

map<entity_t, GameObject *> Tower::getPublishers() {
    return this -> publishers;
}