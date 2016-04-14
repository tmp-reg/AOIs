//
//  TowerAOIService.hpp
//  AOIs
//
//  Created by zklgame on 4/14/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef TowerAOIService_hpp
#define TowerAOIService_hpp

#include "AOIService.hpp"
#include "../AOIModels/TowerAOI/TowerAOI.hpp"

class TowerAOIService: public AOIService {
public:
    TowerAOIService(position_t worldWidth, position_t worldHeight, position_t towerWidth, position_t towerHeight);
    virtual ~TowerAOIService();
};


#endif /* TowerAOIService_hpp */
