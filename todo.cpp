#include"todo.hpp"
#include<Wt/WBootstrapTheme.h>
#include<Wt/WContainerWidget.h>
#include<Wt/WPushButton.h>
#include<Wt/WTableView.h>
#include<Wt/WText.h>
#include<Wt/WHBoxLayout.h>
#include<Wt/WLength.h>
#include<Wt/WStandardItemModel.h>
#include<Wt/WLineEdit.h>

todo::todo(const Wt::WEnvironment& env) : Wt::WApplication(env){
    setTitle("TODO LIST");
    setTheme(std::make_shared<Wt::WBootstrapTheme>());

    auto* root = this->root();
    root->setStyleClass("container");
    root->setPadding(20);

    auto* title = root->addWidget(std::make_unique<Wt::WText>("Список задач"));
    title->setTextFormat(Wt::TextFormat::UnsafeXHTML);

    auto model = std::make_shared<Wt::WStandardItemModel>(0, 2);
    model->setHeaderData(0, Wt::Orientation::Horizontal, std::string("Добавить"));
    model->setHeaderData(1, Wt::Orientation::Horizontal, std::string("Задача"));

    auto* table = root->addWidget(std::make_unique<Wt::WTableView>());
    table->setModel(model);
    table->setColumnWidth(0, Wt::WLength(40));
    table->setColumnWidth(1, Wt::WLength(400));
    table->setHeaderHeight(30);
    table->setRowHeight(30);

    auto* inputRow = root->addWidget(std::make_unique<Wt::WContainerWidget>());
    auto* inputLayout = inputRow->setLayout(std::make_unique<Wt::WHBoxLayout>());

    auto* lineEdit = inputLayout->addWidget(std::make_unique<Wt::WLineEdit>());
    lineEdit->setPlaceholderText("Введите новую задачу");
    lineEdit->setWidth(400);

    auto* addBtn = inputLayout->addWidget(std::make_unique<Wt::WPushButton>("Добавить"));
    addBtn->addStyleClass("add");

}
