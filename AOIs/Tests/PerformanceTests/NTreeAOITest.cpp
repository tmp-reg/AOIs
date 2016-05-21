//
//  NTreeAOITest.cpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "BasePerformanceTest.hpp"
#include "../../AOIServices/NTreeAOIService.hpp"

int main() {
    
    int i = 0;
    
    cout << "Test " << i + 1 << " :" << endl;
    BasePerformanceTest *test = new BasePerformanceTest();
    test -> world -> aoi = new NTreeAOIService(test -> world -> width, test -> world -> length, 2250, 10);
    test -> test(i + 1);
    delete test -> world -> aoi;
    delete test;
    
    return 0;
}