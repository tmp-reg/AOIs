//
//  XYListAOIService.cpp
//  AOIs
//
//  Created by zklgame on 4/7/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "XYListAOIService.hpp"

XYListAOIService::XYListAOIService() {
    this -> aoi = new XYListAOI();
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: XYListAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

XYListAOIService::~XYListAOIService() {
    delete this -> aoi;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: ~XYListAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}