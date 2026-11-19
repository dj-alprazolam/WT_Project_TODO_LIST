#pragma once 
#include "data.hpp"
#include <Wt/WStandardItemModel.h>

class row_worker
{
public:
    static void append(Wt::WStandardItemModel &model, const dataptr &item);
};

