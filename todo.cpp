#include"todo.hpp"
#include"data.hpp"
#include<Wt/WBootstrapTheme.h>
#include<Wt/WContainerWidget.h>
#include<Wt/WPushButton.h>
#include<Wt/WTableView.h>
#include<Wt/WText.h>
#include<Wt/WHBoxLayout.h>
#include<Wt/WLength.h>
#include<Wt/WStandardItemModel.h>
#include<Wt/WLineEdit.h>
#include<WT/Dbo/backend/Sqlite3.h>
#include<Wt/WStandardItem.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WDate.h>
#include <Wt/WTime.h>
#include<iostream>
todo::todo(const Wt::WEnvironment& env) : Wt::WApplication(env),
    sqlite3_(std::make_unique<Wt::Dbo::backend::Sqlite3>("task.db")),
    session_() 
{

    session_.setConnection(std::move(sqlite3_));
    session_.mapClass<data>("data");

    try
    {
        session_.createTables();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    setTitle("TODO LIST");
    setTheme(std::make_shared<Wt::WBootstrapTheme>());
    useStyleSheet("todo.css");     

    auto* root = this->root();
    root->setStyleClass("todo-app");
    root->setPadding(20);

    auto* title = root->addWidget(std::make_unique<Wt::WText>("Список задач"));
    title->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    title->setStyleClass("todo-title");

    auto model = std::make_shared<Wt::WStandardItemModel>(0, 4);
    model->setHeaderData(0, Wt::Orientation::Horizontal, std::string("Добавить"));
    model->setHeaderData(1, Wt::Orientation::Horizontal, std::string("Задача"));
    model->setHeaderData(2, Wt::Orientation::Horizontal, std::string("Созадана"));
    model->setHeaderData(3, Wt::Orientation::Horizontal, std::string("Дедлайн"));

    auto* table = root->addWidget(std::make_unique<Wt::WTableView>());
    table->addStyleClass("todo-table");
    table->setModel(model);
    model->itemChanged().connect([this](Wt::WStandardItem *item)
                                 {
    if (item->column() == 1) {

        auto* model = dynamic_cast<Wt::WStandardItemModel*>(item->model());
        if (!model) return;

        auto* idItem = model->item(item->row(), 0);
        if (!idItem) return;

        long long id = std::any_cast<long long>(
            idItem->data(Wt::ItemDataRole::User));
        if (id == 0) return;

        std::string newText = item->text().toUTF8();
        if (newText.empty()) return;

        updateTask(id, newText);
    } });
    table->setWidth(Wt::WLength(750));
    table->setHeight(Wt::WLength(400));
    table->setColumnWidth(0, Wt::WLength(40));   
    table->setColumnWidth(1, Wt::WLength(430)); 
    table->setColumnWidth(2, Wt::WLength(140));  
    table->setColumnWidth(3, Wt::WLength(140)); 
    table->setColumnWidth(4, Wt::WLength::Auto);
    table->setHeaderHeight(Wt::WLength(40)); 
    table->setRowHeight(Wt::WLength(35));

    loaddata(model.get());

    auto* inputRow = root->addWidget(std::make_unique<Wt::WContainerWidget>());
    inputRow->setStyleClass("todo-input");
    inputRow->setMargin(Wt::WLength(20), Wt::Side::Top);
    auto* inputLayout = inputRow->setLayout(std::make_unique<Wt::WHBoxLayout>());

    auto* dataEdit = inputLayout->addWidget(std::make_unique<Wt::WDateEdit>());
    dataEdit->setDate(Wt::WDate::currentDate());
    dataEdit->setFormat("dd.MM.yyyy");

    auto *timeEdit = inputLayout->addWidget(std::make_unique<Wt::WTimeEdit>());
    timeEdit->setTime(Wt::WTime::currentTime());
    timeEdit->setFormat("HH:mm");

    auto* lineEdit = inputLayout->addWidget(std::make_unique<Wt::WLineEdit>());
    lineEdit->setPlaceholderText("Введите новую задачу");
    lineEdit->addStyleClass("task-input"); 
    inputLayout->setStretchFactor(lineEdit, 1);

    auto* addBtn = inputLayout->addWidget(std::make_unique<Wt::WPushButton>("Добавить"));
    addBtn->addStyleClass("add");
    addBtn->addStyleClass("btn btn-primary ms-2"); 

    auto addHandler = [this, lineEdit, timeEdit, dataEdit, model]()
    {
        std::string text = lineEdit->text().toUTF8();
        if (text.empty())
            return;
        Wt::WDateTime deadline(dataEdit->date(), timeEdit->time());
        addtask(text, model.get(), deadline);
        lineEdit->setText("");
        lineEdit->setFocus();
    };

    addBtn->clicked().connect(addHandler);
    lineEdit->enterPressed().connect(addHandler);

}

void todo::loaddata(Wt::WStandardItemModel* model){
        model->removeRows(0,model->rowCount());
        Wt::Dbo::Transaction t(session_);
        auto items = session_.find<data>().orderBy("id").resultList();
        for (const auto& item : items)
        {  int row = model->rowCount();
           model ->insertRow(row);
            
        

        auto checkitem = std::make_unique<Wt::WStandardItem>();
        checkitem->setCheckable(true);
        checkitem->setCheckState(item->getComple() ? Wt::CheckState::Checked 
                                                  : Wt::CheckState::Unchecked);
        checkitem->setData(item.id(), Wt::ItemDataRole::User);
        model->setItem(row, 0, std::move(checkitem));

        auto textitem = std::make_unique<Wt::WStandardItem>(item->getText());
        textitem->setEditable(true);
        model->setItem(row, 1, std::move(textitem));

        auto createItem = std::make_unique<Wt::WStandardItem>(item->getcreate().toString("dd.MM.yyyy HH:mm"));
        createItem->setEditable(false);
        model->setItem(row, 2, std::move(createItem));

        std::string deadlinestr = item->getdeadline().isValid() ? item->getdeadline().toString("dd.MM.yyyy HH:mm").toUTF8() : "-";

        auto deadlineItem = std::make_unique<Wt::WStandardItem>(deadlinestr);
        deadlineItem->setEditable(false);
        model->setItem(row, 3, std::move(deadlineItem));
        }
}

void todo::addtask(const std::string& text, Wt::WStandardItemModel* model, const Wt::WDateTime& deadline){
    Wt::Dbo::Transaction t(session_);

    auto item = std::make_unique<data>();
    item->setText(text);
    item->setComple(false);
    item->setcreate(Wt::WDateTime::currentDateTime());
    item->setdeadline(deadline);


    Wt::Dbo::ptr<data> ptr = session_.add(std::move(item));

    t.commit();

    int row = model->rowCount();
    model->insertRow(row);

    auto checkitem = std::make_unique<Wt::WStandardItem>();
    checkitem->setCheckable(true);
    checkitem->setCheckState(Wt::CheckState::Unchecked);
    checkitem->setData(ptr.id(),Wt::ItemDataRole::User);
    model->setItem(row,0,std::move(checkitem));

    auto textitem = std::make_unique<Wt::WStandardItem>(text);
    textitem->setEditable(true);
    model->setItem(row,1,std::move(textitem));

    auto createItem = std::make_unique<Wt::WStandardItem>(ptr->getcreate().toString("dd.MM.yyyy HH:mm"));
    createItem->setEditable(false);
    model->setItem(row,2,std::move(createItem));

    std::string deadlinestr = ptr->getdeadline().isValid() ? ptr->getdeadline().toString("dd.MM.yyyy HH:mm").toUTF8(): "-";

    auto deadlineItem = std::make_unique<Wt::WStandardItem>(deadlinestr);
    deadlineItem->setEditable(false);
    model->setItem(row, 3, std::move(deadlineItem));
}


void todo::updateTask(long long id, const std::string& text){
    Wt::Dbo::Transaction t(session_);
    Wt::Dbo::ptr<data> item = session_.load<data>(id);
    if(item) item.modify() -> setText(text);
    t.commit();
}