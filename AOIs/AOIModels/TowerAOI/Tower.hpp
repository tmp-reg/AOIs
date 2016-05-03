//
//  Tower.hpp
//  AOIs
//
//  Created by zklgame on 4/13/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef Tower_hpp
#define Tower_hpp

#include "../../GameBasics/GameObject.hpp"
#include <map>

class Tower {
public:
    Tower() {}
    virtual ~Tower() {}
    
    bool addPublisher(GameObject *obj);
    bool removePublisher(GameObject *obj);
    
    map<entity_t, GameObject *> getPublishers();
    
private:
    map<entity_t, GameObject *> publishers;
};


#endif /* Tower_hpp */
