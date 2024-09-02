#pragma once
#include <semaphore>

class WaterSensor {
private:
	double waterLevel;
	double criticalPoint;
	std::binary_semaphore provideWater;
	std::binary_semaphore consumeWater;

public:
	WaterSensor(double criticalPoint);
	WaterSensor(double criticalPoint, double initialWaterLevel);

	void produce();
	void consume();
};
