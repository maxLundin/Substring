#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
    class string_input;
}

class string_input : public QDialog {
Q_OBJECT

public:
    explicit string_input(QWidget *parent = nullptr);

    ~string_input();

signals:

    void return_string(std::string *);


public slots:

    void accept();

    void reject();

    void enter_pressed(std::string *);


private:
    std::unique_ptr<Ui::string_input> ui;
};
