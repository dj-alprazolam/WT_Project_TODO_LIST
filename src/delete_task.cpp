#include "delete_task.hpp"

#include "data.hpp"

void delete_task::exe(TaskContext &ctt)
{
    {
        Wt::Dbo::Transaction t(ctt.session());

        dataptr item = ctt.session().load<data>(ctt.id());
        if (item)
            item.remove();

        t.commit();
    }
    ctt.model().removeRow(ctt.row());
}
