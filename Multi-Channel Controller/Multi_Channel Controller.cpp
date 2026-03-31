#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

struct SignalChannel {
    float* data = nullptr;
    int samples = 0;
    float augment = 1.0f;
    float bias = 0.0f;

    explicit SignalChannel(float* d, int s, float a = 1.0f)
        : data(d), samples(s), augment(a) { 
        float sum = 0.0f;
        for (int i = 0; i < s; i++) sum += d[i];
        bias = sum / static_cast<float>(s);
    }
};

// --- SENIOR-NO BRANCHING VERSION ---
void ProcessSeniorNB(SignalChannel& ch) {

    float* __restrict d = ch.data;
    const int s = ch.samples;
    const float a = ch.augment;
    const float bias = ch.bias;

    #pragma omp parallel for
    for (int i = 0; i < s; i++){  
        d[i] = std::max(0.0f, (d[i] * a) - bias); // No Branching 
    }
}

// --- SENIOR VERSION ---
void ProcessSenior(float* data, int n, float aug, float b) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        float val = (data[i] * aug) - b;
        data[i] = (val > 0.0f) ? val : 0.0f; // ternary
    }
}

// --- JUNIOR VERSION ---
void ProcessJunior(float* data, int n, float aug, float b) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        float val = (data[i] * aug) - b;
        if (val < 0.0f) val = 0.0f; // Branching
        data[i] = val;
    }
}

int main() {
    const int sample_size = 100000000; // Increased to 10M to see the gap
    std::vector<float> eeg_source(sample_size);

    // Create two identical datasets
    // 1. Create the initial random data
    std::mt19937 gen(42); // High-quality random engine
    std::uniform_real_distribution<float> dist(-0.5f, 0.5f);

    for(auto& x : eeg_source) x = dist(gen);

    // 2. Clone the data so both versions fight the same battle
    std::vector<float> data_senior = eeg_source;
    std::vector<float> data_seniorNB = eeg_source;
    std::vector<float> data_junior = eeg_source;

    SignalChannel sc(data_seniorNB.data(), sample_size, 2.0f);
    float aug = 2.0f;
    float b = sc.bias;

    // --- BENCHMARK JUNIOR ---
    // Warm-up
    ProcessJunior(data_junior.data(), sample_size, aug, b);

    auto start_j = std::chrono::high_resolution_clock::now();
    ProcessJunior(data_junior.data(), sample_size, aug, b);
    auto end_j = std::chrono::high_resolution_clock::now();

    // --- BENCHMARK SENIOR ---
    // Warm-up
    ProcessSenior(data_senior.data(), sample_size, aug, b); 
    
    auto start_s = std::chrono::high_resolution_clock::now();
    ProcessSenior(data_senior.data(), sample_size, aug, b);
    auto end_s = std::chrono::high_resolution_clock::now();

    // --- BENCHMARK SENIORNB ---
    // Warm-up

    ProcessSeniorNB(sc); 
    
    auto start_y = std::chrono::high_resolution_clock::now();
    ProcessSeniorNB(sc);
    auto end_y = std::chrono::high_resolution_clock::now();

    // CALCULATE RESULTS
    auto time_s = std::chrono::duration_cast<std::chrono::microseconds>(end_s - start_s).count();
    auto time_j = std::chrono::duration_cast<std::chrono::microseconds>(end_j - start_j).count();
    auto time_y = std::chrono::duration_cast<std::chrono::microseconds>(end_y - start_y).count();

    std::cout << "--- Preprocessing 100 Million Samples ---" << std::endl;
    std::cout << "SeniorNB (No Branching + Restrict): " << time_y << " us" << std::endl;
    std::cout << "Senior (Branching + Restrict): " << time_s << " us" << std::endl;
    std::cout << "Junior (Branching + No Restrict): " << time_j << " us" << std::endl;
    
    if (time_s < time_j) {
        std::cout << "Speedup: " << (float)time_j / time_s << "x faster!" << std::endl;
    }

    return 0;
}