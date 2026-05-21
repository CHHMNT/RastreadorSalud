/**
 * @file MainWindow.h
 * @brief Main application window with all feature tabs.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include "Database.h"
#include "User.h"
#include "Statistics.h"

/**
 * @class MainWindow
 * @brief Central window containing Register, History, Statistics, and Export tabs.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Database *db, const User &user, QWidget *parent = nullptr);

private slots:
    void onSaveMeasurement();
    void onExportCSV();
    void onRefreshHistory();
    void onCalculateStats();

private:
    void setupRegisterTab();
    void setupHistoryTab();
    void setupStatisticsTab();
    void setupExportTab();

    Database    *db;        ///< Shared database instance
    User         currentUser; ///< Currently logged-in user
    Statistics   stats;     ///< Statistics calculator

    QTabWidget  *tabWidget;

    // Register tab widgets
    QLineEdit   *weightEdit;
    QLineEdit   *systolicEdit;
    QLineEdit   *diastolicEdit;
    QLineEdit   *glucoseEdit;
    QPushButton *saveButton;

    // History tab widgets
    QTableWidget *historyTable;

    // Statistics tab widgets
    QLabel      *avgWeightLabel;
    QLabel      *avgGlucoseLabel;
    QLabel      *avgBPLabel;
    QLabel      *bmiLabel;
    QLabel      *kcalLabel;
    QLabel      *trendLabel;

    // Export tab widgets
    QPushButton *exportButton;
    QLabel      *exportStatusLabel;
};

#endif