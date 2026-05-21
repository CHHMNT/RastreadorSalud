#include "Database.h"

Database::Database() {}
Database::~Database() { close(); }

bool Database::open(const QString &path) {
    dbPath = path;
    return true; // Full implementation in Phase 6
}

void Database::close() {}

bool Database::initSchema()                              { return true; }
bool Database::insertUser(const QString&, const QString&) { return true; }
User Database::getUserByUsername(const QString&)         { return User(); }
bool Database::insertMeasurement(const Measurement&)     { return true; }
QList<Measurement> Database::getMeasurementsByUser(int)  { return {}; }
