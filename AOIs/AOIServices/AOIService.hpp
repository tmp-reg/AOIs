//
//  AOIService.hpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef AOIService_hpp
#define AOIService_hpp

#include "../AOIModels/BaseAOI/BaseAOI.hpp"

#include <iostream>
using namespace::std;

// the BASE class of all the AoiServiceClasses
class AOIService {
public:
    AOIService() {};
    virtual ~AOIService() {};
    
    virtual bool addObject(GameObject *obj) {
        return this -> aoi -> addObject(obj);
    }
    virtual bool removeObject(GameObject *obj) {
        return this -> aoi -> removeObject(obj);
    }
    virtual bool onObjectMove(GameObject *obj, position_t oldPosX, position_t oldPosY) {
        return this -> aoi -> onObjectMove(obj, oldPosX, oldPosY);
    }
    
protected:
    BaseAOI *aoi;
};

#endif /* AOIService_hpp */
