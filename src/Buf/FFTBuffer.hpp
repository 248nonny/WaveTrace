#pragma once

#include <cstdio>
#include <math.h>
#include <stdint.h>
#include <chrono>

// some template buisiness, should only modify T1 T2 for name, or all if you want to add T3 or more Ts.
#define T1 Histogram_Size
#define T2 Array_Size
#define TEMPLATE template <int T1, int T2>
// this is for ez function declaration.
#define FnT FFTBuffer<T1, T2>


using Time = std::uint64_t;

const Time time0 = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

inline Time time_since_start() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time0;
}

// this is the format for the output of the getter function.
TEMPLATE
struct FFTData {
    double data[Histogram_Size][Array_Size][2];
    unsigned int timestamps[Histogram_Size]; // time in milliseconds since start of program
};

// class definition only here, functions and whatnot defined later.
TEMPLATE
class FFTBuffer {
    public:
        FFTBuffer();
        int arr_size();
        int histogram_size();
        unsigned int latest_timestamp();

        void push_array(double (&array)[Array_Size][2], unsigned int timestamp = 0);

        void print_contents(int histogram_start = 0, int histogram_end = Histogram_Size);
        void print_array(int i);
        

        // this gets the last n arrays in the histogram.
        template <int Sub_Histogram_Size>
        void pop_sub_histogram(double (&sub_histogram)[Sub_Histogram_Size][Array_Size][2],Time (&timestamps)[Sub_Histogram_Size]);
        
    private:
        double data[Histogram_Size][Array_Size][2];
        Time timestamps[Histogram_Size];
        // const double (*data_p)[Array_Size][12]; do we really need to store a pointer to the data? 
        // definitely we should not pass such a thing around to other objects.
        unsigned int index = 0; // should range from 0 to Histogram_Size - 1.
        // index represents the slot to be written at; as if it were the start of the array.
        // i.e. index - i is the most recent value.

        void advance_index();
        void clear_data();
        
};

TEMPLATE
FnT::FFTBuffer() {
    clear_data();
}


TEMPLATE
void FnT::clear_data() {
    for (int i = 0; i < Histogram_Size; i++) {
        for (int j = 0; j < Array_Size; j++) {
            data[i][j][0] = 0;
            data[i][j][1] = 0;
        }
    }
    index = 0;
}


TEMPLATE
void FnT::advance_index() {
    index = (index + 1) % Histogram_Size;
}


// should start every function declaration as:
// TEMPLATE
// FnT:: //which expands to the template equivalent of FFTBuffer::
TEMPLATE
int FnT::arr_size() {
    return T2;
}

TEMPLATE
int FnT::histogram_size() {
    return T1;
}

TEMPLATE
unsigned int FnT::latest_timestamp() {
    return timestamps[(index + Histogram_Size - 1) % Histogram_Size]; // what if index is 0? hence the + Hist._size.
}

TEMPLATE
void FnT::print_contents(int histogram_start, int histogram_end) {
    if (histogram_end <= histogram_start) {
        printf("FFTBuffer::print_contents error: end value must be > start value. \n");
    } else {
        
        printf("index: %d\n",index);

        for (int i = histogram_start; i < histogram_end; i++) {
            printf("time since start(ns) = %4d ", timestamps[i]);
            for (int j = 0; j < Array_Size; j++) {
                printf(" (%4.2lf,%4.2lf)",data[i][j][0],data[i][j][1]);
            }
            // printf("\n}\n");
            printf("\n");
        }
    }
}

TEMPLATE
void FnT::print_array(int i) {
    int hist_index = (2 * Histogram_Size + i) % Histogram_Size; //continue here
    printf("time: %lf {\n",(double)timestamps[hist_index]/1000);
    for (int j = 0; j < Array_Size; j++) {
        printf(" (%2.3lf,%2.3lf)",data[hist_index][j][0],data[hist_index][j][1]);
    }
    printf("\n}\n");
}


TEMPLATE
template <int Sub_Histogram_Size>
void FnT::pop_sub_histogram(double (&sub_histogram)[Sub_Histogram_Size][Array_Size][2], Time (&sub_timestamps)[Sub_Histogram_Size]) {

    int hist_index; // compute once, use many times! note that there is a safety in place to ensure it remains within master histogram index range.

    for (int i = 0; i < Sub_Histogram_Size; i++) {
        hist_index = ((index - Sub_Histogram_Size + i + Histogram_Size)%Histogram_Size);
        // hist_index = ((index - Sub_Histogram_Size + i)%Histogram_Size + Histogram_Size) % Histogram_Size;
        // printf("\n%d\n",index);
        // printf("%d\n",((index - Sub_Histogram_Size + i + Histogram_Size)%Histogram_Size));
        sub_timestamps[i] = timestamps[hist_index];
        for (int j = 0; j < Array_Size; j++) {
            sub_histogram[i][j][0] = data[hist_index][j][0];
            sub_histogram[i][j][1] = data[hist_index][j][1];
        }   
    }
}

TEMPLATE
void FnT::push_array(double (&array)[Array_Size][2], unsigned int timestamp) {

    for (int i = 0; i < Array_Size; i++) {
        data[index][i][0] = array[i][0];
        data[index][i][1] = array[i][1];
    }


    timestamps[index]=timestamp + (timestamp == 0) * time_since_start();

    advance_index();
}


#undef TEMPLATE
#undef FnT
#undef T1
#undef T2