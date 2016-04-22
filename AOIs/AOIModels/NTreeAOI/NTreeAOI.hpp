//
//  NTreeAOI.hpp
//  AOIs
//
//  Created by zklgame on 4/18/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef NTreeAOI_hpp
#define NTreeAOI_hpp

#include "../BaseAOI/BaseAOI.hpp"
#include "NTree.hpp"

class NTreeAOI: public BaseAOI {
public:
    NTreeAOI(position_t worldWidth, position_t worldHeight, entity_t towerMaxPublisherNum, position_t maxLevel);
    virtual ~NTreeAOI();

private:
    position_t worldWidth, worldHeight;
    entity_t towerMaxPublisherNum; // the max entity num in each tower

    NTree * tree;

    bool addPublisher(GameObject *obj);
    bool addSubscriber(GameObject *obj);    
    bool removePublisher(GameObject *obj);
    bool removeSubscriber(GameObject *obj);

    bool onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    bool onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    
    // ADD or REMOVE: find by obj
    // if state = 1, add; if state = 2, remove
    void findSubscribersInTheirRange(GameObject *obj, state_t state);
    void findPublishersInRange(GameObject *obj, state_t state);

    // MOVE: find by position
    map<entity_t, GameObject *> findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY);
    map<entity_t, GameObject *> findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range);
    
};

#endif /* NTreeAOI_hpp */
