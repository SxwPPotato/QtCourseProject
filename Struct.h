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
    empty = 999
};

enum statistic{
    year_stat = 0,
    day_stat = 1

};

enum mounth{
    January = 0,
    February = 1,
    March = 2,
    April = 3,
    May = 4,
    June = 5,
    July = 6,
    August = 7,
    September = 8,
    October = 9,
    November= 10,
    December = 11
};



#endif // STRUCT_H
