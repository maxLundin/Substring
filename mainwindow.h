#pragma once

#include "string_input.h"
#include "file_digger.h"
#include "debug_cout.h"
#include <memory>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QMainWindow>
#include <QTreeWidgetItem>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(int argc, char *argv[], QWidget *parent = nullptr);

    ~MainWindow() override;

signals:

    void prescan_finished();

private slots:

    void closeEvent(QCloseEvent *event) override;

    void add_to_list(QFileInfo);

    void finish_work(file_digger *);

    void start_scanning();

    void work_finished_scanning();

    void get_string();

    void receive_string(std::string *);

    void prescanDirectory();

    void select_directory();

    void add_to_ui(QTreeWidgetItem *);

    void substring_scan_finished();

    void inc_progress_bar();

    void set_max_range_progress_bar(size_t);

    void update_dir(const QString &);

    void update_file(const QString &);

    void stop_scanning();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<string_input> input_string;
    std::vector<QFileInfo> notbinary_files;
    std::vector<std::string> arguments;
    QThread *thread;
    bool *ready;
    QFileSystemWatcher watcher;

};
