/**
 * @file Database.h
 * @brief Handles all SQLite3 database operations.
 */
#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QList>
#include "User.h"
#include "Measurement.h"

/**
 * @class Database
 * @brief Manages connection and CRUD operations for the SQLite3 database.
 */
class Database {
public:
    Database();
    ~Database();

    bool open(const QString &path);
    void close();
    bool initSchema();

    // User operations
    bool insertUser(const QString &username, const QString &password);
    User getUserByUsername(const QString &username);

    // Measurement operations
    bool insertMeasurement(const Measurement &m);
    QList<Measurement> getMeasurementsByUser(int userId);

private:
    QString dbPath; ///< Path to the SQLite3 database file
};

#endif