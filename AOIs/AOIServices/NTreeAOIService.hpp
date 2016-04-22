//
//  NTreeAOIService.hpp
//  AOIs
//
//  Created by zklgame on 4/21/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef NTreeAOIService_hpp
#define NTreeAOIService_hpp

#include "AOIService.hpp"
#include "../AOIModels/NTreeAOI/NTreeAOI.hpp"

class NTreeAOIService: public AOIService {
public:
    NTreeAOIService(position_t worldWidth, position_t worldHeight, entity_t towerMaxPublisherNum, position_t maxLevel);
    virtual ~NTreeAOIService();
};
#endif /* NTreeAOIService_hpp */
