#include "mainwindow.h"

#include <nlohmann/json.hpp>

#include <QApplication>

#include <algorithm>
#include <random>

int main(int argc, char *argv[]) {
    // test_json();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
