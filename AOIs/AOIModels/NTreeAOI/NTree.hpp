//
//  NTree.hpp
//  AOIs
//
//  Created by zklgame on 4/19/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef NTree_hpp
#define NTree_hpp

#include "../../GameBasics/GameObject.hpp"
#include <stdlib.h>
#include <map>
#include <cmath>

#include <iostream>

class NTree {
public:
    NTree(position_t minX, position_t maxX, position_t minY, position_t maxY, entity_t maxPublisherNum, position_t maxLevel, position_t level);
    virtual ~NTree();
    
    bool addPublisher(GameObject *obj);
    bool removePublisher(GameObject *obj); // if the sum of all the publishers in children < 1 /2 * maxPublisherNum, merge
    bool removePublisherByPos(entity_t objId, position_t posX, position_t posY);
    
    map<entity_t, GameObject *> findPublishersInRange(position_t posX, position_t posY, position_t range);

private:
    // subscribers are found by traversal
    // so split is just decided by the number of publishers
    map<entity_t, GameObject *> publishers;

    bool isSplit;
    entity_t maxPublisherNum;
    int childrenNum;

    position_t minX, maxX, minY, maxY;
    position_t maxLevel;
    position_t level;

    NTree * children[4];
    NTree * parent;

    entity_t getPublisherNum();
    map<entity_t, GameObject *> getAllPublishers();
    void mergeChildTrees();

    void classifyTree(position_t posX, position_t posY, position_t range, list<NTree *> &fullCoveredTrees, list<NTree *> &partialCoveredTrees);

    // tool funcs
    inline bool isInRange(position_t pos, position_t otherPos, position_t range) {
        position_t del = pos >= otherPos ? pos - otherPos : otherPos - pos;
        return del <= range;
    }
    
    inline bool isInRange2(position_t posX, position_t posY, position_t otherPosX, position_t otherPosY, position_t range) {
        position_t distance2 = (posX - otherPosX) * (posX - otherPosX) + (posY - otherPosY) * (posY - otherPosY);
        return distance2 <= range * range;
    }
    
    inline bool isFullCoveredTree(position_t posX, position_t posY, position_t range) {
        position_t pos1X = minX;
        position_t pos1Y = minY;
        position_t pos2X = maxX;
        position_t pos2Y = maxY;
        if ((isInRange2(posX, posY, pos1X, pos1Y, range)) &&
            (isInRange2(posX, posY, pos2X, pos2Y, range)) &&
            (isInRange2(posX, posY, pos1X, pos2Y, range)) &&
            (isInRange2(posX, posY, pos2X, pos1Y, range))) {
            return true;
        }
        
        return false;
    }

    inline bool isPartialCoveredTree(position_t posX, position_t posY, position_t range) {
        position_t pos1X = minX;
        position_t pos1Y = minY;
        position_t pos2X = maxX;
        position_t pos2Y = maxY;
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
        
        // pos in this rect
        if (posX >= minX && posX < maxX && posY >= minY && posY < maxY) {
            return true;
        }
        
        return false;
    }
    
    inline bool isCoveredBySide(position_t posX, position_t posY, position_t range, position_t pos1X, position_t pos1Y, position_t pos2X, position_t pos2Y) {
        if (isInRange(posX, pos1X, range)) {
            position_t dis2 = range * range - (posX - pos1X) * (posX - pos1X);
            position_t testposY = sqrt(dis2) + posY;
            return (testposY * testposY) >= pos1Y * pos1Y && (testposY * testposY) <= pos2Y * pos2Y;
        }
        
        return false;
    }
    
};

//    void pp(int n) {
//        for (int i = 0; i < n; i ++) {
//            cout << " ";
//        }
//        if (this -> parent) {
//            cout << this << " " << this -> parent << " " << minX << " " << maxX << " " << minY << " " << maxY << " "  << endl;
//        } else {
//            cout << this << " " << minX << " " << maxX << " " << minY << " " << maxY << " "  << endl;
//        }
//        map<entity_t, GameObject *>::iterator iter;
//        for (iter = this -> publishers . begin(); iter != this -> publishers . end(); iter ++) {
//            cout << iter -> first << " | " << iter -> second -> posX << " | " << iter -> second -> posY << " | " << iter -> second -> range << " " << uint16_t(iter -> second -> type) << endl;
//        }
//        if (isSplit) {
//            for (int i = 0; i < 4; i ++) {
//                children[i] -> pp(n + 1);
//            }
//        }
//    }
//
//    void tt() {
//        if (!isSplit) {
//            map<entity_t, GameObject *> maps = this -> publishers;
//            map<entity_t, GameObject *>::iterator iter;
//            for (iter = maps.begin(); iter != maps.end(); iter ++) {
//                if (iter -> second -> posX < minX || iter -> second -> posX >= maxX || iter -> second -> posY < minY || iter -> second -> posY >= maxY) {
//                    cout << "RRRRRRRRRRRRRRRRRRRRR " << iter -> second -> id << " " << iter -> second -> posX << " " << iter -> second -> posY << endl;
//                    cout << minX << " " << maxX << " " << minY << " " << maxX << endl;
//                }
//            }
//
//        } else {
//            for (int i = 0; i < 4; i ++) {
//                this -> children[i] -> tt();
//            }
//        }
//    }

#endif /* NTree_hpp */
