//
//  EuDistanceAOITest.cpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "../../GameBasics/GameWorld.hpp"
#include "../../AOIServices/EuDistanceAOIService.hpp"

#include <iostream>
#include <vector>

using namespace std;

FILE *fp;
int width = 10000;
int height = 10000;

int objectNum = 10000;
int movedNum = 5000;
int leaveNum = 5000;
int maxRange = 200;
int maxMove = 100;

GameWorld *world;
vector<GameObject *> gameObjects, movedObjects, leavedObjects;
vector<GameObject *>::iterator iter;

vector<int> movedPosX;
vector<int> movedPosY;

void initGame(int i);
void deInitGame();

void test(int i);
void testAdd(int i);
void testMove(int i);
void testLeave(int i);

int main222() {
    
    for (int i = 0; i < 5; i ++) {
        test(i + 1);
    }

    return 0;
}

void test(int i) {
    initGame(i);
    
    testAdd(i);
    testMove(i);
    testLeave(i);
    
    deInitGame();
}

void initGame(int i) {
    gameObjects.clear();
    movedObjects.clear();
    leavedObjects.clear();
    movedPosX.clear();
    movedPosY.clear();
    
    
    world = new GameWorld();
    world -> aoi = new EuDistanceAOIService();
    
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

void deInitGame() {
    for (int i = 0; i < objectNum; i ++) {
        delete gameObjects[i + 1];
    }
    delete world -> aoi;
    delete world;
}

void testAdd(int i) {
    for (iter = gameObjects . begin(); iter != gameObjects . end(); iter ++) {
        world -> addObject(*iter);
    }
}

void testMove(int i) {
    for (int i = 0; i < movedObjects.size(); i ++) {
        movedObjects[i] -> move(movedPosX[i], movedPosY[i]);
    }
}

void testLeave(int i) {
    for (int i = 0; i < leavedObjects.size(); i ++) {
        world -> removeObject(leavedObjects[i]);
    }
}


