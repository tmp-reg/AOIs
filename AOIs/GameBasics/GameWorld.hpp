//
//  GameWorld.hpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef GameWorld_hpp
#define GameWorld_hpp

#include "GameObject.hpp"
#include "../AOIServices/AOIService.hpp"

#include <map>

using namespace std;

class GameWorld {
    
public:
    GameWorld();
    GameWorld(position_t width, position_t length);
    virtual ~GameWorld();
    
    position_t width;
    position_t length;
    
    entity_t entityNum;
    
    AOIService * aoi;
    
    // Suppose GameWorld gets GameObject* from the LoginApp
    // And the GameWorld do nothing with GameObject's creation or destruction
    bool addObject(GameObject *obj);
    bool removeObject(GameObject *obj);
    
    bool onObjectMove(GameObject *obj, position_t newPosX, position_t newPosY);
    
private:
    map<entity_t, GameObject*> gameObjects;
    
};

#endif /* GameWorld_hpp */
