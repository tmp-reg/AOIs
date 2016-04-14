//
//  EuDistanceAOIService.cpp
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "EuDistanceAOIService.hpp"

EuDistanceAOIService::EuDistanceAOIService() {
    this -> aoi = new EuDistanceAOI();
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: EuDistanceAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

EuDistanceAOIService::~EuDistanceAOIService() {
    delete this -> aoi;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: ~EuDistanceAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

