#pragma once 
#include<string>
#include<Wt/Dbo/Dbo.h>


class data
{
private:
    std::string text;
    bool comple = false;


public:

    template<class Action>
    void persist(Action& a){
        Wt::Dbo::field(a, text, "text");
        Wt::Dbo::field(a,comple,"comple");
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
};
 
using dataptr = Wt::Dbo::ptr<data>;



