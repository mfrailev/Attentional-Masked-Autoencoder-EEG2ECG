#include <iostream>

void remove_dc_offset(float* data, float meanVal, int thread_id) {

    *(data + thread_id) = *(data + thread_id) - meanVal;

}

int main(){

    float data[] = {-0.2f, 0.1f, 0.3f};
    float meanVal = 10.0f;

    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++) {

        std::cout << data[i] << std::endl;
        remove_dc_offset(data, meanVal, i);
        std::cout << data[i] << std::endl;
        
    }

}