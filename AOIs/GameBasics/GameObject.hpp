//
//  GameObject.hpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include "../alias.h"
#include <list>
#include <vector>
#include <map>

using namespace std;

class GameWorld;

class NTree;

class GameObject {
    
public:
    GameObject(entity_t id, type_t type, position_t posX, position_t posY, position_t range);
    virtual ~GameObject();
    
    entity_t id;
    type_t type; // 1: publisher only; 2: subscriber only; 3: both two
    
    position_t posX;
    position_t posY;
    
    position_t range; // 影响范围
    
    void move(position_t newPosX, position_t newPosY);
    
    // state means whether it is an add message or a leave message
    // 1 is add, 2 is leave, 3 is move
    void receiveMessagesFromPublisher(GameObject * publisher, state_t state);
    void receiveMessagesFromPublishers(list<GameObject *> publishers, state_t state);
    
    GameWorld * world;

    // to test the performance
    entity_t messageNum;
    entity_t addMessageNum;
    entity_t leaveMessageNum;
    entity_t moveMessageNum;
    map<entity_t, entity_t> addMessageDetail, removeMessageDetail;
    
    NTree *tree;
    
    // for test
    //vector<entity_t> addMessageDetail, removeMessageDetail;
    
    ///////////////////////////////////////
    // the properties below is optional //
    /////////////////////////////////////

    // record the position in XYListAOI
    list<GameObject *>::iterator listPosX;
    list<GameObject *>::iterator listPosY;
    
    // record the position in XYOrthogonalListAOI
    struct Node {
        position_t pos;
        list<Node *>::iterator iterPos;
        list<GameObject *> objList;
    };
    list<Node *>::iterator listNodePosX;
    list<Node *>::iterator listNodePosY;
    list<GameObject *>::iterator listSubPosX;
    list<GameObject *>::iterator listSubPosY;
    
};


#endif /* GameObject_hpp */
