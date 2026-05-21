/**
 * @file Statistics.h
 * @brief Provides statistical calculations on biometric data.
 */
#ifndef STATISTICS_H
#define STATISTICS_H

#include <QList>
#include "Measurement.h"

/**
 * @class Statistics
 * @brief Computes averages, trends, BMI, and daily caloric needs.
 */
class Statistics {
public:
    Statistics();

    double averageWeight(const QList<Measurement> &data);
    double averageGlucose(const QList<Measurement> &data);
    double averageSystolic(const QList<Measurement> &data);
    double averageDiastolic(const QList<Measurement> &data);

    double computeBMI(double weightKg, double heightM);
    QString bmiCategory(double bmi);

    /**
     * @brief Calculates daily kcal using Mifflin-St Jeor formula.
     * @param weightKg Body weight in kg
     * @param heightCm Height in cm
     * @param ageYears Age in years
     * @param isMale True if male, false if female
     * @param activityFactor 1.2 sedentary … 1.9 very active
     * @param goal -1 = deficit, 0 = maintenance, 1 = surplus
     */
    double dailyKcal(double weightKg, double heightCm, int ageYears,
                     bool isMale, double activityFactor, int goal);

    QString weightTrend(const QList<Measurement> &data);

private:
    static constexpr double DEFICIT_ADJUST  = -400.0; ///< kcal adjustment for deficit
    static constexpr double SURPLUS_ADJUST  = +400.0; ///< kcal adjustment for surplus
};

#endif