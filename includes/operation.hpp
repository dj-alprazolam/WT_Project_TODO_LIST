#pragma once 
#include<Wt/Dbo/Dbo.h>
#include <Wt/WStandardItemModel.h>
#include<string>
#include<Wt/WDateTime.h>
class TaskContext
{
public:
    TaskContext(Wt::Dbo::Session &session, Wt::WStandardItemModel &model)
        : session_(session), model_(model) {}

    Wt::Dbo::Session &session() const { return session_; }
    Wt::WStandardItemModel &model() const { return model_; }

    long long id() const { return id_; }
    int row() const { return row_; }
    const std::string &text() const { return text_; }
    const Wt::WDateTime &deadline() const { return deadline_; }
    bool comple() const { return comple_; }

    TaskContext &setId(long long id)
    {
        id_ = id;
        return *this;
    }
    TaskContext &setRow(int row)
    {
        row_ = row;
        return *this;
    }
    TaskContext &setText(const std::string &text)
    {
        text_ = text;
        return *this;
    }
    TaskContext &setDeadline(const Wt::WDateTime &deadline)
    {
        deadline_ = deadline;
        return *this;
    }
    TaskContext &setDone(bool comple)
    {
        comple_ = comple;
        return *this;
    }

private:
    Wt::Dbo::Session &session_;
    Wt::WStandardItemModel &model_;
    long long id_ = 0;
    int row_ = -1;
    std::string text_;
    Wt::WDateTime deadline_;
    bool comple_ = false;
};

class TaskOperations{
public:
    virtual~TaskOperations() = default;
    virtual void exe(TaskContext& ctt) = 0;
};

 