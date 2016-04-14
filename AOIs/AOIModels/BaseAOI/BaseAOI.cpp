//
//  BaseAOI.cpp
//  AOIs
//
//  Created by zklgame on 4/7/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "BaseAOI.hpp"

bool BaseAOI::addObject(GameObject *obj) {
    switch (obj -> type) {
        case PUBLISHER:
            this -> addPublisher(obj);
            break;
        case SUBSCRIBER:
            this -> addSubscriber(obj);
            break;
        case PUB_SUB:
            this -> addPublisher(obj);
            this -> addSubscriber(obj);
            break;
        default:
            break;
    }
    
    return true;
}

bool BaseAOI::removeObject(GameObject *obj) {
    switch (obj -> type) {
        case PUBLISHER:
            this -> removePublisher(obj);
            break;
        case SUBSCRIBER:
            this -> removeSubscriber(obj);
            break;
        case PUB_SUB:
            this -> removePublisher(obj);
            this -> removeSubscriber(obj);
        default:
            break;
    }
    
    return true;
}

bool BaseAOI::onObjectMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
    switch (obj -> type) {
        case PUBLISHER:
            this -> onPublisherMove(obj, oldPosX, oldPosY);
            break;
        case SUBSCRIBER:
            this -> onSubscriberMove(obj, oldPosX, oldPosY);
            break;
        case PUB_SUB:
            this -> onPublisherMove(obj, oldPosX, oldPosY);
            this -> onSubscriberMove(obj, oldPosX, oldPosY);
        default:
            break;
    }
    
    return true;
}

