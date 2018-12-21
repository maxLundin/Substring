//
// Created by max on 12/14/18.
//

#pragma once

#include <QObject>
#include <QDir>
#include <QList>

class file_digger : public QObject {
Q_OBJECT

public:
    file_digger(QDir, bool *, bool *);

    std::vector<QFileInfo> getFiles();

    QList<QString> getDirs();

public slots:

    void PrescanDirectory();

signals:

    void finished();

    void is_not_binary(QFileInfo);

    void progress_bar_files_number(size_t);

    void file_checked();

public:
    ~file_digger() override = default;

private:
    void recursive_scan();

    void scan_trigramms(std::vector<QFileInfo> &);

private:
    QDir dir;
    std::vector<QFileInfo> list_of_files;
    QList<QString> list_of_dirs;
    bool *stop;
    bool *is_running;
};
