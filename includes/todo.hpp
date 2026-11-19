#pragma once
#include <Wt/WApplication.h>
#include <Wt/WDateTime.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Dbo/Dbo.h>
#include <memory>
#include <string>

#include "operation.hpp"

class data;

class todo : public Wt::WApplication
{
public:
     explicit todo(const Wt::WEnvironment &env);
     Wt::Dbo::Session &session()
     {
          return session_;
     }

private:
     std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3_;
     Wt::Dbo::Session session_;

     std::unique_ptr<TaskOperations> loadData_;
     std::unique_ptr<TaskOperations> addTask_;
     std::unique_ptr<TaskOperations> updateTask_;
     std::unique_ptr<TaskOperations> deleteTask_;
     std::unique_ptr<TaskOperations> completedTask_;

     void loaddata(Wt::WStandardItemModel *model);
     void addtask(const std::string &text, Wt::WStandardItemModel *model, const Wt::WDateTime &deadline);
     void updateTask(long long id, const std::string &text);
     void deleteTask(long long id, int row, Wt::WStandardItemModel *model);
     void completedTask(long long id, bool comple);
};