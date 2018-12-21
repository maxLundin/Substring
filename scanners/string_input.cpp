#include "string_input.h"
#include "ui_stringinput.h"
#include "text_input.h"

string_input::string_input(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::string_input) {
    ui->setupUi(this);
    QWidget::setWindowTitle("Choose substring to find");
    ui->textInput->grabKeyboard();
    connect(ui->textInput, &text_input::enter_pressed, this, &string_input::enter_pressed);
}


string_input::~string_input() = default;

void string_input::reject() {
    ui->textInput->releaseKeyboard();
    QDialog::reject();
}

void string_input::accept() {
    ui->textInput->releaseKeyboard();
    emit return_string(new std::string(ui->textInput->getString()));
    QDialog::accept();
}

void string_input::enter_pressed(std::string *text) {
    ui->textInput->releaseKeyboard();
    emit return_string(text);
    QDialog::accept();
}
