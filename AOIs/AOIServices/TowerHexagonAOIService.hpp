//
//  TowerHexagonAOIService.hpp
//  AOIs
//
//  Created by zklgame on 4/18/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef TowerHexagonAOIService_hpp
#define TowerHexagonAOIService_hpp

#include "AOIService.hpp"
#include "../AOIModels/TowerHexagonAOI/TowerHexagonAOI.hpp"

class TowerHexagonAOIService: public AOIService {
public:
    TowerHexagonAOIService(position_t worldWidth, position_t worldHeight, position_t towerLength);
    virtual ~TowerHexagonAOIService();
};

#endif /* TowerHexagonAOIService_hpp */
