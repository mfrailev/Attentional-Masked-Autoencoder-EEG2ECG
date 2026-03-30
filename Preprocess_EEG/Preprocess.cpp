#include <iostream>

void Preprocessing(float* data, float augment, float bias, float activation, int thread_id) {

    *(data + thread_id) = (*(data + thread_id) * augment) - bias;

    if (*(data + thread_id) <= activation){
        *(data + thread_id) = 0;
    }
}

int main(){

    float data[] = {0.2f, 0.1f, 0.3f};
    int n = sizeof(data) / sizeof(data[0]);
    float augment = 2.0f;
    float bias = 5.0f;

    float sum = 0.0f;
    for (int i = 0; i < n; i++){
        sum += data[i];
    }
    float mean_bias = sum = sum / n;

    float activation = 0.0f;

    for (int i = 0; i < n; i ++){

        std::cout << data[i] << std::endl;
        Preprocessing(data, augment, bias, activation, i);
        std::cout << data[i] << std::endl;

    }

    return 0;
}