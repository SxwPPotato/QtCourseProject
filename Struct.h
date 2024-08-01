#ifndef STRUCT_H
#define STRUCT_H

#include <stdint.h>

enum Type_message{

    automatic   = 0,
    activated   = 1
};

enum type_req{
    arrival = 0,
    departure = 1,
    statistic = 2,
    empty = 3
};

enum statistic{
    year_stat = 0,
    day_stat = 1

};



#endif // STRUCT_H
