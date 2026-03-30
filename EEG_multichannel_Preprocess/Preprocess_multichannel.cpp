#include <iostream>
#include <chrono>

struct SignalChannel {
    float* samples;
    int num_samples;
    float augment;
    float bias;

    SignalChannel(float* s, int n, float a){
        samples = s;
        num_samples = n;
        augment = a;

        float sum = 0.0f;
        for (int i = 0; i < n; i++) sum += s[i];
        bias = sum /n;
    }
};

void ProcessChannel(SignalChannel* ch, int thread_id){

    float* current_sample = ch -> samples + thread_id;
    *current_sample = (*current_sample * ch-> augment) - ch -> bias;
    
    if (*current_sample < 0.0f){
        *current_sample = 0.0f;
    }
    
}

int main(){

    float data[] = {0.5f, 0.3f, 0.8f};
    int n = sizeof(data)/sizeof(data[0]);

    SignalChannel MyChannel(data, n, 2.0f);
    std::cout << "Calculated Bias: " << MyChannel.bias << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++){
        ProcessChannel(&MyChannel, i);
        std::cout << "Sample " << i << ": " << data[i] << std::endl;        
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Kernel Latency: " << elapsed.count() << " ms" << std::endl;
    return 0;
}
