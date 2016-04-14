//
//  XYOrthogonalListAOI.hpp
//  AOIs
//
//  Created by zklgame on 4/12/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef XYOrthogonalListAOI_hpp
#define XYOrthogonalListAOI_hpp

#include "../BaseAOI/BaseAOI.hpp"
#include <list>

class XYOrthogonalListAOI: public BaseAOI {
public:
    XYOrthogonalListAOI();
    virtual ~XYOrthogonalListAOI();
    
private:
    list<GameObject::Node *> listX, listY;
    
    bool addPublisher(GameObject *obj);
    bool addSubscriber(GameObject *obj);
    bool removePublisher(GameObject *obj);
    bool removeSubscriber(GameObject *obj);

    bool onPublisherMove(GameObject *obj, position_t oldPosX, position_t oldPosY);
    bool onSubscriberMove(GameObject *obj, position_t oldPosX, position_t oldPosY);

    // ADD or REMOVE: find by obj
    // if state = 1, add; if state = 2, remove
    void findSubscribersInTheirRange(GameObject *obj, state_t state);
    void findPublishersInRange(GameObject *obj, state_t state);

    // MOVE: find by position
    map<entity_t, GameObject *> findSubscribersInTheirRange(entity_t objId, position_t posX, position_t posY);
    map<entity_t, GameObject *> findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range) { return map<entity_t, GameObject *>(); };

    bool addObjectInList(GameObject *obj, state_t listStyle);
    bool removeObjectInList(GameObject *obj, state_t listStyle);
    
    map<entity_t, GameObject *> findPublishersInRange(entity_t objId, position_t posX, position_t posY, position_t range, list<GameObject::Node *>::iterator listNodePosX, list<GameObject::Node *>::iterator listNodePosY, list<GameObject *>::iterator listSubPosX, list<GameObject *>::iterator listSubPosY);
};

#endif /* XYOrthogonalListAOI_hpp */
