#pragma once 
#include<string>
#include<Wt/Dbo/Dbo.h>


class data
{
public:
    std::string text;
    bool comple = false;

    template<class Action>
    void persist(Action& a){
        Wt::Dbo::field(a, text, "text");
        Wt::Dbo::field(a,comple,"comple");
    }

};
 
using dataptr = Wt::Dbo::ptr<data>;



