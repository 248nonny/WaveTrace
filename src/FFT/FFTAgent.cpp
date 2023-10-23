#include "FFTAgent.hpp"
#include <fftw3.h>

FFTAgent::FFTAgent(const int agent_id, const int array_size, FFTWindow::WindowType init_window_type, double sample_rate):
array_size(array_size),
agent_id(agent_id),
window_type(init_window_type)
{

    // Allocate arrays for storing window, in, and out arrays.
    window_array = (double *)fftw_malloc(sizeof(double) * array_size);
    in = (double *)fftw_malloc(sizeof(double) * array_size);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (array_size / 2 + 3));
    plan = fftw_plan_dft_r2c_1d(array_size, in, out, FFTW_MEASURE);

}