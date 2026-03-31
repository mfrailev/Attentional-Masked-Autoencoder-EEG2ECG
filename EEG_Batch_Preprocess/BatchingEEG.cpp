#include <iostream>
#include <vector>

struct SignalChannel {
    float* data;
    int samples;
    float augment;
    float bias;

    SignalChannel(float* d, float s, float a){
        data = d;
        samples = s;
        augment = a;

        float sum = 0.0f;
        for (int i = 0; i < s; i++) sum += d[i];
        bias = sum / s;
    }
};

void ProcessDataset(SignalChannel* ch){

    for (int i = 0; i < ch->samples; i++){
        float* val = &ch -> data[i];
        *val = (*val * ch -> augment) - ch -> bias;
        if (*val < 0.0f) *val = 0.0f; 
    }
}


int main(){

    int large_n = 1000000;
    std::vector<float> eeg_array(large_n, 0.5f);

    SignalChannel Preprocess(eeg_array.data(), large_n, 2.0f);

    std::cout << "First sample after processing: " << eeg_array[0] << std::endl;
    ProcessDataset(&Preprocess);
    std::cout << "First sample after processing: " << eeg_array[0] << std::endl;
    return 0;
}
