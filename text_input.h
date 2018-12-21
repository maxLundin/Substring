//
// Created by max on 12/18/18.
//

#pragma once


#include <QTextEdit>
#include <QKeyEvent>

class text_input : public QTextEdit {
Q_OBJECT
public :
    explicit text_input(QWidget *parent);

    void keyPressEvent(QKeyEvent *event) override;

    std::string getString();

signals:

    void enter_pressed(std::string *);

};

