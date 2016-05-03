//
//  XYOrthogonalListAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/12/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "XYOrthogonalListAOI.hpp"

XYOrthogonalListAOI::XYOrthogonalListAOI() {}

XYOrthogonalListAOI::~XYOrthogonalListAOI() {
    list<GameObject::Node *>::iterator iter;
    for (iter = this -> listX . begin(); iter != this -> listX .end(); iter ++) {
        delete (*iter);
    }
    for (iter = this -> listY . begin(); iter != this -> listY .end(); iter ++) {
        delete (*iter);
    }
}

bool XYOrthogonalListAOI::addPublisher(GameObject *obj) {
    this -> addObjectInList(obj, LISTX);
    this -> addObjectInList(obj, LISTY);
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool XYOrthogonalListAOI::addSubscriber(GameObject *obj) {
    if (obj -> type != PUB_SUB) {
        this -> addObjectInList(obj, LISTX);
        this -> addObjectInList(obj, LISTY);
    }
    
    this -> subscribers[obj -> id] = obj;
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool XYOrthogonalListAOI::removePublisher(GameObject *obj) {
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);
    if (obj -> type != PUB_SUB) {
        this -> removeObjectInList(obj, LISTX);
        this -> removeObjectInList(obj, LISTY);
    }
    
    return true;
}

bool XYOrthogonalListAOI::removeSubscriber(GameObject *obj) {
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    this -> subscribers . erase(obj -> id);
    
    this -> removeObjectInList(obj, LISTX);
    this -> removeObjectInList(obj, LISTY);
    
    return true;
}

bool XYOrthogonalListAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
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
    oldSubMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, oldPosX, oldPosY);
    newSubMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    
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

bool XYOrthogonalListAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    map<entity_t, GameObject *> oldPubMaps, newPubMaps;
    
    oldPubMaps = this -> findPublishersInRange(obj -> id, oldPosX, oldPosY, obj -> range, obj -> listNodePosX, obj -> listNodePosY, obj -> listSubPosX, obj -> listSubPosY);
    
    // update subscriber's listPosX and listPosY
    if (oldPosX != obj -> posX) {
        this -> removeObjectInList(obj, LISTX);
        this -> addObjectInList(obj, LISTX);
    }
    if (oldPosY != obj -> posY) {
        this -> removeObjectInList(obj, LISTY);
        this -> addObjectInList(obj, LISTY);
    }
    
    newPubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range, obj -> listNodePosX, obj -> listNodePosY, obj -> listSubPosX, obj -> listSubPosY);
    
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

bool XYOrthogonalListAOI::addObjectInList(GameObject *obj, state_t listStyle) {
    list<GameObject::Node *>::iterator iter;
    position_t pos;
    
    if (LISTX == listStyle) {
        pos = obj -> posX;
        
        for (iter = this -> listX . begin(); iter != this -> listX . end(); iter ++) {
            if (pos <= (*iter) -> pos) {
                break;
            }
        }
        if (iter == this -> listX . end() || pos < (*iter) -> pos) {
            GameObject::Node *n = new GameObject::Node();
            n -> pos = pos;
            n -> objList.push_back(obj);
            
            this -> listX . insert(iter, n);
            n -> iterPos = -- iter;
            obj -> listNodePosX = n -> iterPos;
            obj -> listSubPosX = n -> objList.begin();
        } else if (pos == (*iter) -> pos) {
            (*iter) -> objList . push_front(obj);
            obj -> listNodePosX = (*iter) -> iterPos;
            obj -> listSubPosX = (*iter) -> objList . begin();
        }
        
    } else if(LISTY == listStyle){
        pos = obj -> posY;
        
        for (iter = this -> listY . begin(); iter != this -> listY . end(); iter ++) {
            if (pos <= (*iter) -> pos) {
                break;
            }
        }
        if (iter == this -> listY . end() || pos < (*iter) -> pos) {
            GameObject::Node *n = new GameObject::Node;
            n -> pos = pos;
            n -> objList.push_back(obj);
            
            this -> listY . insert(iter, n);
            n -> iterPos = -- iter;
            obj -> listNodePosY = n -> iterPos;
            obj -> listSubPosY = n -> objList.begin();
        } else if (pos == (*iter) -> pos) {
            (*iter) -> objList . push_front(obj);
            obj -> listNodePosY = (*iter) -> iterPos;
            obj -> listSubPosY = (*iter) -> objList . begin();
        }
        
    } else {
        return false;
    }
    
    return true;
}

bool XYOrthogonalListAOI::removeObjectInList(GameObject *obj, state_t listStyle) {
    if (listStyle == LISTX) {
        (*(obj -> listNodePosX)) -> objList . erase(obj -> listSubPosX);
        obj -> listSubPosX = (*(obj -> listNodePosX)) -> objList . end();
        obj -> listNodePosX = this -> listX . end();
    } else if (listStyle == LISTY) {
        (*(obj -> listNodePosY)) -> objList . erase(obj -> listSubPosY);
        obj -> listSubPosY = (*(obj -> listNodePosY)) -> objList . end();
        obj -> listNodePosY = this -> listY . end();
    } else {
        return false;
    }
    
    return true;
}

void XYOrthogonalListAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> BaseAOI::findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }

}

void XYOrthogonalListAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps =
    this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range, obj -> listNodePosX, obj -> listNodePosY, obj -> listSubPosX, obj -> listSubPosY);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
}

map<entity_t, GameObject *> XYOrthogonalListAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range, list<GameObject::Node *>::iterator listNodePosX, list<GameObject::Node *>::iterator listNodePosY, list<GameObject *>::iterator listSubPosX, list<GameObject *>::iterator listSubPosY) {
    map<entity_t, GameObject *> pubs;
    map<entity_t, GameObject *> pubsX, pubsY;
    list<GameObject::Node *>::iterator iter;
    list<GameObject *>::iterator subIter;
    map<entity_t, GameObject *>::iterator mapIter;
    
    // find pubs in listX, search from obj -> listPosX
    iter = listNodePosX;
    for (; iter != this -> listX . end(); iter ++) {
        if (isInRange((*iter) -> pos, posX, range)) {
            for (subIter = (*iter) -> objList . begin(); subIter != (*iter) -> objList . end(); subIter ++) {
                if (SUBSCRIBER == (*subIter) -> type || objId == (*subIter) -> id) {
                    continue;
                }
                pubsX[(*subIter) -> id] = (*subIter);
            }
            
        } else {
            break;
        }
    }
    iter = listNodePosX;
    if (iter != this -> listX .begin()) {
        iter --;
        for (; iter != this -> listX . begin(); iter --) {
            if (isInRange((*iter) -> pos, posX, range)) {
                for (subIter = (*iter) -> objList . begin(); subIter != (*iter) -> objList . end(); subIter ++) {
                    if (SUBSCRIBER == (*subIter) -> type || objId == (*subIter) -> id) {
                        continue;
                    }
                    pubsX[(*subIter) -> id] = (*subIter);
                }
            } else {
                break;
            }
        }
    }
    if (this -> listX . begin() == iter) {
        if (isInRange((*iter) -> pos, posX, range)) {
            for (subIter = (*iter) -> objList . begin(); subIter != (*iter) -> objList . end(); subIter ++) {
                if (SUBSCRIBER == (*subIter) -> type || objId == (*subIter) -> id) {
                    continue;
                }
                pubsX[(*subIter) -> id] = (*subIter);
            }
        }
    }
    
    // find pubs in listY, search from obj -> listPosY
    iter = listNodePosY;
    for (; iter != this -> listY . end(); iter ++) {
        if (isInRange((*iter) -> pos, posY, range)) {
            for (subIter = (*iter) -> objList . begin(); subIter != (*iter) -> objList . end(); subIter ++) {
                if (SUBSCRIBER == (*subIter) -> type || objId == (*subIter) -> id) {
                    continue;
                }
                pubsY[(*subIter) -> id] = (*subIter);
            }
            
        } else {
            break;
        }
    }
    iter = listNodePosY;
    if (iter != this -> listY .begin()) {
        iter --;
        for (; iter != this -> listY . begin(); iter --) {
            if (isInRange((*iter) -> pos, posY, range)) {
                for (subIter = (*iter) -> objList . begin(); subIter != (*iter) -> objList . end(); subIter ++) {
                    if (SUBSCRIBER == (*subIter) -> type || objId == (*subIter) -> id) {
                        continue;
                    }
                    pubsY[(*subIter) -> id] = (*subIter);
                }
            } else {
                break;
            }
        }
    }
    if (this -> listY . begin() == iter) {
        if (isInRange((*iter) -> pos, posY, range)) {
            for (subIter = (*iter) -> objList . begin(); subIter != (*iter) -> objList . end(); subIter ++) {
                if (SUBSCRIBER == (*subIter) -> type || objId == (*subIter) -> id) {
                    continue;
                }
                pubsY[(*subIter) -> id] = (*subIter);
            }
        }
    }

    // rect aoi, not the circle!
    for (mapIter = pubsX.begin(); mapIter != pubsX.end(); mapIter ++) {
        if (pubsY.find(mapIter -> first) != pubsY.end()) {
            pubsY.erase(mapIter -> first);
            pubs[mapIter -> first] = mapIter -> second;
        }
    }
    
    return pubs;
}




