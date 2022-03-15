#include "mainwindow.h"
#include "loginwindow.h"

#include <QDataStream>
#include <QFileDialog>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMessageBox>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    CreateWidgets();
    ConnectSignals();
    SetStyle();
    ReadUsersPaths();

    setWindowTitle("Password Handler");
    setMinimumSize(666, 400);
    login_ = new LoginWindow(this);
    login_->show();
}

void MainWindow::Login(StrCRef login, StrCRef password) {
    if (login.isEmpty()) {
        throw "Please enter a valid login";
    }
    if (QRegularExpression("[a-zA-Z0-9_\\-]+").match(login)
            .capturedLength() != login.length()) {
        throw "Login must contain only latin letters, digits, \"-\" and \"_\"";
    }
    if (password.length() < 5) {
        throw "Password length must be at least 5";
    }
    if (users_.contains(login)) {
        current_save_path_ = users_[login];
    } else {
        users_[login] = current_save_path_ = QString("packs/") + login + ".pw-pack";
        WriteUsersPaths();
    }
    encryptor_.Init(password);
    Open(current_save_path_);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        if (view_->hasFocus() && view_->currentItem() != nullptr) {
            int i = view_->currentRow();
            accounts_.erase(accounts_.begin() + i);
            view_->removeRow(i);
        }
        break;
    }
}

void MainWindow::ConnectSignals() {
    connect(new_, &QPushButton::pressed, this, [&] {
        AddAccount(resource_edit_->text(), login_edit_->text(), password_edit_->text());
        for (auto i : {resource_edit_, login_edit_, password_edit_}) {
            i->clear();
        }
    });
    connect(view_, &QTableWidget::cellChanged, this, [&] (int i, int j) {
        auto& acc = accounts_[i];
        *QList{&acc.resource, &acc.login, &acc.password}[j] =
            view_->item(i, j)->text();
    });
    connect(save_, &QPushButton::pressed, this, &MainWindow::Save);
    connect(save_as_, &QPushButton::pressed, this, [&]{SaveAs();});
    connect(open_, &QPushButton::pressed, this, [&]{Open();});
}

void MainWindow::CreateWidgets() {
    setCentralWidget(main_ = new QWidget);
    main_->setLayout(l0_layout_ = new QHBoxLayout);
    l0_layout_->addWidget(view_ = new QTableWidget);
    l0_layout_->addLayout(l1_button_layout_ = new QVBoxLayout);

    l1_button_layout_->addWidget(adding_ = new QGroupBox("New account"));
    adding_->setLayout(l2_adding_layout_ = new QVBoxLayout);
    l2_adding_layout_->addWidget(resource_edit_ = new QLineEdit);
    l2_adding_layout_->addWidget(login_edit_ = new QLineEdit);
    l2_adding_layout_->addWidget(password_edit_ = new QLineEdit);
    l2_adding_layout_->addWidget(new_ = new QPushButton("Add"));
    resource_edit_->setPlaceholderText("Resource");
    login_edit_->setPlaceholderText("Login");
    password_edit_->setPlaceholderText("Password");

    l1_button_layout_->addWidget(open_ = new QPushButton("Open"));
    l1_button_layout_->addWidget(save_ = new QPushButton("Save"));
    l1_button_layout_->addWidget(save_as_ = new QPushButton("Save as"));

    InitTable();
}

void MainWindow::SetTableCell(int i, int j, StrCRef text) {
    table_model_->setData(table_model_->index(i, j), text);
}

void MainWindow::SetTableHeader(int column, StrCRef text) {
    view_->setHorizontalHeaderItem(column, new QTableWidgetItem(text));
}

void MainWindow::AddAccount(StrCRef resource, StrCRef login, StrCRef password) {
    int index = accounts_.length();
    accounts_.emplace_back(resource, login, password);
    view_->setRowCount(index + 1);
    ShowAccount(index);
}

void MainWindow::ShowAccount(int index) {
    const auto& acc = accounts_[index];
    SetTableCell(index, 0, acc.resource);
    SetTableCell(index, 1, acc.login);
    SetTableCell(index, 2, acc.password);
}

void MainWindow::SaveAs(StrCRef filename) {
    if (filename.isEmpty()) {
        auto f = QFileDialog::getOpenFileName();
        if (f.isEmpty()) return;
        return SaveAs(f);
    }

    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        return;
    }
    QDataStream out(&file);
    out << accounts_.size();
    for (const auto& acc : accounts_) {
        for (const auto& field : {acc.resource, acc.login, acc.password}) {
            encryptor_.Write(out, field);
        }
    }
    file.close();
}

void MainWindow::Save() {
    SaveAs(current_save_path_);
}

void MainWindow::Open(StrCRef filename) {
    if (filename.isEmpty()) {
        auto f = QFileDialog::getOpenFileName();
        if (f.isEmpty()) return;
        return Open(f);
    }
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return;
    }
    QDataStream in(&file);
    qsizetype count;
    in >> count;
    accounts_.clear();
    accounts_.reserve(count);
    for (qsizetype i = 0; i < count; ++i) {
        QString re, lo, pa;
        for (auto field : {&re, &lo, &pa}) {
            *field = encryptor_.Read(in);
        }
        AddAccount(re, lo, pa);
    }
    file.close();
}

void MainWindow::InitTable() {
    table_model_ = view_->model();
    view_->setColumnCount(3);
    SetTableHeader(0, "Resource");
    SetTableHeader(1, "Login");
    SetTableHeader(2, "Password");
}

void MainWindow::SetStyle() {
    setStyleSheet("QTableView:item:selected {background: #c8ffc4; color: #005c03}");
}

void MainWindow::ReadUsersPaths() {
    QFile file("paths.qmap");
    if (file.open(QFile::ReadOnly)) {
        QDataStream stream(&file);
        stream >> users_;
        file.close();
    }
}

void MainWindow::WriteUsersPaths() {
    QFile file("paths.qmap");
    if (file.open(QFile::WriteOnly)) {
        QDataStream stream(&file);
        stream << users_;
        file.close();
    }
}

MainWindow::Account::Account(StrCRef r, StrCRef l, StrCRef p)
    : resource(r), login(l), password(p) {}
