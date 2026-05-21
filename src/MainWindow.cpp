#include "MainWindow.h"

MainWindow::MainWindow(Database *db, const User &user, QWidget *parent)
    : QMainWindow(parent), db(db), currentUser(user) {}

void MainWindow::onSaveMeasurement()  {}
void MainWindow::onExportCSV()        {}
void MainWindow::onRefreshHistory()   {}
void MainWindow::onCalculateStats()   {}
void MainWindow::setupRegisterTab()   {}
void MainWindow::setupHistoryTab()    {}
void MainWindow::setupStatisticsTab() {}
void MainWindow::setupExportTab()     {}
