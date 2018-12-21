//
// Created by max on 12/15/18.
//

#include "file_substring.h"
#include "debug_cout.h"
#include <utility>
#include <fstream>
#include <QtWidgets/QTreeWidgetItem>
#include <iostream>

FileSubstring::FileSubstring(std::vector<QFileInfo> *files) : notbinary_files(files) {}


int64_t FileSubstring::find_substring(std::string filename, std::string &substring) {
    std::ifstream ifstream(filename);
    char *buffer = new char[std::max(buffer_length, substring.length() << 2)];
    std::string my_string;
    while (!ifstream.eof()) {
        ifstream.read(buffer, buffer_length);
        my_string = std::string(buffer, static_cast<unsigned long>(ifstream.gcount()));
        auto pos = my_string.find(substring);
        if (pos != std::string::npos) {
            delete[] buffer;
            return pos;
        }
    }
    delete[] buffer;
    return -1;
}

void FileSubstring::do_search(std::string &input_string) {
    std::cout << input_string << std::endl;
    for (auto &notbinary_file : *notbinary_files) {
        debug_cout(notbinary_file.absoluteFilePath().toStdString(), '\n');
        auto position = find_substring(notbinary_file.absoluteFilePath().toStdString(), input_string);
        debug_cout(position, '\n');
        if (position != -1) {
            auto *item = new QTreeWidgetItem();
            item->setText(0, notbinary_file.absoluteFilePath());
            emit add_to_ui(item);
        }
        emit update();
    }
    emit finished();
}
