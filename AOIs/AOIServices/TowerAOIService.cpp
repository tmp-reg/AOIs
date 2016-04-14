//
//  TowerAOIService.cpp
//  AOIs
//
//  Created by zklgame on 4/14/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "TowerAOIService.hpp"

TowerAOIService::TowerAOIService(position_t worldWidth, position_t worldHeight, position_t towerWidth, position_t towerHeight) {
    this -> aoi = new TowerAOI(worldWidth, worldHeight, towerWidth, towerHeight);
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: TowerAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

TowerAOIService::~TowerAOIService() {
    delete this -> aoi;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: ~TowerAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}
