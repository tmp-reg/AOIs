//
//  AOIEventManager.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "AOIEventManager.hpp"

void AOIEventManager::onAddPublisher(GameObject *theObj, list<GameObject *> subscribers) {
    list<GameObject *>::iterator iter;
    for (iter = subscribers.begin(); iter != subscribers.end(); iter ++) {
        (*iter) -> receiveMessagesFromPublisher(theObj, ADD_MESSAGE);
    }
}

void AOIEventManager::onAddSubscriber(GameObject *theObj, list<GameObject *> publishers) {
    theObj -> receiveMessagesFromPublishers(publishers, ADD_MESSAGE);
}

void AOIEventManager::onRemovePublisher(GameObject *theObj, list<GameObject *> subscribers) {
    list<GameObject *>::iterator iter;
    for (iter = subscribers.begin(); iter != subscribers.end(); iter ++) {
        (*iter) -> receiveMessagesFromPublisher(theObj, REMOVE_MESSAGE);
    }
}

void AOIEventManager::onRemoveSubscriber(GameObject *theObj, list<GameObject *> publishers) {
    theObj -> receiveMessagesFromPublishers(publishers, REMOVE_MESSAGE);
}

void AOIEventManager::onMovePublisher(GameObject *theObj, list<GameObject *> subscribers) {
    list<GameObject *>::iterator iter;
    for (iter = subscribers.begin(); iter != subscribers.end(); iter ++) {
        (*iter) -> receiveMessagesFromPublisher(theObj, MOVE_MESSAGE);
    }
}