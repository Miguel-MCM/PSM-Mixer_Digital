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
    vector<double> buffer(BUFFER_SIZE*file_info.channels, 0);
    this->buffer = buffer;


    return true;
}

bool InputController::read_file() {
    if (sf_readf_double(file, buffer.data(), BUFFER_SIZE) > 0) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            for (int channel = 0; channel < file_info.channels; channel++) {
                double sample = buffer[i * file_info.channels + channel];
                channels[channel][i] = sample;
            }
        }
        return true;
    }
    return false;
}

vector<double> InputController::getChannel(int channel) {
    return channels[channel];
}

void InputController::close_file() {
    sf_close(file);
}
