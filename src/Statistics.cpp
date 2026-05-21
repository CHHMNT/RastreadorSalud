#include "Statistics.h"
#include <cmath>

Statistics::Statistics() {}
double Statistics::averageWeight(const QList<Measurement> &data) {
    if (data.isEmpty()) return 0;
    double sum = 0;
    for (const auto &m : data) sum += m.getWeight();
    return sum / data.size();
}
double Statistics::averageGlucose(const QList<Measurement> &data) {
    if (data.isEmpty()) return 0;
    double sum = 0;
    for (const auto &m : data) sum += m.getGlucose();
    return sum / data.size();
}

double Statistics::averageSystolic(const QList<Measurement> &data) {
    if (data.isEmpty()) return 0;
    double sum = 0;
    for (const auto &m : data) sum += m.getSystolic();
    return sum / data.size();
}
double Statistics::averageDiastolic(const QList<Measurement> &data) {
    if (data.isEmpty()) return 0;
    double sum = 0;
    for (const auto &m : data) sum += m.getDiastolic();
    return sum / data.size();
}
double Statistics::computeBMI(double weightKg, double heightM) {
    if (heightM <= 0) return 0;
    return weightKg / (heightM * heightM);
}
QString Statistics::bmiCategory(double bmi) {
    if (bmi < 18.5) return "Underweight";
    if (bmi < 25.0) return "Normal";
    if (bmi < 30.0) return "Overweight";
    return "Obese";
}
double Statistics::dailyKcal(double weightKg, double heightCm,
                              int ageYears, bool isMale,
                              double activityFactor, int goal) {
    double bmr = isMale
        ? (10 * weightKg) + (6.25 * heightCm) - (5 * ageYears) + 5
        : (10 * weightKg) + (6.25 * heightCm) - (5 * ageYears) - 161;
    double tdee = bmr * activityFactor;
    if (goal == -1) return tdee + DEFICIT_ADJUST;
    if (goal ==  1) return tdee + SURPLUS_ADJUST;
    return tdee;
}
QString Statistics::weightTrend(const QList<Measurement> &data) {
    if (data.size() < 2) return "Not enough data";
    double first = data.first().getWeight();
    double last  = data.last().getWeight();
    if (last > first) return "Increasing";
    if (last < first) return "Decreasing";
    return "Stable";
}
