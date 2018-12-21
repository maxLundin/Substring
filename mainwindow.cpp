#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file_substring.h"
#include "trigram_counter.h"

#include <memory>
#include <thread>
#include <iostream>
#include <thread>
#include <regex>
#include <QCommonStyle>
#include <QThread>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDirIterator>

MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        input_string(nullptr),
        notbinary_files(),
        thread(nullptr),
        ready(new bool[3]) {
    ui->setupUi(this);
    arguments.resize(3);

    setGeometry(60, 100, 600, 600);

    QCommonStyle style;

    debug_cout("You are in Debug)\n");

    qRegisterMetaType<QFileInfo>("QFileInfo");
    qRegisterMetaType<size_t>("size_t");

    ui->actionScan_Directory->setIcon(style.standardIcon(QCommonStyle::SP_DialogOpenButton));
    ui->actionStopScanning->setIcon(style.standardIcon(QCommonStyle::SP_BrowserStop));
    ui->actionStopScanning->setVisible(false);

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->headerItem()->setText(0, "File Path");

    ui->statusBar->addPermanentWidget(ui->label);
    ui->statusBar->addPermanentWidget(ui->progressBar);

    ui->label->setText("");
    ui->progressBar->setVisible(false);

    ui->toolBar->addWidget(ui->textEdit);
    connect(ui->textEdit, &text_input::enter_pressed, [this](std::string *input) {
        ui->treeWidget->clear();
        this->receive_string(input);
    });


    std::fill(ready, ready + 3, false);

    for (int i = 1; i < argc; i++) {
        arguments[i - 1] = argv[i];
        debug_cout(argv[i], '\n');
    }

    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, [](QTreeWidgetItem *button) {
        QString fileName = button->text(0);
        QFileInfo dir(fileName);
        if (dir.isFile()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
        }
    });

    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::update_dir);
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::update_file);

    switch (argc) {
        case 1:
            connect(ui->actionScan_Directory, &QAction::triggered, this, &MainWindow::select_directory);
            break;
        case 2:
            prescanDirectory();
            get_string();
        default:
            prescanDirectory();
            break;

    }
}


void MainWindow::receive_string(std::string *input_string) {
    arguments[1] = *input_string;
    debug_cout(arguments[1], '\n');
    delete input_string;
    work_finished_scanning();
}


void MainWindow::get_string() {
    ui->treeWidget->clear();
    input_string = std::make_unique<string_input>();
    connect(input_string.get(), SIGNAL(return_string(std::string * )), this, SLOT(receive_string(std::string * )));
    input_string->setModal(true);
    input_string->exec();

}

void MainWindow::select_directory() {
    ui->treeWidget->clear();
    watcher.removePaths(watcher.directories());
    std::fill(ready, ready + 2, false);
    notbinary_files.clear();
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(),
                                                    QFileDialog::DontResolveSymlinks);
    QFileInfo check_file(dir);
    if (check_file.exists()) {
        arguments[0] = dir.toStdString();
        prescanDirectory();
        get_string();
    }
}

void MainWindow::prescanDirectory() {
    QDir working_directory(arguments[0].data());
    ui->actionStopScanning->setVisible(true);

    stop_scanning();
    ready[1] = false;

    thread = new QThread(nullptr);
    auto *digger = new file_digger(working_directory, ready + 1, ready + 2);
    digger->moveToThread(thread);

    connect(thread, SIGNAL(started()), digger, SLOT(PrescanDirectory()));
    connect(digger, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, &QThread::finished, this, std::bind(&MainWindow::finish_work, this, digger));
    connect(digger, SIGNAL (is_not_binary(QFileInfo)), this, SLOT (add_to_list(QFileInfo)));
    connect(digger, &file_digger::progress_bar_files_number, this, &MainWindow::set_max_range_progress_bar);
    connect(digger, &file_digger::file_checked, this, &MainWindow::inc_progress_bar);
    connect(ui->actionStopScanning, &QAction::triggered, this, &MainWindow::stop_scanning);


    thread->start();

}

void MainWindow::stop_scanning() {
    if (thread != nullptr) {
        ready[1] = true;
        while (ready[2]) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        thread->quit();
        thread->deleteLater();
        thread = nullptr;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    stop_scanning();
    event->accept();
}

void MainWindow::add_to_list(QFileInfo fileInfo) {
    notbinary_files.push_back(fileInfo);
    watcher.addPath(fileInfo.absoluteFilePath());
}

void MainWindow::finish_work(file_digger *digger) {
    ready[1] = false;
    ui->label->setText("Done");
    work_finished_scanning();
    ui->actionStopScanning->setVisible(false);
    auto files = digger->getFiles();
    auto dirs = digger->getDirs();
    digger->deleteLater();
    debug_cout(files.size(), '\n');
    watcher.addPath(QString(arguments[0].c_str()));
    debug_cout(dirs.size(), '\n');
    watcher.addPaths(dirs);
}

void MainWindow::work_finished_scanning() {
    if ((ready)[0]) {
        start_scanning();
    } else {
        (ready)[0] = true;
    }
}

void MainWindow::start_scanning() {
    std::cout << "Scanning started" << std::endl;

    set_max_range_progress_bar(notbinary_files.size());

    auto *thread = new QThread(nullptr);
    auto *digger = new FileSubstring(&notbinary_files);

    for (const auto &x : notbinary_files) {
        debug_cout(x.absoluteFilePath().toStdString(), '\n');
    }
    digger->moveToThread(thread);


    connect(thread, &QThread::started, std::bind(&FileSubstring::do_search, digger, arguments[1]));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(digger, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), digger, SLOT (deleteLater()));
    connect(digger, &FileSubstring::update, this, &MainWindow::inc_progress_bar);
    connect(thread, SIGNAL(finished()), this, SLOT(substring_scan_finished()));
    connect(digger, &FileSubstring::add_to_ui, this, &MainWindow::add_to_ui);

    thread->start();

    ui->label->setText("Doing things");
}

void MainWindow::add_to_ui(QTreeWidgetItem *item) {
    ui->treeWidget->addTopLevelItem(item);
}

void MainWindow::substring_scan_finished() {
    ui->label->setText("Done");
    ui->actionStopScanning->setVisible(false);
}

void MainWindow::inc_progress_bar() {
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

void MainWindow::set_max_range_progress_bar(size_t size) {
    ui->progressBar->setVisible(true);
    ui->progressBar->setRange(0, static_cast<int>(size));
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow() {
    delete thread;
    delete[] ready;
}


void MainWindow::update_dir(const QString &directory) {
    debug_cout("DirChanged ", directory.toStdString(), '\n');
    QDir dir(directory);
    if (dir.exists()) {
        QDirIterator iterator(dir.absolutePath(),
                              QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::Readable | QDir::NoDotAndDotDot);
        auto watchList = watcher.directories();
        while (iterator.hasNext()) {
            QFileInfo file = (iterator.next());
            if (file.isDir()) {
                auto iter = std::find(watchList.begin(), watchList.end(), file.absoluteFilePath());
                if (iter == watchList.end()) {
                    watcher.addPath(file.absoluteFilePath());
                }
                continue;
            }
            if (!trigram_counter::is_binary(file)) {
                auto iter = std::find(notbinary_files.begin(), notbinary_files.end(), file);
                if (iter == notbinary_files.end()) {
                    notbinary_files.push_back(file);
                }
            }
        }
    }
}

void MainWindow::update_file(const QString &filename) {
    debug_cout("FileChanged ", filename.toStdString(), '\n');
    QFileInfo file(filename);
    if (!file.exists()) {
        notbinary_files.erase(std::find(notbinary_files.begin(), notbinary_files.end(), file));
    } else {
        if (trigram_counter::is_binary(file)) {
            notbinary_files.erase(std::find(notbinary_files.begin(), notbinary_files.end(), file));
        }
    }
}


