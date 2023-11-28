#include "inputController.h"

#include <iostream>



bool InputController::setFile(string path) {
    file_path = path;
    file = sf_open(file_path.c_str(), SFM_READ, &file_info);

    if (!file) {
        return false;
    }

    std::cout << "Taxa de amostragem: " << file_info.samplerate << " Hz" << std::endl;
    std::cout << "Número de canais: " << file_info.channels << std::endl;
    std::cout << "Número de amostras: " << file_info.frames << std::endl;

    channels.resize(file_info.channels, vector<double>(BUFFER_SIZE, 0));

    // buffer.resize(BUFFER_SIZE*file_info.channels, 0);
    // buffer.reserve(BUFFER_SIZE*file_info.channels);
    buffer = new vector<double>(BUFFER_SIZE*file_info.channels + 1);

    return true;
}

void InputController::teste() {
    // double b[buffer.size()];
    // for (int i=0 ; i < buffer.size() ; i++) {
    //     b[i] = buffer[i];
    // }

    // double* b = new double[BUFFER_SIZE*file_info.channels];
    vector<double> b(BUFFER_SIZE*file_info.channels);

    while (sf_readf_double(file, buffer->data(), BUFFER_SIZE) > 0) {
        std::cout << (*buffer)[0] << std::endl;
        for (int i = 0; i < BUFFER_SIZE; i++) {
            for (int channel = 0; channel < file_info.channels; channel++) {
                double sample = (*buffer)[i * file_info.channels + channel];
                channels[channel][i] = sample;
            }
        }
    }
}

vector<double> InputController::getChannel(int channel) {
    return channels[channel];
}

void InputController::close_file() {
    sf_close(file);
}

// InputController::~InputController() {
//     delete buffer;
// }
