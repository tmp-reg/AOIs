//
//  TowerAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/13/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "TowerAOI.hpp"

#include <cmath>

TowerAOI::TowerAOI(position_t worldWidth, position_t worldHeight, position_t towerWidth, position_t towerHeight): worldWidth(worldWidth), worldHeight(worldHeight), towerWidth(towerWidth), towerHeight(towerHeight) {
    // tower range: [ ... )
    this -> towerX = worldWidth / towerWidth + 1;
    this -> towerY = worldHeight / towerHeight + 1;
    
    for (position_t i = 0; i < this -> towerX; i ++) {
        for (position_t j = 0; j < this -> towerY; j ++) {
            this -> towers[i][j] = new Tower();
        }
    }
}

TowerAOI::~TowerAOI() {
    for (position_t i = 0; i < this -> towerX; i ++) {
        for (position_t j = 0; j < this -> towerY; j ++) {
            delete this -> towers[i][j];
        }
    }
}

bool TowerAOI::addPublisher(GameObject *obj) {
    position_t towerPosX = obj -> posX / this -> towerWidth;
    position_t towerPosY = obj -> posY / this -> towerHeight;
    this -> towers[towerPosX][towerPosY] -> addPublisher(obj);
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerAOI::addSubscriber(GameObject *obj) {
    position_t towerPosX = obj -> posX / this -> towerWidth;
    position_t towerPosY = obj -> posY / this -> towerHeight;
    this -> towers[towerPosX][towerPosY] -> addSubscriber(obj);
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerAOI::removePublisher(GameObject *obj) {
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);
    
    position_t towerPosX = obj -> posX / this -> towerWidth;
    position_t towerPosY = obj -> posY / this -> towerHeight;
    this -> towers[towerPosX][towerPosY] -> removePublisher(obj);
    
    return true;
}

bool TowerAOI::removeSubscriber(GameObject *obj) {
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    position_t towerPosX = obj -> posX / this -> towerWidth;
    position_t towerPosY = obj -> posY / this -> towerHeight;
    this -> towers[towerPosX][towerPosY] -> removeSubscriber(obj);
    
    return true;
}

bool TowerAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    if (oldPosX != obj -> posX || oldPosY != obj -> posY) {
        this -> towers[oldPosX / towerWidth][oldPosY / towerHeight] -> removePublisher(obj);
        this -> towers[obj -> posX / towerWidth][obj -> posY / towerHeight] -> addPublisher(obj);
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

bool TowerAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    map<entity_t, GameObject *> oldPubMaps, newPubMaps;
    
    oldPubMaps = this -> findPublishersInRange(obj -> id, oldPosX, oldPosY, obj -> range);
    
    if (oldPosX != obj -> posX || oldPosY != obj -> posY) {
        this -> towers[oldPosX / towerWidth][oldPosY / towerHeight] -> removeSubscriber(obj);
        this -> towers[obj -> posX / towerWidth][obj -> posY / towerHeight] -> addSubscriber(obj);
    }
    
    newPubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
        
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

void TowerAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
}

void TowerAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
}

map<entity_t, GameObject *> TowerAOI::findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY) {
    map<entity_t, GameObject *> subs;
    map<entity_t, GameObject *> towerSubs;
    map<entity_t, GameObject *>::iterator iter;
    
    for (position_t i = 0; i < this -> towerX; i ++) {
        for (position_t j = 0; j < this -> towerY; j ++) {
            towerSubs = this -> towers[i][j] -> getSubscribers();
            for (iter = towerSubs.begin(); iter != towerSubs.end(); iter ++) {
                if (isInRange2(posX, posY, iter -> second -> posX, iter -> second -> posY, iter -> second -> range)) {
                    subs[iter -> first] = iter -> second;
                }
            }
        }
    }
    
    subs.erase(objId);
    
    return subs;
}

map<entity_t, GameObject *> TowerAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) {
    map<entity_t, GameObject *> pubs, partialPubs;
    list<Tower*> fullCoveredTowers, partialCoveredTowers;
    list<Tower*>::iterator iter;
    map<entity_t, GameObject *>::iterator pubMapIter;
    
    position_t towerPosX = posX / this -> towerWidth;
    position_t towerPosY = posY / this -> towerHeight;
    
    position_t i, j;
    for (i = towerPosX; i < this -> towerX; i ++) {
        for (j = towerPosY; j < this -> towerY; j ++) {
            if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                break;
            }
        }
        if (towerPosY != 0) {
            for (j = towerPosY - 1; j != 0; j --) {
                if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                    break;
                }
            }
            if (0 == j) {
                classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers);
            }
        }
    }
    if (towerPosX != 0) {
        for (i = towerPosX - 1; i != 0; i --) {
            for (j = towerPosY; j < this -> towerY; j ++) {
                if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                    break;
                }
            }
            if (towerPosY != 0) {
                for (j = towerPosY - 1; j != 0; j --) {
                    if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                        break;
                    }
                }
                if (0 == j) {
                    classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers);
                }
            }
        }
        if (0 == i) {
            for (j = towerPosY; j < this -> towerY; j ++) {
                if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                    break;
                }
            }
            if (towerPosY != 0) {
                for (j = towerPosY - 1; j != 0; j --) {
                    if (!classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers)) {
                        break;
                    }
                }
                if (0 == j) {
                    classifyTower(posX, posY, range, i, j, fullCoveredTowers, partialCoveredTowers);
                }
            }
        }
    }
    
    for (iter = fullCoveredTowers . begin(); iter != fullCoveredTowers . end(); iter ++) {
        pubs = addTwoMaps(pubs, (*iter) -> getPublishers());
    }

    // traversal all the partialCoveredTowers to find all the object in range
    for (iter = partialCoveredTowers . begin(); iter != partialCoveredTowers . end(); iter ++) {
        partialPubs = (*iter) -> getPublishers();
        for (pubMapIter = partialPubs.begin(); pubMapIter != partialPubs.end(); pubMapIter ++) {
            if (isInRange2(posX, posY, pubMapIter -> second -> posX, pubMapIter -> second -> posY, range)) {
                pubs[pubMapIter -> first] = pubMapIter -> second;
            }
        }
    }
    
    pubs.erase(objId);
    
    return pubs;
}

bool TowerAOI::isFullCoveredTower(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
    if ((isInRange2(posX, posY, pos1X, pos1Y, range)) &&
        (isInRange2(posX, posY, pos2X, pos2Y, range)) &&
        (isInRange2(posX, posY, pos1X, pos2Y, range)) &&
        (isInRange2(posX, posY, pos2X, pos1Y, range))) {
        return true;
    }
    
    return false;
}

bool TowerAOI::isPartialCoveredTower(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
    // angel in circle
    if ((isInRange2(posX, posY, pos1X, pos1Y, range)) ||
        (isInRange2(posX, posY, pos2X, pos2Y, range)) ||
        (isInRange2(posX, posY, pos1X, pos2Y, range)) ||
        (isInRange2(posX, posY, pos2X, pos1Y, range))) {
        return true;
    }
    
    // side in circle
    if ((isCoveredBySide(posX, posY, range, pos1X, pos1Y, pos2X, pos2Y)) ||
        (isCoveredBySide(posX, posY, range, pos2X, pos1Y, pos1X, pos2Y)) ||
        (isCoveredBySide(posY, posX, range, pos1Y, pos1X, pos2Y, pos2X)) ||
        (isCoveredBySide(posY, posX, range, pos2Y, pos1X, pos1Y, pos2X))) {
        return true;
    }
    
    
    if ((isInRange(posX, pos1X, range)) ||
        (isInRange(posX, pos2X, range)) ||
        (isInRange(posY, pos1Y, range)) ||
        (isInRange(posY, pos2Y, range))) {
        return true;
    }
    
    return false;
}

bool TowerAOI::isCoveredBySide(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
    if (isInRange(posX, pos1X, range)) {
        position_t dis2 = range * range - (posX - pos1X) * (posX - pos1X);
        position_t testposY = sqrt(dis2) + posY;
        return (testposY * testposY) >= pos1Y * pos1Y && (testposY * testposY) <= pos2Y * pos2Y;
    }
    
    return false;
}

bool TowerAOI::classifyTower(position_t posX, position_t posY, position_t range, int i, int j, list<Tower*> &fullCoveredTowers, list<Tower*> &partialCoveredTowers) {
    if (isFullCoveredTower(posX, posY, range, i * towerWidth, j * towerHeight, (i + 1) * towerWidth, (j + 1) * towerHeight)) {
        fullCoveredTowers.push_back(towers[i][j]);
    } else if (isPartialCoveredTower(posX, posY, range, i * towerWidth, j * towerHeight, (i + 1) * towerWidth, (j + 1) * towerHeight)) {
        partialCoveredTowers.push_back(towers[i][j]);
    } else if (i == posX / towerWidth && j == posY / towerHeight) {
        partialCoveredTowers.push_back(towers[i][j]);
    } else {
        return false;
    }
    
    return true;
}

