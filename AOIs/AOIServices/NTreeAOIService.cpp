//
//  NTreeAOIService.cpp
//  AOIs
//
//  Created by zklgame on 4/21/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "NTreeAOIService.hpp"

NTreeAOIService::NTreeAOIService(position_t worldWidth, position_t worldHeight, entity_t towerMaxPublisherNum, position_t maxLevel) {
    this -> aoi = new NTreeAOI(worldWidth, worldHeight, towerMaxPublisherNum, maxLevel);
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: NTreeAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

NTreeAOIService::~NTreeAOIService() {
    delete this -> aoi;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: ~NTreeAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

