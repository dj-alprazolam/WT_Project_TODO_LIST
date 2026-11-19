#include"row_worker.hpp"

#include <Wt/WStandardItem.h>

#include <memory>
#include <string>

void row_worker::append(Wt::WStandardItemModel &model, const dataptr &item)
{
    int row = model.rowCount();
    model.insertRow(row);

    auto checkItem = std::make_unique<Wt::WStandardItem>();
    checkItem->setCheckable(true);
    checkItem->setCheckState(item->getComple() ? Wt::CheckState::Checked
                                               : Wt::CheckState::Unchecked);
    checkItem->setData(item.id(), Wt::ItemDataRole::User);
    model.setItem(row, 0, std::move(checkItem));

    auto textItem = std::make_unique<Wt::WStandardItem>(item->getText());
    textItem->setEditable(true);
    model.setItem(row, 1, std::move(textItem));

    auto createItem = std::make_unique<Wt::WStandardItem>(
        item->getcreate().toString("dd.MM.yyyy HH:mm"));
    createItem->setEditable(false);
    model.setItem(row, 2, std::move(createItem));

    std::string deadlineStr = item->getdeadline().isValid()
                                  ? item->getdeadline().toString("dd.MM.yyyy HH:mm").toUTF8()
                                  : "-";
    auto deadlineItem = std::make_unique<Wt::WStandardItem>(deadlineStr);
    deadlineItem->setEditable(false);
    model.setItem(row, 3, std::move(deadlineItem));

    auto deleteItem = std::make_unique<Wt::WStandardItem>("✖️");
    deleteItem->setEditable(false);
    deleteItem->setStyleClass("todo-del");
    model.setItem(row, 4, std::move(deleteItem));
}
