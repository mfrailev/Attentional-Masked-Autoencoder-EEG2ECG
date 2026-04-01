#include <iostream>
#include <vector>
#include <memory>
# include <random>
#include <algorithm>
#include <cassert>

class Tensor{
    public:
        int rows, cols;
        std::unique_ptr<float[]> data = nullptr;

        Tensor(int r, int c) : rows(r), cols(c), data(std::make_unique<float[]> (r*c)) {};

        inline float& operator()(int r, int c) {
            assert(r >= 0 && r <= rows && "Row index out of Bounds!");
            assert(c >= 0 && c <= cols && "Column index out of Bounds!");
            return data[r * cols + c];
        };

        void fill_constant(float value){
            std::fill(data.get(),data.get() + (rows * cols), value);
        };

        void fill_random(){
            std::default_random_engine generator;
            std::normal_distribution<float> distribution(0.0 ,1.0);

            for (int i = 0; i < (rows*cols); i++){
                data[i] = distribution(generator);
            };
        };

        void transpose(Tensor& ws){

            assert(ws.cols == rows && ws.rows == cols && "Matrix size is not compatible with tensor transpose!");

            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    ws(j,i) = (*this)(i,j);
                }
            }
        }
};

int main(){
    Tensor eeg_window(1024,129);
    Tensor workspace(129,1024); // Transpose pre-allocation

    eeg_window(0,128) = 0.5f;
    eeg_window.fill_random();
    std::cout << "Value at (0, 128): " << eeg_window(64, 128) << std::endl;
    eeg_window.transpose(workspace);
    std::cout << "Value at (0, 128): " << eeg_window(64, 128) << std::endl;
    return 0;
}