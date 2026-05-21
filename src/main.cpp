#include <QApplication>
#include <QLabel>
#include <QDebug>
#include "Database.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Test database connection
    Database db;
    if (db.open("/tmp/test_rastreador.db")) {
        qDebug() << "DB open: OK";

        // Test insert user
        db.insertUser("testuser", "1234");

        // Test retrieve user
        User u = db.getUserByUsername("testuser");
        qDebug() << "Retrieved user:" << u.getUsername() << "ID:" << u.getId();

        // Test insert measurement
        Measurement m(u.getId(), 70.5, 120, 80, 95.0, QDateTime::currentDateTime());
        db.insertMeasurement(m);

        // Test retrieve measurements
        QList<Measurement> list = db.getMeasurementsByUser(u.getId());
        qDebug() << "Measurements count:" << list.size();

        db.close();
    } else {
        qDebug() << "DB open: FAILED";
    }

    QLabel label("Database test — check terminal output");
    label.show();

    return app.exec();
}