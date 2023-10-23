
#include <cstdio>
#define TEMPLATE template <int Buf_Size>
#define FnT AudioBuffer<Buf_Size>

#define AUDIO_BUFFER_SIZE 64

TEMPLATE
class AudioBuffer {
    public:
        AudioBuffer();
        int buf_size();
        
        void push_data(int data_size, double (*data_array)) {
            for (int i = 0; i < data_size; i++) {
                data[(index + i) % Buf_Size] = data_array[i];
            }
            index = (index+data_size)%Buf_Size;
        }

        void pop_data(int size, double (*data_array));

        void print_contents(int start = 0, int end = Buf_Size - 1);
        void clear_data();

    private:
        double data[Buf_Size];
        int index;
};

TEMPLATE
FnT::AudioBuffer() {
    clear_data();
}

TEMPLATE
void FnT::clear_data() {
    for (int i = 0; i < Buf_Size; i++) {
        data[i] = 0;
    }
    index = 0;
}

TEMPLATE
int FnT::buf_size() {
    return Buf_Size;
}

TEMPLATE
void FnT::print_contents(int start, int end) {
    int indx;
    printf("Printing audio buffer contents...\n");
    for (int i = start; i <= end; i++) {
        indx = (i + index) % Buf_Size;
        // indx = i;
        printf("%d:%.2lf, ",indx, data[indx]);
    }
    printf("\n");
}

TEMPLATE
void FnT::pop_data(int size, double (*data_array)) {
    int indx = (((index - size)%Buf_Size) + Buf_Size)%Buf_Size;
    for (int i = 0; i < size; i++) {
        data_array[i] = data[(i + indx)%Buf_Size];
    }
}