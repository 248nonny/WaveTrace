
#include <fftw3.h>
#include "window-functions.hpp"

class FFTAgent {
public:
    FFTAgent(const int agent_id, const int array_size, FFTWindow::WindowType init_window_type, double sample_rate);
    void new_data();

    const int array_size;

private:

    void apply_window_function();


    double *window_array;
    double *in;
    fftw_complex *out;

    fftw_plan plan;

    int sample_rate;
    const int agent_id;
    const
    
    FFTWindow::WindowType window_type;
    
};