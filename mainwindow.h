#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

#include "encryptor.h"

class LoginWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

    using StrCRef = const QString&;

public:
    MainWindow(QWidget *parent = nullptr);
    void Login(StrCRef login, StrCRef password);

    QWidget* main_;

    void keyPressEvent(QKeyEvent* event) override;

private:
    struct Account {
        QString resource;
        QString login;
        QString password;
        Account(StrCRef r, StrCRef l, StrCRef p);
    };

    void ConnectSignals();
    void CreateWidgets();
    void InitTable();
    void SetStyle();
    void ReadUsersPaths();
    void WriteUsersPaths();

    void SetTableCell(int i, int j, StrCRef text);
    void SetTableHeader(int column, StrCRef text);
    void AddAccount(StrCRef resource, StrCRef login, StrCRef password);
    void ShowAccount(int index);

    void Open(StrCRef filename = "");
    void SaveAs(StrCRef filename = "");
    void Save();

    LoginWindow* login_;
    QHBoxLayout* l0_layout_;
    QVBoxLayout* l1_button_layout_;
    QVBoxLayout* l2_adding_layout_;
    QGroupBox* adding_;
    QTableWidget* view_;
    QAbstractItemModel* table_model_;
    QLineEdit* resource_edit_;
    QLineEdit* login_edit_;
    QLineEdit* password_edit_;
    QPushButton* new_;
    QPushButton* open_;
    QPushButton* save_;
    QPushButton* save_as_;

    QVector<Account> accounts_;
    Encryptor encryptor_;
    QString current_save_path_;
    QMap<QString, QString> users_;
};

#endif  // MAINWINDOW_H
