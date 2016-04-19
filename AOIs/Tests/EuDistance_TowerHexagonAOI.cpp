//
//  EuDistance_TowerHexagonAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/19/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "../GameBasics/GameWorld.hpp"
#include "../AOIServices/EuDistanceAOIService.hpp"
#include "../AOIServices/TowerHexagonAOIService.hpp"

#include <iostream>
#include <vector>

int main() {
    // create object data
    entity_t objectNum = 10000;
    entity_t movedNum = 5000;
    entity_t leaveNum = 2000;
    
    vector<GameObject *> gameObjects, gameObjectsCopy2;
    vector<GameObject *> MovedgameObjects1, MovedgameObjects2;
    vector<GameObject *> leavedObjects1, leavedObjects2;
    vector<int> movedPosX;
    vector<int> movedPosY;
    
    entity_t id;
    type_t type;
    position_t posX, posY, range;
    position_t maxRange = 200;
    
    srand((int)time(0));
    for (int i = 0; i < objectNum; i ++) {
        type = rand() % 3 + 1;
        posX = rand() % 10000;
        posY = rand() % 10000;
        range = rand() % maxRange;
        GameObject *obj = new GameObject(i, type, posX, posY, range);
        GameObject *obj2 = new GameObject(i, type, posX, posY, range);
        gameObjects.push_back(obj);
        gameObjectsCopy2.push_back(obj2);
    }
    
    srand((int)time(0));
    for (int i = 0; i < movedNum; i ++) {
        id = rand() % objectNum;
        MovedgameObjects1.push_back(gameObjects[id]);
        MovedgameObjects2.push_back(gameObjectsCopy2[id]);
        movedPosX.push_back(rand() % 500);
        movedPosY.push_back(rand() % 500);
    }
    
    srand((int)time(0));
    for (int i = 0; i < leaveNum; i ++) {
        id = rand() % objectNum;
        leavedObjects1.push_back(gameObjects[id]);
        leavedObjects2.push_back(gameObjectsCopy2[id]);
    }
    
    GameWorld *world1 = new GameWorld();
    world1 -> aoi = new EuDistanceAOIService();
    GameWorld * world2 = new GameWorld();
    world2 -> aoi = new TowerHexagonAOIService(10000, 10000, 100);
    
    //////////////////////
    // test addObject
    //////////////////////
    vector<GameObject *>::iterator iter;
    for (iter = gameObjects . begin(); iter != gameObjects . end(); iter ++) {
        world1 -> addObject(*iter);
    }
    for (iter = gameObjectsCopy2 . begin(); iter != gameObjectsCopy2 . end(); iter ++) {
        world2 -> addObject(*iter);
    }
    
    bool isAddRight = true;
    for (int i = 0; i < objectNum; i ++) {
        if (gameObjects[i] -> messageNum == gameObjectsCopy2[i] -> messageNum &&
            gameObjects[i] -> addMessageNum == gameObjectsCopy2[i] -> addMessageNum &&
            gameObjects[i] -> leaveMessageNum == gameObjectsCopy2[i] -> leaveMessageNum &&
            gameObjects[i] -> moveMessageNum == gameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isAddRight : FALSE!!! at " << i << endl << endl;
            isAddRight = false;
            return 1;
        }
    }
    
    cout << "??????????????????????????????????" << endl;
    cout << "ADD OBJECT PASS" << endl;
    cout << "??????????????????????????????????" << endl << endl;
    
    //////////////////////
    // test moveObject
    //////////////////////
    for (int i = 0; i < MovedgameObjects1.size(); i ++) {
        MovedgameObjects1[i] -> move(movedPosX[i], movedPosY[i]);
    }
    for (int i = 0; i < MovedgameObjects2.size(); i ++) {
        MovedgameObjects2[i] -> move(movedPosX[i], movedPosY[i]);
    }
    
    bool isMoveRight = true;
    for (int i = 0; i < objectNum; i ++) {
        if (gameObjects[i] -> messageNum == gameObjectsCopy2[i] -> messageNum &&
            gameObjects[i] -> addMessageNum == gameObjectsCopy2[i] -> addMessageNum &&
            gameObjects[i] -> leaveMessageNum == gameObjectsCopy2[i] -> leaveMessageNum &&
            gameObjects[i] -> moveMessageNum == gameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isMoveRight : FALSE!!! at " << i << endl << endl;
            isMoveRight = false;
            return 1;
        }
    }
    
    cout << "??????????????????????????????????" << endl;
    cout << "MOVE OBJECT PASS" << endl;
    cout << "??????????????????????????????????" << endl << endl;
    
    
    //////////////////////
    // test removeObject
    //////////////////////
    for (int i = 0; i < leavedObjects1.size(); i ++) {
        world1 -> removeObject(leavedObjects1[i]);
    }
    for (int i = 0; i < leavedObjects2.size(); i ++) {
        world2 -> removeObject(leavedObjects2[i]);
    }
    
    bool isRemoveRight = true;
    for (int i = 0; i < objectNum; i ++) {
        if (gameObjects[i] -> messageNum == gameObjectsCopy2[i] -> messageNum &&
            gameObjects[i] -> addMessageNum == gameObjectsCopy2[i] -> addMessageNum &&
            gameObjects[i] -> leaveMessageNum == gameObjectsCopy2[i] -> leaveMessageNum &&
            gameObjects[i] -> moveMessageNum == gameObjectsCopy2[i] -> moveMessageNum) {
            
        } else {
            cout << "isRemoveRight : FALSE!!! at " << i << endl << endl;
            isRemoveRight = false;
            return 1;
        }
    }
    
    cout << "??????????????????????????????????" << endl;
    cout << "REMOVE OBJECT PASS" << endl;
    cout << "??????????????????????????????????" << endl << endl;
    
    delete world1 -> aoi;
    delete world1;
    delete world2 -> aoi;
    delete world2;
    
    return 0;
}