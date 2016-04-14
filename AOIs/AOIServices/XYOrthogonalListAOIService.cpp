//
//  XYOrthogonalListAOIService.cpp
//  AOIs
//
//  Created by zklgame on 4/13/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "XYOrthogonalListAOIService.hpp"

XYOrthogonalListAOIService::XYOrthogonalListAOIService() {
    this -> aoi = new XYOrthogonalListAOI();
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: XYOrthogonalListAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}

XYOrthogonalListAOIService::~XYOrthogonalListAOIService() {
    delete this -> aoi;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cout << "AOI: ~XYOrthogonalListAOIService" << endl;
    cout << "&&&&&&&&&&&&&&&&&&&&&&&&&\n" << endl;
}