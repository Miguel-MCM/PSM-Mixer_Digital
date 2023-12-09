#include "inputController.h"

bool InputController::setFile(string path) {
    file_path = path;
    file = sf_open(file_path.c_str(), SFM_READ, &file_info);

    if (!file) {
        return false;
    }

    channels.resize(file_info.channels, vector<int16_t>(BUFFER_SIZE, 0));
    vector<int16_t> buffer(BUFFER_SIZE*file_info.channels, 0);
    this->buffer = buffer;

    return true;
}

bool InputController::read_file() {
    if (sf_readf_short(file, buffer.data(), BUFFER_SIZE) > 0) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            for (int channel = 0; channel < file_info.channels; channel++) {
                int16_t sample = buffer[i * file_info.channels + channel];
                channels[channel][i] = sample;
            }
        }
        return true;
    }
    return false;
}

vector<int16_t>* InputController::getChannel(int channel) {
    return &(channels[channel]);
}

void InputController::close_file() {
    sf_close(file);
}
