//
//  EuDistanceAOITest.cpp
//  AOIs
//
//  Created by zklgame on 5/3/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#include "BasePerformanceTest.hpp"
#include "../../AOIServices/EuDistanceAOIService.hpp"

int main() {
    BasePerformanceTest *test = new BasePerformanceTest();
    
    test -> world -> aoi = new EuDistanceAOIService();
    
    // here it the test
    test -> test(1);
    // end test
    
    delete test;
    
    return 0;
}