#include <iostream>

void multiply_matrix(float* data, float scalar, int thread_id) {
    *(data + thread_id) = *(data + thread_id) * scalar;
}

int main(){
    float my_signal[3] = {0.2f, 0.1f, 0.3f};
    float scalar = 2.5f;
    
    std::cout << sizeof(my_signal) << std::endl;
    
    for (int i = 0; i < sizeof(my_signal) / sizeof(my_signal[0]); i++){

        std::cout << my_signal[i] << std::endl;
        multiply_matrix(my_signal, scalar, i);
        std::cout << my_signal[i] << std::endl;
        
    }
}

