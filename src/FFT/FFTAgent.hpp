
#include <fftw3.h>
#include "window-functions.hpp"

class FFTAgent {
public:

    void new_data();

private:

    void apply_window_function();


    double *window_array;
    double *in;
    fftw_complex *out;

    fftw_plan plan;

    int sample_rate;
    
};