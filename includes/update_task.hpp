#pragma once
#include "operation.hpp"

class UpdateTask : public TaskOperations
{
public:
    void exe(TaskContext &ctt) override;
};

