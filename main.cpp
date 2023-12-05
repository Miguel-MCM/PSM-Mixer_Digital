#include <iostream>
#include <thread>

#include "./filters.h"
#include "./inputController.h"
#include "./outputController.h"
#include "./espectroBarras.h"

bool continue_spectro = true;
std::mutex espectro_mutex;


void show_spectro(EspectroBarras *espectro) {
    while (continue_spectro) {
        // std::cout << espectro->barras.size() << "\n";
        sf::sleep(sf::seconds(1));

        espectro->absDFT();

        std::cout << espectro->show() << std::endl;

        std::lock_guard<std::mutex> lock(espectro_mutex);
        for (int i=0; i < BUFFER_SIZE; i++) {
            espectro->input[i][0] = 0;
        }
    }
}

int main() {
    const string file_input = "Queen – Bohemian Rhapsody (Official Video Remastered).wav";
    InputController input_controller;
    input_controller.setFile(file_input);
    SF_INFO file_info = input_controller.getFileInfo();

    TotalFilter filter;

    filter.set_gain(0, -20);
    filter.set_gain(1, -20);
    filter.set_gain(2, -20);
    filter.set_gain(3, -20);
    filter.set_gain(4, -20);
    filter.set_gain(5, 20);
    filter.set_gain(6, -20);
    filter.set_gain(7, -20);
    filter.set_gain(8, -10);
    filter.set_gain(9, 0);


    vector<sf::Int16> test_audio(BUFFER_SIZE*2);

    vector<int16_t> *input_channel0 = input_controller.getChannel(0);
    vector<int16_t> *input_channel1 = input_controller.getChannel(1);

    OutputController output_controller(file_info.channels, file_info.samplerate);

    vector<double> canal1;
    vector<double> canal2;

    output_controller.play();

    const int N_ESPECTRO = BUFFER_SIZE;
    EspectroBarras espectro(N_ESPECTRO);

    std::thread espectro_thread(show_spectro, &espectro);

    while (input_controller.read_file()) {
        while (output_controller.getBufferedTime() > 1 && output_controller.getStatus() == OutputController::Playing) { }

        canal1 = filter.convolve(*input_channel0);
        canal2 = filter.convolve(*input_channel1);

        // std::cout << output_controller.buffers.size() << " a\n";
        for (int i=0; i < BUFFER_SIZE; i++) {
            test_audio[i*2] = static_cast<sf::Int16>(canal1[i]);
            test_audio[i*2+1] = static_cast<sf::Int16>(canal2[i]);
        }
        std::lock_guard<std::mutex> lock(espectro_mutex);
        for (int i=0; i < BUFFER_SIZE; i++) {
            espectro.input[i][0] += static_cast<double>(canal1[i]) / INT16_MAX;
            espectro.input[i][0] += static_cast<double>(canal2[i]) / INT16_MAX;
            // espectro.input[i][0] /= INT16_MAX*2;
        }

        output_controller.appendBuffer(test_audio);
    }

    std::cout << "foi o arquivo.\n";

    while (output_controller.getStatus() == OutputController::Playing) {
        // sf::sleep(sf::seconds(1));
    }

    continue_spectro = false;
    espectro_thread.join();


    input_controller.close_file();



    return 0;
}

