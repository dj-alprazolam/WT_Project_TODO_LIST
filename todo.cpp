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

    auto model = std::make_shared<Wt::WStandardItemModel>(0, 2);
    model->setHeaderData(0, Wt::Orientation::Horizontal, std::string("Добавить"));
    model->setHeaderData(1, Wt::Orientation::Horizontal, std::string("Задача"));

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
    table->setColumnWidth(0, Wt::WLength(40));
    table->setColumnWidth(1, Wt::WLength(400));
    table->setHeaderHeight(30);
    table->setRowHeight(30);

    loaddata(model.get());

    auto* inputRow = root->addWidget(std::make_unique<Wt::WContainerWidget>());
    inputRow->setStyleClass("todo-input");
    auto* inputLayout = inputRow->setLayout(std::make_unique<Wt::WHBoxLayout>());

    auto* lineEdit = inputLayout->addWidget(std::make_unique<Wt::WLineEdit>());
    lineEdit->setPlaceholderText("Введите новую задачу");
    lineEdit->setWidth(400);

    auto* addBtn = inputLayout->addWidget(std::make_unique<Wt::WPushButton>("Добавить"));
    addBtn->addStyleClass("add");

    auto addHandler = [this, lineEdit, model]() {
    std::string text = lineEdit->text().toUTF8();
    if (text.empty()) return;
    addtask(text, model.get());
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
        }
}

void todo::addtask(const std::string& text, Wt::WStandardItemModel* model){
    Wt::Dbo::Transaction t(session_);

    auto item = std::make_unique<data>();
    item->setText(text);
    item->setComple(false);


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
}


void todo::updateTask(long long id, const std::string& text){
    Wt::Dbo::Transaction t(session_);
    Wt::Dbo::ptr<data> item = session_.load<data>(id);
    if(item) item.modify() -> setText(text);
    t.commit();
}