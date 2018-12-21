//
// Created by max on 12/15/18.
//

#pragma once


#include <QObject>
#include <QFileInfo>
#include <QTreeWidgetItem>

const size_t buffer_length = 128000;

class FileSubstring : public QObject {
Q_OBJECT

public:
    explicit FileSubstring(std::vector<QFileInfo> *notbinary_files);

    int64_t find_substring(std::string, std::string &);


public slots:

    void do_search(std::string &);

signals:

    void update();

    void add_to_ui(QTreeWidgetItem *);

    void finished();

private:
    std::vector<QFileInfo> *notbinary_files;
};
