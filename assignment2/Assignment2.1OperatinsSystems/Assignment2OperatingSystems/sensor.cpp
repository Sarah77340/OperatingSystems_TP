#include "sensor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random> 
#define RED "\033[31m"
#define MAGENTA "\033[35m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define MAX_LEVEL 100

// Constructors
WaterSensor::WaterSensor(double criticalPoint)
	: criticalPoint(criticalPoint), waterLevel(0), provideWater(1), consumeWater(0) {}

WaterSensor::WaterSensor(double criticalPoint, double initialWaterLevel)
	: criticalPoint(criticalPoint), waterLevel(initialWaterLevel), provideWater(1), consumeWater(0) {}

// Simulate water production process
void WaterSensor::produce()
{
	std::random_device rd;
	while (1) {
		provideWater.acquire(); // Wait until allowed to produce water

		// Simulate random water production
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(5, MAX_LEVEL / 4);
		double amountProduced = dis(gen);
		for (int i = 0; i < amountProduced; i++) {
			if (waterLevel < MAX_LEVEL) {
				waterLevel++;
				std::cout << BLUE << "(Producing water): Tank capacity = " + std::to_string(waterLevel) + "%\n";
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}

		// Check if water level is below or at the critical point
		if (criticalPoint >= waterLevel) {
			std::cout << RED << "Tank is equal or lower than critical point\n" << RESET;
			provideWater.release();  // Allow more water production
		}
		else {
			consumeWater.release(); // Signal that water is available for consumption
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

// Simulate water consumption process
void WaterSensor::consume()
{
	std::random_device rd;
	while (1) {
		consumeWater.acquire(); // Wait until allowed to consume water

		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, waterLevel - 1);
		double amountConsumed = dis(gen);

		// Check if water level is above critical point
		if (waterLevel > criticalPoint) {
			for (int i = 0; i < amountConsumed; i++) {
				waterLevel--;
				std::cout << MAGENTA << "(Consuming water): Tank capacity = " + std::to_string(waterLevel) + "%\n";
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}

		// Check if water level is below maximum capacity
		if (waterLevel < MAX_LEVEL) {
			provideWater.release(); // Allow more water production
		}
		else {
			consumeWater.release(); // Signal that water is available for consumption
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
