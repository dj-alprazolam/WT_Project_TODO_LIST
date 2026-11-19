#include "update_task.hpp"

#include "data.hpp"

void UpdateTask::exe(TaskContext &ctt)
{
    Wt::Dbo::Transaction t(ctt.session());

    dataptr item = ctt.session().load<data>(ctt.id());
    if (item)
        item.modify()->setText(ctt.text());

    t.commit();
}
