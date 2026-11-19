#pragma once
#include"operation.hpp"

class LoadData : public TaskOperations
{
public:
    void exe(TaskContext& ctt) override;
};
