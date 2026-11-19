#include "add_task.hpp"
#include "data.hpp"
#include "row_worker.hpp"

#include <memory>

void add_task::exe(TaskContext &ctt)
{
    Wt::Dbo::Transaction t(ctt.session());

    auto item = std::make_unique<data>();
    item->setText(ctt.text());
    item->setComple(false);
    item->setcreate(Wt::WDateTime::currentDateTime());
    item->setdeadline(ctt.deadline());

    dataptr ptr = ctt.session().add(std::move(item));
    t.commit();

    row_worker::append(ctt.model(), ptr);
}
