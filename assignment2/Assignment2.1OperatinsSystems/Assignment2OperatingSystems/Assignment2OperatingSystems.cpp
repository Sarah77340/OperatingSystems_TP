#include <iostream>
#include <thread>
#include <semaphore>
#include "sensor.h"
#include <functional>


int main() {
    WaterSensor sensor(15);
    std::thread producer_thread(std::bind(&WaterSensor::produce, &sensor));
    std::thread consumer_thread(std::bind(&WaterSensor::consume, &sensor));

    producer_thread.join();
    consumer_thread.join();

    return 0;
}