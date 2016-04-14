//
//  GameWorld.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "GameWorld.hpp"
#include <iostream>

using namespace std;

GameWorld::GameWorld() {
    new(this)GameWorld(10000, 10000);
}

GameWorld::GameWorld(position_t width, position_t length): width(width), length(length), entityNum(0) {
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "Create  !!! GameWorld width: " << width << ", length: " << length << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" << endl;
}

GameWorld::~GameWorld() {
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "Destroy !!! GameWorld" << endl;
    cout << "GameObjects Num: " << this -> entityNum << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" << endl;
}

bool GameWorld::addObject(GameObject *obj) {
    this -> gameObjects[obj -> id] = obj;
    entityNum ++;
    
    obj -> world = this;
    
    return this -> aoi -> addObject(obj);    
}

bool GameWorld::removeObject(GameObject *obj) {
    bool isRemoved= false;
    
    if (this -> gameObjects. find(obj -> id) != this -> gameObjects . end()) {
        this -> gameObjects . erase(obj -> id);
        entityNum --;
        
        this -> aoi -> removeObject(obj);
        
        isRemoved = true;
        
    }
    
    return isRemoved;
}

bool GameWorld::onObjectMove(GameObject *obj, position_t newPosX, position_t newPosY) {
    position_t oldPosX = obj -> posX;
    position_t oldPosY = obj -> posY;
    
    obj -> posX = newPosX;
    obj -> posY = newPosY;
    
    return this -> aoi -> onObjectMove(obj, oldPosX, oldPosY);
}



