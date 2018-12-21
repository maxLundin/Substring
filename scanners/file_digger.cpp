//
// Created by max on 12/14/18.
//

#include <trigram/trigram_counter.h>
#include "file_digger.h"
#include <QThread>
#include <QDirIterator>

file_digger::file_digger(QDir directory, bool *stop, bool *runstate) : dir(directory),
                                                                       stop(stop),
                                                                       is_running(runstate) { (*runstate) = true; }

void file_digger::PrescanDirectory() {
    recursive_scan();
    emit finished();
    *is_running = false;
}


void file_digger::recursive_scan() {
    QDirIterator it(dir.absolutePath(),
                    QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::Readable | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo file = (it.next());
        if (file.isDir()) {
            list_of_dirs.push_back(file.absolutePath());
            continue;
        }
        list_of_files.push_back(file);
        if (*stop) {
            return;
        }
    }
    emit progress_bar_files_number(list_of_files.size());

    scan_trigramms(list_of_files);
}

void file_digger::scan_trigramms(std::vector<QFileInfo> &files) {
    for (QFileInfo &file : files) {
        if (*stop) {
            return;
        }
        if (!trigram_counter::is_binary(file)) {
            emit is_not_binary(file);
        }
        emit file_checked();
//        if (stop_scanning){
//
//        }
    }
}

std::vector<QFileInfo> file_digger::getFiles() {
    return list_of_files;
}

QList<QString> file_digger::getDirs() {
    return list_of_dirs;
}



