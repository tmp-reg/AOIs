//
//  TowerHexagonAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/15/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "TowerHexagonAOI.hpp"
#include <cmath>

TowerHexagonAOI::TowerHexagonAOI(position_t worldWidth, position_t worldHeight, position_t towerLength): worldWidth(worldWidth), worldHeight(worldHeight), towerLength(towerLength) {
    // tower range: [ ... )
    if (worldWidth < sqrt(3) / 2 * towerLength) {
        this -> towerX = 1;
    } else {
        this -> towerX = 2 + (worldWidth - sqrt(3) / 2 * towerLength) / (sqrt(3) * towerLength);
    }
    if (worldHeight < towerLength) {
        this -> towerY = 1;
    } else {
        this -> towerY = (worldHeight - towerLength) / (1.5 * towerLength) + 2;
    }
    
    for (position_t i = 0; i < this -> towerX; i ++) {
        for (position_t j = 0; j < this -> towerY; j ++) {
            this -> towers[i][j] = new Tower();
        }
    }
}

TowerHexagonAOI::~TowerHexagonAOI() {
    for (position_t i = 0; i < this -> towerX; i ++) {
        for (position_t j = 0; j < this -> towerY; j ++) {
            delete this -> towers[i][j];
        }
    }
}

bool TowerHexagonAOI::addPublisher(GameObject *obj) {
    position_t towerPosX, towerPosY;
    getTowerPosByXY(obj -> posX, obj -> posY, towerPosX, towerPosY);
    this -> towers[towerPosX][towerPosY] -> addPublisher(obj);
    
    this -> findSubscribersInTheirRange(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerHexagonAOI::addSubscriber(GameObject *obj) {
    position_t towerPosX, towerPosY;
    getTowerPosByXY(obj -> posX, obj -> posY, towerPosX, towerPosY);
    this -> towers[towerPosX][towerPosY] -> addSubscriber(obj);
    
    this -> findPublishersInRange(obj, ADD_MESSAGE);
    
    return true;
}

bool TowerHexagonAOI::removePublisher(GameObject *obj) {
    this -> findSubscribersInTheirRange(obj, REMOVE_MESSAGE);

    position_t towerPosX, towerPosY;
    getTowerPosByXY(obj -> posX, obj -> posY, towerPosX, towerPosY);
    this -> towers[towerPosX][towerPosY] -> removePublisher(obj);
    
    return true;
}

bool TowerHexagonAOI::removeSubscriber(GameObject *obj) {
    this -> findPublishersInRange(obj, REMOVE_MESSAGE);
    
    position_t towerPosX, towerPosY;
    getTowerPosByXY(obj -> posX, obj -> posY, towerPosX, towerPosY);
    this -> towers[towerPosX][towerPosY] -> removeSubscriber(obj);
    
    return true;
}

bool TowerHexagonAOI::onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    position_t oldTowerPosX, oldTowerPosY, towerPosX, towerPosY;
    if (oldPosX != obj -> posX || oldPosY != obj -> posY) {
        getTowerPosByXY(oldPosX, oldPosY, oldTowerPosX, oldTowerPosY);
        getTowerPosByXY(obj -> posX, obj -> posY, towerPosX, towerPosY);
        
        this -> towers[oldTowerPosX][oldTowerPosY] -> removePublisher(obj);
        this -> towers[towerPosX][towerPosY] -> addPublisher(obj);
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

bool TowerHexagonAOI::onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    map<entity_t, GameObject *> oldPubMaps, newPubMaps;
    
    oldPubMaps = this -> findPublishersInRange(obj -> id, oldPosX, oldPosY, obj -> range);
    
    position_t oldTowerPosX, oldTowerPosY, towerPosX, towerPosY;
    if (oldPosX != obj -> posX || oldPosY != obj -> posY) {
        getTowerPosByXY(oldPosX, oldPosY, oldTowerPosX, oldTowerPosY);
        getTowerPosByXY(obj -> posX, obj -> posY, towerPosX, towerPosY);
        
        this -> towers[oldTowerPosX][oldTowerPosY] -> removeSubscriber(obj);
        this -> towers[towerPosX][towerPosY] -> addSubscriber(obj);
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

void TowerHexagonAOI::findSubscribersInTheirRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> subMaps = this -> findSubscribersInTheirRange(obj -> id, obj -> posX, obj -> posY);
    list<GameObject *> subs = transMapsToLists(subMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddPublisher(obj, subs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemovePublisher(obj, subs);
    }
}

void TowerHexagonAOI::findPublishersInRange(GameObject *obj, state_t state) {
    map<entity_t, GameObject *> pubMaps = this -> findPublishersInRange(obj -> id, obj -> posX, obj -> posY, obj -> range);
    list<GameObject *> pubs = transMapsToLists(pubMaps);
    
    if (ADD_MESSAGE == state) {
        this -> aoiEventManager -> onAddSubscriber(obj, pubs);
    } else if (REMOVE_MESSAGE == state) {
        this -> aoiEventManager -> onRemoveSubscriber(obj, pubs);
    }
}

map<entity_t, GameObject *> TowerHexagonAOI::findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY) {
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

map<entity_t, GameObject *> TowerHexagonAOI::findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) {
    map<entity_t, GameObject *> pubs, partialPubs;
    list<Tower*> fullCoveredTowers, partialCoveredTowers;
    list<Tower*>::iterator iter;
    map<entity_t, GameObject *>::iterator pubMapIter;
    
    position_t towerPosX, towerPosY;
    getTowerPosByXY(posX, posY, towerPosX, towerPosY);
    
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

bool TowerHexagonAOI::isFullCoveredTower(position_t posX, position_t posY, position_t range, position_t centerX, position_t centerY) {
    if ((isInRange2(posX, posY, centerX, centerY - towerLength, range)) &&
        (isInRange2(posX, posY, centerX + sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength, range)) &&
        (isInRange2(posX, posY, centerX + sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength, range)) &&
        (isInRange2(posX, posY, centerX, centerY + towerLength, range)) &&
        (isInRange2(posX, posY, centerX - sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength, range)) &&
        (isInRange2(posX, posY, centerX - sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength, range))) {
        return true;
    }
    
    return false;
}

bool TowerHexagonAOI::isPartialCoveredTower(position_t posX, position_t posY, position_t range, position_t centerX, position_t centerY) {
    // angel in circle
    if ((isInRange2(posX, posY, centerX, centerY - towerLength, range)) ||
        (isInRange2(posX, posY, centerX + sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength, range)) ||
        (isInRange2(posX, posY, centerX + sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength, range)) ||
        (isInRange2(posX, posY, centerX, centerY + towerLength, range)) ||
        (isInRange2(posX, posY, centerX - sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength, range)) ||
        (isInRange2(posX, posY, centerX - sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength, range))) {
        return true;
    }
    
    // side in circle
    if ((isCoveredBySide(posX, posY, range, centerX, centerY - towerLength, centerX + sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength)) ||
        (isCoveredBySide(posX, posY, range, centerX + sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength, centerX + sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength)) ||
        (isCoveredBySide(posX, posY, range, centerX + sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength, centerX, centerY + towerLength)) ||
        (isCoveredBySide(posX, posY, range, centerX, centerY + towerLength, centerX - sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength)) ||
        (isCoveredBySide(posX, posY, range, centerX - sqrt(3) / 2 * towerLength, centerY + 0.5 * towerLength, centerX - sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength)) ||
        (isCoveredBySide(posX, posY, range, centerX - sqrt(3) / 2 * towerLength, centerY - 0.5 * towerLength, centerX, centerY - towerLength))) {
        return true;
    }
    
    return false;
}

bool TowerHexagonAOI::isCoveredBySide(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
        
    if (pos1X != pos2X) {
        double k = (pos1Y - pos2Y) * 1.0 / (pos1X - pos2X);
        double x = (k * posY + posX - k * pos1Y + k * k * pos1X) * 1.0 / (k * k + 1);
        double y = (pos1Y - k * pos1X + k * posX + k * k * posY) / (k * k + 1);
        if (isInRange2(posX, posY, x, y, range)) {
            return true;
        }
    } else {
        if (isInRange(posX, pos1X, range)) {
            return true;
        }
    }
    
    return false;
}

bool TowerHexagonAOI::classifyTower(position_t posX, position_t posY, position_t range, int i, int j, list<Tower*> &fullCoveredTowers, list<Tower*> &partialCoveredTowers) {
    position_t centerX, centerY;
    getCenterByTowerPos(i, j, centerX, centerY);
    
    if (isFullCoveredTower(posX, posY, range, centerX, centerY)) {
        fullCoveredTowers.push_back(towers[i][j]);
    } else if (isPartialCoveredTower(posX, posY, range, centerX, centerY)) {
        partialCoveredTowers.push_back(towers[i][j]);
    }else {
        position_t tmpI, tmpJ;
        getTowerPosByXY(posX, posY, tmpI, tmpJ);
        if (i == tmpI && j == tmpJ) {
            partialCoveredTowers.push_back(towers[i][j]);
            return false;
        }
        
        return false;
    }
    
    return true;
}

void TowerHexagonAOI::getTowerPosByXY(position_t posX, position_t posY, position_t &i, position_t &j) {
    i = posX / (sqrt(3) * towerLength);
    j = posY / (3 * towerLength) * 2;
    
    position_t leftDis = posY % (3 * towerLength);
    if (leftDis < towerLength) {
        return;
    } else if (leftDis >= 1.5 * towerLength && leftDis < 2.5 * towerLength) {
        j ++;
        if (posX < sqrt(3) / 2 * towerLength) {
            i = 0;
        } else {
            i = 1 + (posX - sqrt(3) / 2 * towerLength) / (sqrt(3) * towerLength);
        }
        return;
    } else {
        j ++;
        if (posX < sqrt(3) / 2 * towerLength) {
            i = 0;
        } else {
            i = 1 + (posX - sqrt(3) / 2 * towerLength) / (sqrt(3) * towerLength);
        }
        
        // find center Pos of Tile(i, j)
        // i - 1, j - 1
        // i, j - 1
        // i - 1, j + 1
        // i, j + 1
        position_t centerX, centerY;
        centerX = sqrt(3) * towerLength * i;
        centerY = 0.5 * towerLength + 1.5 * towerLength * j;
        
        // find whether in Tile around (i, j)
        position_t minI = i, minJ = j;
        position_t minRange = getDistance(posX, posY, centerX, centerY), tmpRange;
        
        if (centerX > 0) {
            // left up
            tmpRange = getDistance(posX, posY, centerX - sqrt(3) / 2 * towerLength, centerY - 1.5 * towerLength);
            if (tmpRange < minRange) {
                minRange = tmpRange;
                minI = i - 1;
                minJ = j - 1;
            }
            
            // left down
            tmpRange = getDistance(posX, posY, centerX - sqrt(3) / 2 * towerLength, centerY + 1.5 * towerLength);
            if (tmpRange < minRange) {
                minRange = tmpRange;
                minI = i - 1;
                minJ = j + 1;
            }
        }
        // right up
        tmpRange = getDistance(posX, posY, centerX + sqrt(3) / 2 * towerLength, centerY - 1.5 * towerLength);
        if (tmpRange < minRange) {
            minRange = tmpRange;
            minI = i;
            minJ = j - 1;
        }
        
        // right down
        tmpRange = getDistance(posX, posY, centerX + sqrt(3) / 2 * towerLength, centerY + 1.5 * towerLength);
        if (tmpRange < minRange) {
            minRange = tmpRange;
            minI = i;
            minJ = j + 1;
        }
        
        i = minI;
        j = minJ;
        
        return;
    }
}

position_t TowerHexagonAOI::getDistance(position_t posX, position_t posY, position_t anotherPosX, position_t anotherPosY) {
    return (posX - anotherPosX) * (posX - anotherPosX) + (posY - anotherPosY) * (posY - anotherPosY);
}

void TowerHexagonAOI::getCenterByTowerPos(position_t i, position_t j, position_t &centerX, position_t &centerY) {
    centerY = 0.5 * towerLength + 1.5 * towerLength * j;
    if (0 == j % 2) {
        centerX = sqrt(3) / 2 * towerLength + sqrt(3) * towerLength * i;
    } else {
        centerX = sqrt(3) * towerLength * i;
    }
    return;
}






