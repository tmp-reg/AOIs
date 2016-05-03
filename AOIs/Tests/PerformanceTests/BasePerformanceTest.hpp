//
//  BasePerformanceTest.hpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef BasePerformanceTest_hpp
#define BasePerformanceTest_hpp

#include <stdio.h>
#include "../../GameBasics/GameWorld.hpp"

class BasePerformanceTest {
public:
    BasePerformanceTest();
    virtual ~BasePerformanceTest();
    
    GameWorld *world;
    
    void test(int i);
    
private:
    // basic
    FILE *fp;
    int width = 10000;
    int height = 10000;
    
    int objectNum = 10000;
    int movedNum = 5000;
    int leaveNum = 5000;
    int maxRange = 200;
    int maxMove = 100;
    
    // to set
    vector<GameObject *> gameObjects, movedObjects, leavedObjects;
    vector<int> movedPosX;
    vector<int> movedPosY;
    
    void testAdd(int i);
    void testMove(int i);
    void testLeave(int i);
    
    void initGame(int i);
    void deInitGame();
};


#endif /* BasePerformanceTest_hpp */
