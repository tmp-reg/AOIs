//
//  EuDistance_TowerAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/14/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "../GameBasics/GameWorld.hpp"
#include "../AOIServices/EuDistanceAOIService.hpp"
#include "../AOIServices/TowerAOIService.hpp"

#include <iostream>
#include <vector>

using namespace std;

int main3() {
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
    world2 -> aoi = new TowerAOIService(10000, 10000, 100, 100);
    
    //////////////////////
    // test addObject
    //////////////////////
    vector<GameObject *>::iterator iter;
    for (iter = gameObjects . begin(); iter != gameObjects . end(); iter ++) {
        world1 -> addObject(*iter);
    }
    for (iter = gameObjectsCopy2 . begin(); iter != gameObjectsCopy2 . end(); iter ++) {
        //cout << (*iter) -> id << " " << (*iter) -> posX << " " << (*iter) -> posY << " " << uint16_t((*iter) -> type) << endl;
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

//
////test code
//cout << gameObjects[i] -> id << " | " << uint16_t(gameObjects[i] -> type) << " | " << gameObjects[i] -> posX << " | " << gameObjects[i] -> posY << " | " << gameObjects[i] -> range << " | " << gameObjects[i] -> messageNum << " | " << gameObjects[i] -> addMessageNum << " | " << gameObjects[i] -> moveMessageNum << " | " << gameObjects[i] -> leaveMessageNum << endl;
//
//cout << i << " | " << uint16_t(gameObjectsCopy2[i] -> type) << " | " << gameObjectsCopy2[i] -> posX << " | " << gameObjectsCopy2[i] -> posY << " | " << gameObjectsCopy2[i] -> range << " | " << gameObjectsCopy2[i] -> messageNum << " | " << gameObjectsCopy2[i] -> addMessageNum << " | " << gameObjectsCopy2[i] -> moveMessageNum << " | " << gameObjectsCopy2[i] -> leaveMessageNum << endl;
//
//map<entity_t, entity_t>::iterator listIter;
//cout << "more: " << endl;
//for (listIter = gameObjectsCopy2[i] -> addMessageDetail . begin(); listIter != gameObjectsCopy2[i] -> addMessageDetail . end(); listIter ++) {
//    if (gameObjects[i] -> addMessageDetail . find(listIter -> first) == gameObjects[i] -> addMessageDetail . end()) {
//        cout << listIter -> first << " | " << uint16_t(gameObjectsCopy2[listIter -> first] -> type) << " | " << gameObjectsCopy2[listIter -> first] -> posX << " | " << gameObjectsCopy2[listIter -> first] -> posY << endl;
//    }
//}
//
//cout << "less: " << endl;
//for (listIter = gameObjects[i] -> addMessageDetail . begin(); listIter != gameObjects[i] -> addMessageDetail . end(); listIter ++) {
//    if (gameObjectsCopy2[i] -> addMessageDetail . find(listIter -> first) == gameObjectsCopy2[i] -> addMessageDetail . end()) {
//        cout << listIter -> first << " | " << uint16_t(gameObjectsCopy2[listIter -> first] -> type) << " | " << gameObjectsCopy2[listIter -> first] -> posX << " | " << gameObjectsCopy2[listIter -> first] -> posY << endl;
//    }
//}