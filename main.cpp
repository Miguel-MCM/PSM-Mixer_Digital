#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "./filters.h"
#include "./inputController.h"
#include "./outputController.h"
#include "./espectroBarras.h"

bool continue_spectro = true;
std::mutex espectro_mutex;

bool continue_select_gains = true;
std::mutex  select_gains_mutex;

void show_spectro(EspectroBarras *espectro) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Espectro De Barras");
    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF")) {
        return;
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);

    text.setFillColor(sf::Color::White);

    text.setString(espectro->show());

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);


    while (window.isOpen() && continue_spectro) {
        sf::sleep(sf::seconds(1));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        espectro->absDFT();

        window.clear();
        text.setString(espectro->show());
        window.draw(text);
        window.display();

        std::lock_guard<std::mutex> lock(espectro_mutex);
        for (int i=0; i < BUFFER_SIZE; i++) {
            espectro->input[i][0] = 0;
        }
    }
    window.close();
}

void select_gains(TotalFilter *filter) {
    int selected_filter;
    double new_db_gain;

    while (continue_select_gains) {
        std::cout << std::endl;
        std::cout << std::endl;

        int i = 0;
        for (double &gain : filter->get_db_gains()) {
            std::cout << i << ": " << gain << "\t";
            i++;
        }
        std::cout << std::endl;
        std::cout << "Selecione o Indice do Filtro a ser modificado.\n";
        std::cout << "?";
        std::cin >> selected_filter;
        std::cout << "Selecione o novo ganho em db do filtro.\n";
        std::cout << "?";
        std::cin >> new_db_gain;

        std::lock_guard<std::mutex> lock(select_gains_mutex);
        filter->set_gain(selected_filter, new_db_gain);

        std::cout << std::endl;
        std::cout << std::endl;
    }
}

int main(int argc, char* agrv[]) {
    string file_input;
    if (argc < 2) {
        std::cout << "Qual o arquivo de som? (.wav)\n";
        std::getline(std::cin, file_input);
    } else {
        file_input = agrv[1];
    }

    InputController input_controller;
    input_controller.setFile(file_input);
    SF_INFO file_info = input_controller.getFileInfo();

    TotalFilter filter;

    vector<sf::Int16> output(BUFFER_SIZE*file_info.channels);

    vector<vector<int16_t>*> input_channels(file_info.channels);
    for (int i=0; i < file_info.channels; i++) {
        input_channels[i] = input_controller.getChannel(i);
    }

    OutputController output_controller(file_info.channels, file_info.samplerate);

    vector<vector<double>> channels(file_info.channels);

    output_controller.play();

    const int N_ESPECTRO = BUFFER_SIZE;
    EspectroBarras espectro(N_ESPECTRO);

    std::thread espectro_thread(show_spectro, &espectro);

    std::thread select_gains_thread(select_gains, &filter);

    while (input_controller.read_file()) {
        while (output_controller.getBufferedTime() > 1 && output_controller.getStatus() == OutputController::Playing) { }

        std::lock_guard<std::mutex> show_espectro_lock(select_gains_mutex);
        for (int i=0; i < file_info.channels; i++) {
            channels[i] = filter.convolve(*input_channels[i]);
        }

        for (int i=0; i < BUFFER_SIZE; i++) {
            for (int j=0; j < file_info.channels; j++) {
                output[i*file_info.channels + j] = static_cast<sf::Int16>(channels[j][i]);
            }
        }
        std::lock_guard<std::mutex> select_gains_lock(espectro_mutex);
        for (int i=0; i < BUFFER_SIZE; i++) {
            for (const vector<double> &channel : channels) {
                espectro.input[i][0] += static_cast<double>(channel[i]/INT16_MAX);
            }
        }

        output_controller.appendBuffer(output);
    }

    continue_select_gains = false;

    while (output_controller.getStatus() == OutputController::Playing) {  }

    continue_spectro = false;
    espectro_thread.join();

    select_gains_thread.join();

    input_controller.close_file();

    return 0;
}

