#include "mainwindow.h"

#include <nlohmann/json.hpp>

#include <QApplication>

#include <algorithm>
#include <random>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyle("fusion");
    MainWindow w;
    w.show();
    return a.exec();
}
