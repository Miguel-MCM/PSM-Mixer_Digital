#ifndef OUTPUTCONTROLLER_H_
#define OUTPUTCONTROLLER_H_

#include <portaudio.h>
#include <vector>

using std::vector;

class OutputController {
 private:
    int buffer_size;
    int num_channels;
    int sample_rate;
    PaStream* stream;
    PaError err;

    vector<vector<double>> channels;
    static int callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
             const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

 public:
    OutputController(int buffer_size, int channels, int sample_rate);
    void play();
    bool open_stream();
    bool close_stream();
    void update_channels(const vector<vector<double>> &new_channels);
};


#endif  //  OUTPUTCONTROLLER_H_

