#include "loginwindow.h"
#include "mainwindow.h"

#include <QCalendarWidget>
#include <QIcon>
#include <QMessageBox>

LoginWindow::LoginWindow(MainWindow* parent)
    : parent_(parent) {
    setLayout(layout_ = new QVBoxLayout);

    layout_->addWidget(login_ = new QLineEdit);
    login_->setPlaceholderText("Login");

    layout_->addWidget(password_ = new QLineEdit);
    password_->setPlaceholderText("Password");
    password_->setEchoMode(QLineEdit::Password);

    layout_->addWidget(enter_ = new QPushButton);
    enter_->setText("Enter");

    setMinimumSize(200, minimumHeight());
    setMaximumHeight(minimumHeight());
    setWindowOpacity(.85);
    setWindowTitle("Log in");

    ConnectSignals();
}

void LoginWindow::ConnectSignals() {
    connect(enter_, &QPushButton::pressed, this, &LoginWindow::Enter);
    connect(login_, &QLineEdit::editingFinished, this, [&] {
        password_->setFocus();
    });
    connect(password_, &QLineEdit::editingFinished, this, [&] {
        enter_->setFocus();
    });
}

void LoginWindow::Enter() {
    try {
        parent_->Login(login_->text(), password_->text());
        hide();
        parent_->show();
    } catch (const char* message) {
        auto alert = new QMessageBox;
        alert->setWindowTitle("Failed");
        alert->setText(message);
        alert->setIcon(QMessageBox::Icon::Warning);
        login_->setFocus();
        login_->selectAll();
        alert->show();
        connect(alert, &QMessageBox::accepted, this, [=] {
            delete alert;
        });
    }
}
