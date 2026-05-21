/**
 * @file Measurement.h
 * @brief Defines the Measurement class for biometric records.
 */
#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QString>
#include <QDateTime>

/**
 * @class Measurement
 * @brief Represents one biometric record with timestamp.
 */
class Measurement {
public:
    Measurement();
    Measurement(int userId, double weight, int systolic,
                int diastolic, double glucose, const QDateTime &timestamp);

    int getUserId() const;
    double getWeight() const;
    int getSystolic() const;
    int getDiastolic() const;
    double getGlucose() const;
    QDateTime getTimestamp() const;

    void setUserId(int userId);
    void setWeight(double weight);
    void setSystolic(int systolic);
    void setDiastolic(int diastolic);
    void setGlucose(double glucose);
    void setTimestamp(const QDateTime &timestamp);

private:
    int userId;          ///< ID of the user this measurement belongs to
    double weight;       ///< Body weight in kg
    int systolic;        ///< Systolic blood pressure in mmHg
    int diastolic;       ///< Diastolic blood pressure in mmHg
    double glucose;      ///< Blood glucose in mg/dL
    QDateTime timestamp; ///< Date and time of the measurement
};

#endif