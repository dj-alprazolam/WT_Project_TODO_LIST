#pragma once
#include"operation.hpp"

class completed_task : public TaskOperations
{
public:
    void exe(TaskContext& ctt) override;
};
