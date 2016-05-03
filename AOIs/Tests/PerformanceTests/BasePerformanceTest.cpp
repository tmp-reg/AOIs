//
//  BasePerformanceTest.cpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "BasePerformanceTest.hpp"

BasePerformanceTest::BasePerformanceTest() {
    this -> world = new GameWorld(width, height);
    
    // !!!!
    // aoi is set out of the class !!!
}

BasePerformanceTest::~BasePerformanceTest() {
    if (this -> world -> aoi) {
        delete this -> world -> aoi;
    }
    delete world;
}

void BasePerformanceTest::test(int i) {
    initGame(i);
    
    testAdd(i);
    testMove(i);
    testLeave(i);
    
    deInitGame();
}

void BasePerformanceTest::testAdd(int i) {
    vector<GameObject *>::iterator iter;
    for (iter = gameObjects . begin(); iter != gameObjects . end(); iter ++) {
        world -> addObject(*iter);
    }
    
    
}

void BasePerformanceTest::testMove(int i) {
    for (int i = 0; i < movedObjects.size(); i ++) {
        movedObjects[i] -> move(movedPosX[i], movedPosY[i]);
    }
}

void BasePerformanceTest::testLeave(int i) {
    for (int i = 0; i < leavedObjects.size(); i ++) {
        world -> removeObject(leavedObjects[i]);
    }
}

void BasePerformanceTest::initGame(int i) {
    gameObjects.clear();
    movedObjects.clear();
    leavedObjects.clear();
    movedPosX.clear();
    movedPosY.clear();
        
    int id, type, posX, posY, range;
    
    char num[2];
    sprintf(num, "%d", i);
    // read init data
    char path[100] = "/Users/zklgame/WorkPlace/AOIs/AOIs/Tests/Datas/data";
    strcat(path, num);
    strcat(path, "/data_init.txt");
    fp = fopen(path, "r+");
    if (NULL == fp) {
        printf("ERROR! read init error\n");
        printf("%s\n", path);
        return;
    }
    
    while (fscanf(fp, "%d %d %d %d %d\n", &id, &type, &posX, &posY, &range) != EOF) {
        GameObject *obj = new GameObject(id, type, posX, posY, range);
        gameObjects.push_back(obj);
    }
    
    // read move data
    char movePath[100] = "/Users/zklgame/WorkPlace/AOIs/AOIs/Tests/Datas/data";
    strcat(movePath, num);
    strcat(movePath, "/data_move.txt");
    fp = fopen(movePath, "r+");
    if (NULL == fp) {
        printf("ERROR! read move error\n");
        printf("%s\n", movePath);
        return;
    }
    
    int tmpX, tmpY;
    while (fscanf(fp, "%d %d %d %d\n", &i, &id, &posX, &posY) != EOF) {
        movedObjects.push_back(gameObjects[id]);
        if (i < movedNum * 0.2) {
            movedPosX.push_back(posX);
            movedPosY.push_back(posY);
        } else {
            tmpX = gameObjects[id] -> posX + posX;
            tmpY = gameObjects[id] -> posY + posY;
            if (tmpX < 0) {
                tmpX = 0;
            } else if (tmpX >= width) {
                tmpX = width - 1;
            }
            if (tmpY < 0) {
                tmpY = 0;
            } else if (tmpY >= height) {
                tmpY = height - 1;
            }
            movedPosX.push_back(tmpX);
            movedPosY.push_back(tmpY);
        }
    }
    
    // read leave data
    char leavePath[100] = "/Users/zklgame/WorkPlace/AOIs/AOIs/Tests/Datas/data";
    strcat(leavePath, num);
    strcat(leavePath, "/data_leave.txt");
    fp = fopen(leavePath, "r+");
    if (NULL == fp) {
        printf("ERROR! read leave error\n");
        printf("%s\n", leavePath);
        return;
    }
    
    while (fscanf(fp, "%d\n", &id) != EOF) {
        leavedObjects.push_back(gameObjects[id]);
    }
}

void BasePerformanceTest::deInitGame() {
    for (int i = 0; i < objectNum; i ++) {
        delete gameObjects[i + 1];
    }
}