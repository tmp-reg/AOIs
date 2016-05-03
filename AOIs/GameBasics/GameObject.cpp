//
//  GameObject.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "GameObject.hpp"
#include <iostream>

#include "GameWorld.hpp"

using namespace std;

GameObject::GameObject(entity_t id, type_t type, position_t posX, position_t posY, position_t range): id(id), type(type), posX(posX), posY(posY), range(range), messageNum(0), addMessageNum(0), leaveMessageNum(0), moveMessageNum(0){
    this -> tree = NULL;
    //cout << "Create  !!! GameObject id: " << id << ", type: " << uint16_t(type) << ", posX: " << posX << ", posY: " << posY << ", range: " << range << endl;
}

GameObject::~GameObject() {
    //cout << "Leave   !!! GameObject id: " << id << ", type: " << uint16_t(type) << ", posX: " << posX << ", posY: " << posY << ", range: " << range << endl;
}

void GameObject::move(position_t newPosX, position_t newPosY) {
    this -> world -> onObjectMove(this, newPosX, newPosY);
}

void GameObject::receiveMessagesFromPublisher(GameObject *publisher, state_t state) {
    messageNum ++;
    
    if (ADD_MESSAGE == state) {
        addMessageNum ++;
        this -> world -> addMessageNum ++;
    } else if (REMOVE_MESSAGE == state) {
        leaveMessageNum ++;
        this -> world -> leaveMessageNum ++;
    } else if (MOVE_MESSAGE == state) {
        moveMessageNum ++;
        this -> world -> moveMessageNum ++;
    }
    
    //cout << (uint16_t)state << " : " << this -> id << " receives from " << publisher -> id << endl;
}

void GameObject::receiveMessagesFromPublishers(list<GameObject *> publishers, state_t state) {
    entity_t num = (entity_t)publishers.size();
    
    messageNum += num;
    
    list<GameObject *>::iterator iter;
    for (iter = publishers . begin(); iter != publishers . end(); iter ++) {
        //cout << (uint16_t)state << " : " << this -> id << " receives from " << (*iter) -> id << endl;
    }
    
    if (ADD_MESSAGE == state) {
        addMessageNum += num;
        this -> world -> addMessageNum += num;
    } else if (REMOVE_MESSAGE == state) {
        leaveMessageNum += num;
        this -> world -> leaveMessageNum += num;
    } else if (MOVE_MESSAGE == state) {
        moveMessageNum += num;
        this -> world -> moveMessageNum += num;
    }
    
}


