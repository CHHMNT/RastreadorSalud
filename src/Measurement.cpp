#include "Measurement.h"

Measurement::Measurement()
    : userId(-1), weight(0), systolic(0), diastolic(0), glucose(0) {}

Measurement::Measurement(int userId, double weight, int systolic,
                         int diastolic, double glucose, const QDateTime &timestamp)
    : userId(userId), weight(weight), systolic(systolic),
      diastolic(diastolic), glucose(glucose), timestamp(timestamp) {}

int       Measurement::getUserId()    const { return userId; }
double    Measurement::getWeight()    const { return weight; }
int       Measurement::getSystolic()  const { return systolic; }
int       Measurement::getDiastolic() const { return diastolic; }
double    Measurement::getGlucose()   const { return glucose; }
QDateTime Measurement::getTimestamp() const { return timestamp; }

void Measurement::setUserId(int i)               { userId = i; }
void Measurement::setWeight(double w)            { weight = w; }
void Measurement::setSystolic(int s)             { systolic = s; }
void Measurement::setDiastolic(int d)            { diastolic = d; }
void Measurement::setGlucose(double g)           { glucose = g; }
void Measurement::setTimestamp(const QDateTime &t) { timestamp = t; }
