#pragma once 
#include"operation.hpp"

class add_task : public TaskOperations
{
public:
    void exe(TaskContext &ctt) override;
};

