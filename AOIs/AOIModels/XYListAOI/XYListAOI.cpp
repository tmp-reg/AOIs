//
//  XYListAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/6/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "XYListAOI.hpp"

bool XYListAOI::addPublisher(GameObject *obj) {
    this -> addObjectInList(obj, LISTX);
    this -> addObjectInList(obj, LISTY);
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool XYListAOI::addSubscriber(GameObject *obj) {
    if (obj -> type != PUB_SUB) {
        this -> addObjectInList(obj, LISTX);
        this -> addObjectInList(obj, LISTY);
    }
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool XYListAOI::removePublisher(GameObject *obj) {
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);
    
    if (obj -> type != PUB_SUB) {
        this -> removeObjectInList(obj, LISTX);
        this -> removeObjectInList(obj, LISTY);
    }

    return true;
}

bool XYListAOI::removeSubscriber(GameObject *obj) {
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    this -> removeObjectInList(obj, LISTX);
    this -> removeObjectInList(obj, LISTY);
    
    return true;
}

bool XYListAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    // update publisher's listPosX and listPosY
    if (PUB_SUB != obj -> type) {
        if (oldPosX != obj -> posX) {
            this -> removeObjectInList(obj, LISTX);
            this -> addObjectInList(obj, LISTX);
        }
        if (oldPosY != obj -> posY) {
            this -> removeObjectInList(obj, LISTY);
            this -> addObjectInList(obj, LISTY);
        }
    }
    
    map<entity_t, GameObject *> oldSubMaps, newSubMaps;
    oldSubMaps = this -> findSubscribersInTheirRange(obj -> id, oldPosX, oldPosY);
    newSubMaps = this -> findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldSubMaps . begin(); iter != oldSubMaps . end(); iter ++) {
        if (newSubMaps.find(iter -> first) != newSubMaps.end()) {
            moveSet.push_back(iter -> second);
            newSubMaps.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newSubMaps.size() > 0) {
        for (iter = newSubMaps . begin(); iter != newSubMaps . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    this -> aoiEventManager -> onAddPublisher(obj, addSet);
    this -> aoiEventManager -> onRemovePublisher(obj, removeSet);
    this -> aoiEventManager -> onMovePublisher(obj, moveSet);
    
    return true;
}

bool XYListAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    map<entity_t, GameObject *> oldPubMaps, newPubMaps;
    
    oldPubMaps = this -> findPublishersInRange(obj -> id, oldPosX, oldPosY, obj -> range, obj -> listPosX, obj -> listPosY);
    
    // update subscriber's listPosX and listPosY
    if (oldPosX != obj -> posX) {
        this -> removeObjectInList(obj, LISTX);
        this -> addObjectInList(obj, LISTX);
    }
    if (oldPosY != obj -> posY) {
        this -> removeObjectInList(obj, LISTY);
        this -> addObjectInList(obj, LISTY);
    }
    
    newPubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range, obj -> listPosX, obj -> listPosY);
    
    list<GameObject *> addSet, moveSet, removeSet;
    map<entity_t, GameObject *> :: iterator iter;
    for (iter = oldPubMaps . begin(); iter != oldPubMaps . end(); iter ++) {
        if (newPubMaps.find(iter -> first) != newPubMaps.end()) {
            moveSet.push_back(iter -> second);
            newPubMaps.erase(iter -> first);
        } else {
            removeSet.push_back(iter -> second);
        }
    }
    if (newPubMaps.size() > 0) {
        for (iter = newPubMaps . begin(); iter != newPubMaps . end(); iter ++) {
            addSet.push_back(iter -> second);
        }
    }
    
    // do no change to moveSet
    this -> aoiEventManager -> onAddSubscriber(obj, addSet);
    this -> aoiEventManager -> onRemoveSubscriber(obj, removeSet);
    
    return true;
}

bool XYListAOI::addObjectInList(GameObject *obj, state_t listStyle) {
    list<GameObject *>::iterator iter;
    position_t pos;
    
    if (LISTX == listStyle) {
        pos = obj -> posX;
        
        for (iter = this -> listX . begin(); iter != this -> listX . end(); iter ++) {
            if (pos <= (*iter) -> posX) {
                break;
            }
        }
        this -> listX . insert(iter, obj);
        obj -> listPosX = --iter;
        
    } else if(LISTY == listStyle){
        pos = obj -> posY;
        
        for (iter = this -> listY . begin(); iter != this -> listY . end(); iter ++) {
            if (pos <= (*iter) -> posY) {
                break;
            }
        }
        this -> listY . insert(iter, obj);
        obj -> listPosY = --iter;

    } else {
        return false;
    }
    
    return true;
}

bool XYListAOI::removeObjectInList(GameObject *obj, state_t listStyle) {
    if (LISTX == listStyle) {
        this -> listX .erase(obj -> listPosX);
        obj -> listPosX = this -> listX . end();
    } else if(LISTY == listStyle){
        this -> listY . erase(obj -> listPosY);
        obj -> listPosY = this -> listY . end();
    } else {
        return false;
    }
    
    return true;
}

void XYListAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
   
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
    
}

void XYListAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range, obj -> listPosX, obj -> listPosY);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
    
}

map<entity_t, GameObject *> XYListAOI::findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY) {
    map<entity_t, GameObject *> subs;
    list<GameObject *>::iterator iter;
    
    // to find subscribers who can subscribe this publisher, must search any list from begin to end
    for (iter = this -> listX . begin(); iter != this -> listX . end(); iter ++) {
        if ((*iter) -> type == PUBLISHER || (*iter) -> id == objId) {
            continue;
        }
        if (isInRange2(posX, posY, (*iter) -> posX, (*iter) -> posY, (*iter) -> range)) {
            subs[(*iter) -> id] = (*iter);
        }
    }
    
    return subs;
}

map<entity_t, GameObject *> XYListAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range, list<GameObject *>::iterator listPosX, list<GameObject *>::iterator listPosY) {
    map<entity_t, GameObject *> pubs;
    map<entity_t, GameObject *> pubsX, pubsY;
    list<GameObject *>::iterator iter;
    map<entity_t, GameObject *>::iterator mapIter;
    
    // find pubs in listX, search from obj -> listPosX
    iter = listPosX;
    iter ++;
    for (; iter != this -> listX . end(); iter ++) {
        if (SUBSCRIBER == (*iter) -> type || objId == (*iter) -> id) {
            continue;
        }
        if (isInRange(posX, (*iter) -> posX, range)) {
            pubsX[(*iter) -> id] = (*iter);
        } else {
            break;
        }
    }
    iter = listPosX;
    if (iter != this -> listX . begin()) {
        iter --;
        for (; iter != this -> listX . begin(); iter --) {
            if (SUBSCRIBER == (*iter) -> type || objId == (*iter) -> id) {
                continue;
            }
            if (isInRange(posX, (*iter) -> posX, range)) {
                pubsX[(*iter) -> id] = (*iter);
            } else {
                break;
            }
        }
    }
    
    if (this -> listX . begin() == iter) {
        if (SUBSCRIBER != (*iter) -> type && (*iter) -> id != objId) {
            if (isInRange(posX, (*iter) -> posX, range)) {
                pubsX[(*iter) -> id] = (*iter);
            }
        }
    }
    
    // find pubs in listY, search from obj -> listPosY
    iter = listPosY;
    iter ++;
    for (; iter != this -> listY . end(); iter ++) {
        if (SUBSCRIBER == (*iter) -> type || objId == (*iter) -> id) {
            continue;
        }
        if (isInRange(posY, (*iter) -> posY, range)) {
            pubsY[(*iter) -> id] = (*iter);
        } else {
            break;
        }
    }
    iter = listPosY;
    if (iter != this -> listY . begin()) {
        iter --;
        for (; iter != this -> listY . begin(); iter --) {
            if (SUBSCRIBER == (*iter) -> type || objId == (*iter) -> id) {
                continue;
            }
            if (isInRange(posY, (*iter) -> posY, range)) {
                pubsY[(*iter) -> id] = (*iter);
            } else {
                break;
            }
        }
    }
    
    if (this -> listY . begin() == iter) {
        if (SUBSCRIBER != (*iter) -> type && (*iter) -> id != objId) {
            if (isInRange(posY, (*iter) -> posY, range)) {
                pubsY[(*iter) -> id] = (*iter);
            }
        }
    }
    
    // rect aoi, not the circle!
    for (mapIter = pubsX.begin(); mapIter != pubsX.end(); mapIter ++) {
        if (pubsY.find(mapIter -> first) != pubsY.end()) {
            pubs[mapIter -> first] = mapIter -> second;
        }
    }
    
//    // exactly the same as the EuDistanceAOI
//    for (mapIter = pubsX.begin(); mapIter != pubsX.end(); mapIter ++) {
//        if (isInRange2(posX, posY, mapIter -> second -> posX, mapIter -> second -> posY, range)) {
//            pubs[mapIter->first] = mapIter -> second;
//        }
//    }
    
    return pubs;
}


