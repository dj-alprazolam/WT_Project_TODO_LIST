#include"LoadData.hpp"
#include"data.hpp"
#include"operation.hpp"
#include"row_worker.hpp"

void LoadData::exe(TaskContext &ctt)
{
    ctt.model().removeRows(0, ctt.model().rowCount());

    Wt::Dbo::Transaction t(ctt.session());
    auto items = ctt.session().find<data>().orderBy("id").resultList();

    for (const auto &item : items)
        row_worker::append(ctt.model(), item);
}