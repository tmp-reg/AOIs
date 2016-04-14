//
//  EuDistanceAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "EuDistanceAOI.hpp"

bool EuDistanceAOI::addPublisher(GameObject *obj) {
    this -> publishers[obj -> id] = obj;
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool EuDistanceAOI::addSubscriber(GameObject *obj) {
    this -> subscribers[obj -> id] = obj;
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool EuDistanceAOI::removePublisher(GameObject *obj) {
    if (this -> publishers . find(obj -> id) == this -> publishers . end()) {
        return false;
    }
    
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);
    
    this -> publishers . erase(obj -> id);
    
    return true;
}

bool EuDistanceAOI::removeSubscriber(GameObject *obj) {
    if (this -> subscribers . find(obj -> id) == this -> subscribers . end()) {
        return false;
    }
    
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    this -> subscribers . erase(obj -> id);
    
    return true;
}

bool EuDistanceAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    // to find all the subscribers who can subscribe the publisher in old position
    map<entity_t, GameObject *> oldSubscribers = findSubscribersInTheirRange(obj -> id, oldPosX, oldPosY);
    // to find all the subscribers who can subscribe the publisher in new position
    map<entity_t, GameObject *> newSubscribers = findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldSubscribers . begin(); iter != oldSubscribers . end(); iter ++) {
        if (newSubscribers.find(iter -> first) != newSubscribers.end()) {
            moveSet.push_back(iter -> second);
            newSubscribers.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newSubscribers.size() > 0) {
        for (iter = newSubscribers . begin(); iter != newSubscribers . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    this -> aoiEventManager -> onAddPublisher(obj, addSet);
    this -> aoiEventManager -> onRemovePublisher(obj, removeSet);
    this -> aoiEventManager -> onMovePublisher(obj, moveSet);
    
    return true;
}

bool EuDistanceAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    // to find all the publishers the obj can subscribe in old position
    map<entity_t, GameObject *> oldPublishers = findPublishersInRange(obj -> id, oldPosX, oldPosY, obj -> range);
    // to find all the publishers the obj can subscribe in new position
    map<entity_t, GameObject *> newPublishers = findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldPublishers . begin(); iter != oldPublishers . end(); iter ++) {
        if (newPublishers.find(iter -> first) != newPublishers.end()) {
            moveSet.push_back(iter -> second);
            newPublishers.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newPublishers.size() > 0) {
        for (iter = newPublishers . begin(); iter != newPublishers . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    // do no change to moveSet 
    this -> aoiEventManager -> onAddSubscriber(obj, addSet);
    this -> aoiEventManager -> onRemoveSubscriber(obj, removeSet);
    
    return true;
}

void EuDistanceAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
   
    // send messages to above subscribers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
    
}

void EuDistanceAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    // receive messages from above publishers
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
    
}

map<entity_t, GameObject *> EuDistanceAOI::findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY) {
    map<entity_t, GameObject *> subs;
    map<entity_t, GameObject*>::iterator iter;
    for (iter = this -> subscribers . begin(); iter != this -> subscribers . end(); iter ++) {
        if (isInRange2(posX, posY, iter -> second -> posX, iter -> second -> posY, iter -> second -> range)) {
            subs[iter -> second -> id] = iter -> second;
        }
    }
    
    subs.erase(objId);

    return subs;
}

map<entity_t, GameObject *> EuDistanceAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) {
    map<entity_t, GameObject *> pubs;
    map<entity_t, GameObject *>::iterator iter;
    for (iter = this -> publishers . begin(); iter != this -> publishers . end(); iter ++) {
        if (isInRange2(posX, posY, iter -> second -> posX, iter -> second -> posY, range)) {
            pubs[iter -> second -> id] = iter -> second;
        }
    }
    
    pubs.erase(objId);
    
    return pubs;
}


