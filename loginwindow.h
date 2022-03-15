#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow;

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(MainWindow *parent);

private:
    void ConnectSignals();
    void Enter();

    QVBoxLayout* layout_;

    QLineEdit* login_;
    QLineEdit* password_;
    QPushButton* enter_;

    MainWindow* parent_;
};

#endif  // LOGINWINDOW_H
