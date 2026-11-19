#pragma once 
#include"operation.hpp"

class delete_task : public TaskOperations
{
public:
    void exe(TaskContext& ctt) override;
};
