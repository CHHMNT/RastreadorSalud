#include "Database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QTextStream>

Database::Database() {}

Database::~Database() {
    close();
}

/**
 * @brief Opens the SQLite3 database at the given path.
 * @param path Full file path to the .db file
 * @return true if connection was successful
 */
bool Database::open(const QString &path) {
    dbPath = path;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database opened at:" << path;
    return initSchema();
}

/**
 * @brief Closes the database connection.
 */
void Database::close() {
    QSqlDatabase::database().close();
}

/**
 * @brief Creates the tables if they don't exist yet.
 * @return true if schema was applied successfully
 */
bool Database::initSchema() {
    QSqlQuery query;

    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL"
        ");"
    );

    if (!ok) {
        qDebug() << "Schema error (users):" << query.lastError().text();
        return false;
    }

    ok = query.exec(
        "CREATE TABLE IF NOT EXISTS measurements ("
        "id        INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id   INTEGER NOT NULL,"
        "weight    REAL,"
        "systolic  INTEGER,"
        "diastolic INTEGER,"
        "glucose   REAL,"
        "timestamp TEXT NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id)"
        ");"
    );

    if (!ok) {
        qDebug() << "Schema error (measurements):" << query.lastError().text();
        return false;
    }

    qDebug() << "Schema initialized successfully.";
    return true;
}

/**
 * @brief Inserts a new user into the database.
 * @param username Login name
 * @param password Plain text password
 * @return true if insert succeeded
 */
bool Database::insertUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);

    if (!query.exec()) {
        qDebug() << "Insert user error:" << query.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Retrieves a user by username.
 * @param username The username to look up
 * @return A User object; id will be -1 if not found
 */
User Database::getUserByUsername(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT id, username, password FROM users WHERE username = :u");
    query.bindValue(":u", username);

    if (query.exec() && query.next()) {
        return User(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString()
        );
    }

    return User(); // Returns empty user with id = -1
}

/**
 * @brief Inserts a new biometric measurement.
 * @param m The Measurement object to store
 * @return true if insert succeeded
 */
bool Database::insertMeasurement(const Measurement &m) {
    QSqlQuery query;
    query.prepare(
        "INSERT INTO measurements (user_id, weight, systolic, diastolic, glucose, timestamp) "
        "VALUES (:uid, :w, :s, :d, :g, :t)"
    );
    query.bindValue(":uid", m.getUserId());
    query.bindValue(":w",   m.getWeight());
    query.bindValue(":s",   m.getSystolic());
    query.bindValue(":d",   m.getDiastolic());
    query.bindValue(":g",   m.getGlucose());
    query.bindValue(":t",   m.getTimestamp().toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "Insert measurement error:" << query.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Retrieves all measurements for a given user, ordered by timestamp.
 * @param userId The user's ID
 * @return List of Measurement objects
 */
QList<Measurement> Database::getMeasurementsByUser(int userId) {
    QList<Measurement> list;
    QSqlQuery query;
    query.prepare(
        "SELECT weight, systolic, diastolic, glucose, timestamp "
        "FROM measurements WHERE user_id = :uid ORDER BY timestamp ASC"
    );
    query.bindValue(":uid", userId);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        Measurement m(
            userId,
            query.value(0).toDouble(),
            query.value(1).toInt(),
            query.value(2).toInt(),
            query.value(3).toDouble(),
            QDateTime::fromString(query.value(4).toString(), Qt::ISODate)
        );
        list.append(m);
    }

    return list;
}