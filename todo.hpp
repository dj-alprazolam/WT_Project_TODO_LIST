#pragma once
#include <Wt/WApplication.h>
#include<Wt/Dbo/backend/Sqlite3.h>
#include<Wt/Dbo/Dbo.h>        
#include<memory>

class data;

class todo : public Wt::WApplication
{
public:
     explicit todo(const Wt::WEnvironment& env);
     Wt::Dbo::Session& session(){
          return session_;
     }
private:
     std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3_;
     Wt::Dbo::Session session_;
     void loaddata(Wt::WStandardItemModel* model);
     void addtask(const std::string& text, Wt::WStandardItemModel* model);

};

