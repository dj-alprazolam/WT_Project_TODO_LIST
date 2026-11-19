#include"completed_task.hpp"
#include"data.hpp"

void completed_task(TaskContext& ctt){
    Wt::Dbo::Transaction t(ctt.session());

    dataptr item = ctt.session().load<data>(ctt.id());
    if (item)
        item.modify()->setComple(ctt.comple());

    t.commit();
}