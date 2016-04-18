//
//  alias.h
//  AOIs
//
//  Created by zklgame on 4/5/16.
//  Copyright © 2016 Zhejiang University. All rights reserved.
//

#ifndef alias_h
#define alias_h

#include <inttypes.h>

typedef uint32_t entity_t;

typedef uint32_t position_t;
typedef uint8_t type_t;

typedef uint8_t state_t;


// type of GameObject
#define PUBLISHER 1
#define SUBSCRIBER 2
#define PUB_SUB 3

// state of GameObject
#define ADD 1
#define REMOVE 2
#define MOVE 3

// type of message
#define ADD_MESSAGE 1
#define REMOVE_MESSAGE 2
#define MOVE_MESSAGE 3

// type of list in XYList
#define LISTX 1
#define LISTY 2

#endif /* alias_h */
