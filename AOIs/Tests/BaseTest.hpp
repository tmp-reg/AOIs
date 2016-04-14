//
//  BaseTest.hpp
//  AOIs
//
//  Created by zklgame on 4/7/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef BaseTest_hpp
#define BaseTest_hpp

#include "../GameBasics/GameWorld.hpp"

class BaseTest {
public:
    BaseTest() { gameWorld = new GameWorld(10000, 10000); };
    virtual ~BaseTest() { delete gameWorld; };
    
    // add gameObjects to gameWorld
    // TODO
    
    // move gameObjects
    // TODO
    
    // destroy gameObjects
    // TODO
    
protected:
    GameWorld *gameWorld;
};

#endif /* BaseTest_hpp */
