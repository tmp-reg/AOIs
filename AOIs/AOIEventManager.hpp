//
//  AOIEventManager.hpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef AOIEventManager_hpp
#define AOIEventManager_hpp

#include "GameObject.hpp"
#include <list>

using namespace std;

class AOIEventManager {
public:
    AOIEventManager() {};
    virtual ~AOIEventManager() {};
    
    // only receive publishers' message
    // if A must know about B, then B is the publisher for A
    void onAddPublisher(GameObject *theObj, list<GameObject *> subscribers);
    void onAddSubscriber(GameObject *theObj, list<GameObject *> publishers);
    
    void onRemovePublisher(GameObject *theObj, list<GameObject *> subscribers);
    void onRemoveSubscriber(GameObject *theObj, list<GameObject *> publishers);
    
    void onMovePublisher(GameObject *theObj, list<GameObject *> subscribers);
    
    // no onMoveSubscriber
    
};

#endif /* AOIEventManager_hpp */
