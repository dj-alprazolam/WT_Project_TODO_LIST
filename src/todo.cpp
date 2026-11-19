#include "todo.hpp"
#include "data.hpp"
#include "LoadData.hpp"
#include "add_task.hpp"
#include "update_task.hpp"
#include "delete_task.hpp"
#include "completed_task.hpp"
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WTableView.h>
#include <Wt/WText.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WLength.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WLineEdit.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/WStandardItem.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WDate.h>
#include <Wt/WTime.h>
#include <any>
#include <iostream>
                            todo::todo(const Wt::WEnvironment &env) : Wt::WApplication(env),
                                                                      sqlite3_(std::make_unique<Wt::Dbo::backend::Sqlite3>("task.db")),
                                                                      session_(),
                                                                      loadData_(std::make_unique<LoadData>()),
                                                                      addTask_(std::make_unique<add_task>()),
                                                                      updateTask_(std::make_unique<UpdateTask>()),
                                                                      deleteTask_(std::make_unique<delete_task>()),
                                                                      completedTask_(std::make_unique<completed_task>())
{

    session_.setConnection(std::move(sqlite3_));
    session_.mapClass<data>("data");

    try
    {
        session_.createTables();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    setTitle("TODO LIST");
    setTheme(std::make_shared<Wt::WBootstrapTheme>());
    useStyleSheet("todo.css");

    auto *root = this->root();
    root->setStyleClass("todo-app");
    root->setPadding(20);

    auto *title = root->addWidget(std::make_unique<Wt::WText>("Список задач"));
    title->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    title->setStyleClass("todo-title");

    constexpr int COLS = 5;

    auto model = std::make_shared<Wt::WStandardItemModel>(0, COLS);
    model->setHeaderData(0, Wt::Orientation::Horizontal, std::string("Готово"));
    model->setHeaderData(1, Wt::Orientation::Horizontal, std::string("Задача"));
    model->setHeaderData(2, Wt::Orientation::Horizontal, std::string("Создана"));
    model->setHeaderData(3, Wt::Orientation::Horizontal, std::string("Дедлайн"));
    model->setHeaderData(4, Wt::Orientation::Horizontal, std::string(""));

    auto *table = root->addWidget(std::make_unique<Wt::WTableView>());
    table->addStyleClass("todo-table");
    table->setModel(model);

    model->itemChanged().connect([this](Wt::WStandardItem *item)
                                 {
if (item->column() != 0 && item->column() != 1)
    return;

auto* model = dynamic_cast<Wt::WStandardItemModel*>(item->model());
if (!model) return;

auto* idItem = model->item(item->row(), 0);
if (!idItem) return;

long long id = std::any_cast<long long>(
    idItem->data(Wt::ItemDataRole::User));
if (id == 0) return;

if (item->column() == 1) {
    std::string newText = item->text().toUTF8();
    if (newText.empty()) return;

    updateTask(id, newText);
} else {
    completedTask(id, item->checkState() == Wt::CheckState::Checked);
} });

    auto *rawModel = model.get();
    table->clicked().connect([this, rawModel](const Wt::WModelIndex &index, const Wt::WMouseEvent &)
                             {
                             if (!index.isValid() || index.column() != 4)
                                 return;

                             int row = index.row();
                             auto *idItem = rawModel->item(row, 0);
                             if (!idItem)
                                 return;

                             long long id = std::any_cast<long long>(idItem->data(Wt::ItemDataRole::User));
                             if (id == 0)
                                 return;

                             deleteTask(id, row, rawModel); });

    constexpr int colW[COLS] = {60, 380, 150, 150, 50};
    constexpr int cellPad = 7;
    constexpr int scrollBar = 20;
    int total = 0;
    for (int i = 0; i < COLS; ++i)
    {
        table->setColumnWidth(i, Wt::WLength(colW[i]));
        total += colW[i] + cellPad;
    }
    const int tableWidth = total + scrollBar;

    table->setWidth(Wt::WLength(tableWidth));
    table->setHeight(Wt::WLength(400));
    table->setHeaderHeight(Wt::WLength(40));
    table->setRowHeight(Wt::WLength(35));
    table->setAlternatingRowColors(true);

    loaddata(model.get());
    auto *inputRow = root->addWidget(std::make_unique<Wt::WContainerWidget>());
    inputRow->setStyleClass("todo-input");
    inputRow->setMargin(Wt::WLength(20), Wt::Side::Top);
    inputRow->setWidth(Wt::WLength(tableWidth));
    auto *inputLayout = inputRow->setLayout(std::make_unique<Wt::WHBoxLayout>());
    inputLayout->setContentsMargins(0, 0, 0, 0);

    auto *dataEdit = inputLayout->addWidget(std::make_unique<Wt::WDateEdit>());
    dataEdit->setDate(Wt::WDate::currentDate());
    dataEdit->setFormat("dd.MM.yyyy");
    dataEdit->setWidth(Wt::WLength(120));

    auto *timeEdit = inputLayout->addWidget(std::make_unique<Wt::WTimeEdit>());
    timeEdit->setTime(Wt::WTime::currentTime());
    timeEdit->setFormat("HH:mm");
    timeEdit->setWidth(Wt::WLength(80));

    auto *lineEdit = inputLayout->addWidget(std::make_unique<Wt::WLineEdit>());
    lineEdit->setPlaceholderText("Введите новую задачу");
    lineEdit->addStyleClass("task-input");
    inputLayout->setStretchFactor(lineEdit, 1);

    auto *addBtn = inputLayout->addWidget(std::make_unique<Wt::WPushButton>("Добавить"));
    addBtn->addStyleClass("add");
    addBtn->addStyleClass("btn btn-primary ms-2");

    auto addHandler = [this, lineEdit, timeEdit, dataEdit, rawModel]()
    {
        std::string text = lineEdit->text().toUTF8();
        if (text.empty())
            return;
        Wt::WDateTime deadline(dataEdit->date(), timeEdit->time());
        addtask(text, rawModel, deadline);
        lineEdit->setText("");
        lineEdit->setFocus();
    };

    addBtn->clicked().connect(addHandler);
    lineEdit->enterPressed().connect(addHandler);
}

void todo::loaddata(Wt::WStandardItemModel *model)
{
    TaskContext ctt(session_, *model);
    loadData_->exe(ctt);
}

void todo::addtask(const std::string &text, Wt::WStandardItemModel *model, const Wt::WDateTime &deadline)
{
    TaskContext ctt(session_, *model);
    ctt.setText(text).setDeadline(deadline);
    addTask_->exe(ctt);
}

void todo::updateTask(long long id, const std::string &text)
{
    Wt::WStandardItemModel dummy;
    TaskContext ctt(session_, dummy);
    ctt.setId(id).setText(text);
    updateTask_->exe(ctt);
}

void todo::deleteTask(long long id, int row, Wt::WStandardItemModel *model)
{
    TaskContext ctt(session_, *model);
    ctt.setId(id).setRow(row);
    deleteTask_->exe(ctt);
}

void todo::completedTask(long long id, bool comple)
{
    Wt::WStandardItemModel dummy;
    TaskContext ctt(session_, dummy);
    ctt.setId(id).setDone(comple);
    completedTask_->exe(ctt);
}