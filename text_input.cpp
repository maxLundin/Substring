//
// Created by max on 12/18/18.
//

#include "text_input.h"

void text_input::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        emit enter_pressed(new std::string(getString()));
    } else {
        QTextEdit::keyPressEvent(event);
    }
}

text_input::text_input(QWidget *parent) : QTextEdit(parent) {}

std::string text_input::getString() {
    return this->toPlainText().toStdString();
}
