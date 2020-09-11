#include "concur2020lib/concur2020.hh"
#include <iostream>
#include <mutex>
#include <thread>

const int SAMPLES = 125;

// Mutex used for locking.
std::mutex mu;

typedef uint64_t Counter;
struct {
    Counter alpha;
    Counter beta;
    Counter gamma;
} counters = { 0, 0, 0 };

void printCounters() {

    std::cout << "COUNTER:" << std::endl;
    std::cout << "alpha: " << counters.alpha << std::endl;
    std::cout << "beta : " << counters.beta << std::endl;
    std::cout << "gamma: " << counters.gamma << std::endl;
}

void countSample( concur2020::DetectorData_t sample ) {

    auto name = concur2020::itemName( sample );
    if( name == "alpha" ) {
        counters.alpha++;
    } else if( name == "beta" ) {
        counters.beta++;
    } else if( name == "gamma" ) {
        counters.gamma++;
    }

}

void processSample() {

    // Without using mutex, assertion of
    // detect function returns false
    std::lock_guard<std::mutex> guard(mu);

    auto sample = concur2020::detect();

    std::cout << "Read: " << concur2020::itemName( sample ) << std::endl;
    countSample( sample );

}


int main() {

    // Create 8 threads running sample processing.
    std::thread threads[8];  
    for (int i = 0; i < SAMPLES; i ++) {

        for (int i = 0; i < 8; i ++) {
            threads[i] = std::thread(processSample);
        }
        // Wait for threads to finish.
        for (int i = 0; i < 8; i ++) {
            threads[i].join();
        }
    }

    printCounters();
    return EXIT_SUCCESS;

}
