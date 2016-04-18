//
//  TowerHexagonAOIService.cpp
//  AOIs
//
//  Created by zklgame on 4/18/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "TowerHexagonAOIService.hpp"

TowerHexagonAOIService::TowerHexagonAOIService(position_t worldWidth, position_t worldHeight, position_t towerLength) {
    this -> aoi = new TowerHexagonAOI(worldWidth, worldHeight, towerLength);
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: TowerHexagonAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

TowerHexagonAOIService::~TowerHexagonAOIService() {
    delete this -> aoi;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: ~TowerHexagonAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}