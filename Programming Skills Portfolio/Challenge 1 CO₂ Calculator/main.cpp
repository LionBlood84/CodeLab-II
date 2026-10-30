#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

double getDistanceTravelled() {
    double distance = 0.0;
    while (true) {
        std::cout << "Enter the distance travelled in miles: ";
        if (std::cin >> distance && distance >= 0) {
            return distance;
        }
        std::cout << "Invalid input. Please enter a positive number.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

char getFuelType() {
    char fuelChoice;
    while (true) {
        std::cout << "Select fuel type - (P)etrol or (D)iesel: ";
        std::cin >> fuelChoice;
        fuelChoice = std::toupper(fuelChoice);

        if (fuelChoice == 'P' || fuelChoice == 'D') {
            return fuelChoice;
        }
        std::cout << "Invalid choice. Please enter 'P' for Petrol or 'D' for Diesel.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

double calculateCO2(double distanceMiles, char fuelType) {
    const double LITRES_PER_100_MILES = 9.66;
    const double DIESEL_CO2_PER_LITRE = 2.68;
    const double PETROL_CO2_PER_LITRE = 2.31;

    double litresConsumed = (distanceMiles / 100.0) * LITRES_PER_100_MILES;
    double emissionFactor = (fuelType == 'D') ? DIESEL_CO2_PER_LITRE : PETROL_CO2_PER_LITRE;
    return litresConsumed * emissionFactor;
}

int main() {
    double distance = getDistanceTravelled();
    char fuelType = getFuelType();
    double totalCO2 = calculateCO2(distance, fuelType);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nDistance: " << distance << " miles\n";
    std::cout << "Fuel: " << (fuelType == 'D' ? "Diesel" : "Petrol") << "\n";
    std::cout << "Estimated CO2 Emissions: " << totalCO2 << " kg\n";

    return 0;
}