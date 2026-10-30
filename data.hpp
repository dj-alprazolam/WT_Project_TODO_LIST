#pragma once 
#include<string>
#include<Wt/Dbo/Dbo.h>
#include<Wt/WDateTime.h>
#include <Wt/Dbo/WtSqlTraits.h>


class data
{
private:
    std::string text;
    bool comple = false;
    Wt::WDateTime create;
    Wt::WDateTime deadline;


public:

    template<class Action>
    void persist(Action& a){
        Wt::Dbo::field(a, text, "text");
        Wt::Dbo::field(a,comple,"comple");
        Wt::Dbo::field(a,create,"create");
        Wt::Dbo::field(a,deadline,"deadline");
    }

    void setText(const std::string& t){
        text = t;
    }
    const std::string& getText() const{
        return text;
    }

    void setComple(bool c){
        comple = c;
    }

    bool getComple() const{
        return comple;
    }

    void setcreate(const Wt::WDateTime& dt){
        create = dt;
    }

    const Wt::WDateTime getcreate() const{
        return create;
    } 

    void setdeadline(const Wt::WDateTime& dt){
        deadline = dt;
    }

    const Wt::WDateTime getdeadline() const{
        return deadline;
    } 
};
 
using dataptr = Wt::Dbo::ptr<data>;



