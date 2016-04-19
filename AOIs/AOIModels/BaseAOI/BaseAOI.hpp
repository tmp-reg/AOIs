//
//  BaseAOI.hpp
//  AOIs
//
//  Created by zklgame on 4/7/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef BaseAOI_hpp
#define BaseAOI_hpp

#include "../../GameBasics/GameObject.hpp"
#include "../../AOIEventManager.hpp"

#include <map>

class BaseAOI {
public:
    inline BaseAOI() { this -> aoiEventManager = new AOIEventManager(); };
    inline virtual ~BaseAOI() { delete this -> aoiEventManager; };
    
    virtual bool addObject(GameObject *obj);
    virtual bool removeObject(GameObject *obj);
    virtual bool onObjectMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    
protected:
    AOIEventManager *aoiEventManager;
    
    virtual bool addPublisher(GameObject *obj) = 0;
    virtual bool addSubscriber(GameObject *obj) = 0;
    virtual bool removePublisher(GameObject *obj) = 0;
    virtual bool removeSubscriber(GameObject *obj) = 0;
    
    virtual bool onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) = 0;
    virtual bool onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) = 0;
    
    // ADD or REMOVE: find by obj
    // if state = 1, add; if state = 2, remove
    virtual void findSubscribersInTheirRange(GameObject *obj, state_t state) = 0;
    virtual void findPublishersInRange(GameObject *obj, state_t state) = 0;
    
    // MOVE: find by position
    virtual map<entity_t, GameObject *> findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY) = 0;
    virtual map<entity_t, GameObject *> findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) = 0;
    
    inline virtual bool isInRange2(position_t posX, position_t posY, position_t otherPosX, position_t otherPosY, position_t range) {
        position_t distance2 = (posX - otherPosX) * (posX - otherPosX) + (posY - otherPosY) * (posY - otherPosY);
        return distance2 <= range * range;
    }
    
    inline virtual bool isInRange2WithDouble(double posX, double posY, double otherPosX, double otherPosY, double range) {
        double distance2 = (posX - otherPosX) * (posX - otherPosX) + (posY - otherPosY) * (posY - otherPosY);
        return distance2 <= range * range;
    }
    
    inline virtual bool isInRange(position_t pos, position_t otherPos, position_t range) {
        position_t del = pos >= otherPos ? pos - otherPos : otherPos - pos;
        return del <= range;
    }
    
    inline virtual bool isInRangeWithDouble(double pos, double otherPos, double range) {
        double del = pos >= otherPos ? pos - otherPos : otherPos - pos;
        return del <= range;
    }
    
    inline virtual list<GameObject *> transMapsToLists(map<entity_t, GameObject *> maps) {
        list<GameObject *> objects;
        map<entity_t, GameObject *>::iterator iter;
        for (iter = maps.begin(); iter != maps.end(); iter ++) {
            objects.push_back(iter -> second);
        }
        return objects;
    }
    
    inline virtual map<entity_t, GameObject *> addTwoMaps(map<entity_t, GameObject *> map1, map<entity_t, GameObject *> map2) {
        map<entity_t, GameObject *>::iterator iter;
        for (iter = map2.begin(); iter != map2.end(); iter ++) {
            map1[iter -> first] = iter -> second;
        }
        return map1;
    }

};

#endif /* BaseAOI_hpp */
