#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

/**
 * @brief Constructs the MainWindow and sets up all tabs.
 */
MainWindow::MainWindow(Database *db, const User &user, QWidget *parent)
    : QMainWindow(parent), db(db), currentUser(user) {

    setWindowTitle("Rastreador Personal de Salud — " + user.getUsername());
    setMinimumSize(700, 500);

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    setupRegisterTab();
    setupHistoryTab();
    setupStatisticsTab();
    setupExportTab();
}

// ─────────────────────────────────────────────
//  TAB SETUP
// ─────────────────────────────────────────────

/**
 * @brief Sets up the Register tab for entering new measurements.
 */
void MainWindow::setupRegisterTab() {
    QWidget     *tab    = new QWidget();
    QFormLayout *form   = new QFormLayout();
    QVBoxLayout *layout = new QVBoxLayout(tab);

    weightEdit    = new QLineEdit();
    systolicEdit  = new QLineEdit();
    diastolicEdit = new QLineEdit();
    glucoseEdit   = new QLineEdit();
    saveButton    = new QPushButton("Save Measurement");

    weightEdit->setPlaceholderText("e.g. 70.5");
    systolicEdit->setPlaceholderText("e.g. 120");
    diastolicEdit->setPlaceholderText("e.g. 80");
    glucoseEdit->setPlaceholderText("e.g. 95.0");

    form->addRow("Weight (kg):",      weightEdit);
    form->addRow("Systolic (mmHg):",  systolicEdit);
    form->addRow("Diastolic (mmHg):", diastolicEdit);
    form->addRow("Glucose (mg/dL):",  glucoseEdit);

    layout->addLayout(form);
    layout->addWidget(saveButton);
    layout->addStretch();

    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveMeasurement);

    tabWidget->addTab(tab, "Register");
}

/**
 * @brief Sets up the History tab showing past measurements in a table.
 */
void MainWindow::setupHistoryTab() {
    QWidget     *tab    = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);

    historyTable = new QTableWidget();
    historyTable->setColumnCount(6);
    historyTable->setHorizontalHeaderLabels({
        "Date & Time", "Weight (kg)", "Systolic", "Diastolic", "Glucose (mg/dL)", ""
    });
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QPushButton *refreshButton = new QPushButton("Refresh");
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshHistory);

    layout->addWidget(historyTable);
    layout->addWidget(refreshButton);

    tabWidget->addTab(tab, "History");

    onRefreshHistory(); // Load data immediately on open
}

/**
 * @brief Sets up the Statistics tab showing averages, BMI, and kcal.
 */
void MainWindow::setupStatisticsTab() {
    QWidget     *tab    = new QWidget();
    QFormLayout *form   = new QFormLayout();
    QVBoxLayout *layout = new QVBoxLayout(tab);

    avgWeightLabel = new QLabel("—");
    avgGlucoseLabel= new QLabel("—");
    avgBPLabel     = new QLabel("—");
    bmiLabel       = new QLabel("—");
    kcalLabel      = new QLabel("—");
    trendLabel     = new QLabel("—");

    // Height, age, sex, activity inputs for BMI/kcal
    QLineEdit *heightEdit   = new QLineEdit();
    QLineEdit *ageEdit      = new QLineEdit();
    QComboBox *sexCombo     = new QComboBox();
    QComboBox *activityCombo= new QComboBox();
    QComboBox *goalCombo    = new QComboBox();

    heightEdit->setPlaceholderText("e.g. 170");
    ageEdit->setPlaceholderText("e.g. 22");
    sexCombo->addItems({"Male", "Female"});
    activityCombo->addItems({"Sedentary (×1.2)", "Light (×1.375)",
                              "Moderate (×1.55)", "Active (×1.725)",
                              "Very Active (×1.9)"});
    goalCombo->addItems({"Deficit", "Maintenance", "Surplus"});

    QPushButton *calcButton = new QPushButton("Calculate");

    form->addRow("Avg Weight:",    avgWeightLabel);
    form->addRow("Avg Glucose:",   avgGlucoseLabel);
    form->addRow("Avg BP:",        avgBPLabel);
    form->addRow("Weight Trend:",  trendLabel);
    form->addRow("Height (cm):",   heightEdit);
    form->addRow("Age:",           ageEdit);
    form->addRow("Sex:",           sexCombo);
    form->addRow("Activity:",      activityCombo);
    form->addRow("Goal:",          goalCombo);
    form->addRow("BMI:",           bmiLabel);
    form->addRow("Daily kcal:",    kcalLabel);

    layout->addLayout(form);
    layout->addWidget(calcButton);
    layout->addStretch();

    // Lambda captures the input widgets and calls onCalculateStats
    connect(calcButton, &QPushButton::clicked, this, [=]() {
        QList<Measurement> data = db->getMeasurementsByUser(currentUser.getId());
        if (data.isEmpty()) {
            QMessageBox::information(this, "No data", "No measurements recorded yet.");
            return;
        }

        avgWeightLabel->setText(QString::number(stats.averageWeight(data), 'f', 1) + " kg");
        avgGlucoseLabel->setText(QString::number(stats.averageGlucose(data), 'f', 1) + " mg/dL");
        avgBPLabel->setText(
            QString::number(stats.averageSystolic(data), 'f', 0) + "/" +
            QString::number(stats.averageDiastolic(data), 'f', 0) + " mmHg"
        );
        trendLabel->setText(stats.weightTrend(data));

        double height = heightEdit->text().toDouble();
        int    age    = ageEdit->text().toInt();
        bool   isMale = (sexCombo->currentIndex() == 0);
        double avgW   = stats.averageWeight(data);

        double actFactors[] = {1.2, 1.375, 1.55, 1.725, 1.9};
        double actFactor = actFactors[activityCombo->currentIndex()];
        int    goal      = goalCombo->currentIndex() - 1; // -1, 0, 1

        if (height > 0 && age > 0) {
            double bmi  = stats.computeBMI(avgW, height / 100.0);
            double kcal = stats.dailyKcal(avgW, height, age, isMale, actFactor, goal);
            bmiLabel->setText(QString::number(bmi, 'f', 1) +
                              " (" + stats.bmiCategory(bmi) + ")");
            kcalLabel->setText(QString::number(kcal, 'f', 0) + " kcal/day");
        } else {
            bmiLabel->setText("Enter height and age");
            kcalLabel->setText("Enter height and age");
        }
    });

    tabWidget->addTab(tab, "Statistics");
}

/**
 * @brief Sets up the Export tab for saving data to CSV.
 */
void MainWindow::setupExportTab() {
    QWidget     *tab    = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);

    exportButton      = new QPushButton("Export to CSV");
    exportStatusLabel = new QLabel("");

    layout->addStretch();
    layout->addWidget(exportButton);
    layout->addWidget(exportStatusLabel);
    layout->addStretch();

    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExportCSV);

    tabWidget->addTab(tab, "Export");
}

// ─────────────────────────────────────────────
//  SLOTS
// ─────────────────────────────────────────────

/**
 * @brief Reads form fields and saves a new measurement to the database.
 */
void MainWindow::onSaveMeasurement() {
    double weight    = weightEdit->text().toDouble();
    int    systolic  = systolicEdit->text().toInt();
    int    diastolic = diastolicEdit->text().toInt();
    double glucose   = glucoseEdit->text().toDouble();

    if (weight == 0 && systolic == 0 && diastolic == 0 && glucose == 0) {
        QMessageBox::warning(this, "Empty fields", "Please enter at least one value.");
        return;
    }

    Measurement m(currentUser.getId(), weight, systolic, diastolic,
                  glucose, QDateTime::currentDateTime());

    if (db->insertMeasurement(m)) {
        QMessageBox::information(this, "Saved", "Measurement saved successfully.");
        weightEdit->clear();
        systolicEdit->clear();
        diastolicEdit->clear();
        glucoseEdit->clear();
        onRefreshHistory();
    } else {
        QMessageBox::critical(this, "Error", "Failed to save measurement.");
    }
}

/**
 * @brief Reloads the history table from the database.
 */
void MainWindow::onRefreshHistory() {
    QList<Measurement> data = db->getMeasurementsByUser(currentUser.getId());
    historyTable->setRowCount(data.size());

    for (int i = 0; i < data.size(); ++i) {
        const Measurement &m = data[i];
        historyTable->setItem(i, 0, new QTableWidgetItem(
            m.getTimestamp().toString("yyyy-MM-dd hh:mm")));
        historyTable->setItem(i, 1, new QTableWidgetItem(
            QString::number(m.getWeight(), 'f', 1)));
        historyTable->setItem(i, 2, new QTableWidgetItem(
            QString::number(m.getSystolic())));
        historyTable->setItem(i, 3, new QTableWidgetItem(
            QString::number(m.getDiastolic())));
        historyTable->setItem(i, 4, new QTableWidgetItem(
            QString::number(m.getGlucose(), 'f', 1)));
    }
}

/**
 * @brief Exports all measurements to a CSV file chosen by the user.
 */
void MainWindow::onExportCSV() {
    QString path = QFileDialog::getSaveFileName(
        this, "Export CSV", "", "CSV Files (*.csv)");

    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        exportStatusLabel->setText("Failed to open file.");
        return;
    }

    QTextStream out(&file);
    out << "Timestamp,Weight(kg),Systolic,Diastolic,Glucose(mg/dL)\n";

    QList<Measurement> data = db->getMeasurementsByUser(currentUser.getId());
    for (const Measurement &m : data) {
        out << m.getTimestamp().toString("yyyy-MM-dd hh:mm") << ","
            << m.getWeight()    << ","
            << m.getSystolic()  << ","
            << m.getDiastolic() << ","
            << m.getGlucose()   << "\n";
    }

    file.close();
    exportStatusLabel->setText("Exported to: " + path);
}

void MainWindow::onCalculateStats() {}
