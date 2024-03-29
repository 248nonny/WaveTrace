#pragma once

#include <glog/logging.h>

namespace Audio {

class AudioBuffer {
public:
    AudioBuffer();

    void test() {
        DLOG(INFO) << "AudioBuffer test.";
    }

    // fairly simple function that writes a value and advances to the next index.
    inline void write_value(float value) {
        data[index] = value;
        // printf("wrote %lf at index %d\n", data[index], index);
        advance_index();
    }

    // 0 is oldest value, -1 is most recent value.
    inline float read_value(int i, int ref_index = -1) {
        // ref_index is the index to treat as 0; this exists just in case something is writing data as we read (increasing the index),
        // which allows us to continue reading data in relation to the index we started with.
        // the index to use as "ref_index" can be obtained with the "get_current_index()" function.

        // set the ref_index to the current index if it is negative (default is -1, so if no parameter is given this is the output.)
        // (modulo buffer size so we don't go out of bounds.)
        ref_index = (ref_index < 0) ? index : ref_index % buffer_size;

        // this operation ensures that if i is a large negative number, perhaps even greater than buffer_size,
        // then the index that we get data from is still between 0 and buffer_size, and doesn't go out of bounds.
        return data[((ref_index + i) % buffer_size + buffer_size) % buffer_size];
    }

    void pop_array(float *output, int array_size);

    void print_contents();

    void set_sample_rate(int rate);         // set the sample rate of the incoming data.
    void set_buffer_period(double seconds); // set the length of time worth of data to store in the buffer.


    // inline getter functions.
    int get_sample_rate() {return sample_rate;}
    int get_buffer_period() {return buffer_period;}

    int get_buffer_size() {return buffer_size;}
    int get_current_index() {return index;}

private:
    // index should always be pointing at the oldest value (i.e. the next value to write to).
    int index = 0;
    float *data = nullptr;
    int buffer_size = 0;

    void resize_buffer(int size = -1);

    inline void advance_index() {
        // advance the index by 1, modulo "buffer_size."
        index = (index + 1) % buffer_size;
    }

    int sample_rate = 0; // in Hz
    double buffer_period = 0; // in seconds

};

}