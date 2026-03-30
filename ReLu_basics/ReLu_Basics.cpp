#include <iostream>

void kill_if_LZero(float* data, float activation_val, int thread_id) {

    if (*(data + thread_id) <= 0.0f){
        *(data + thread_id) = 0.0f;
    }
}

int main(){

    float data[] =  {-0.2f, 0.1f, 0.3f};
    float activation_val = 0.0f;

    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++){

        std::cout << data[i] << std::endl;
        kill_if_LZero(data, activation_val, i);
        std::cout << data[i] << std::endl;

    }

}