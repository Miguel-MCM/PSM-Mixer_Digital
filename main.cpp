#include <iostream>
#include <cmath>
#include "filters.h"
#include "inputController.h"

int main() {

    string file_path = "Queen-–-Bohemian-Rhapsody-_Official-Video-Remastered_.wav";
    InputController input_controller;

    if (!input_controller.setFile(file_path)) {
        std::cout << "Deu Erro\n";
        return -1;
    }
    while (input_controller.read_file()) {
        std::cout << input_controller.getChannel(0)[0] <<  " " << input_controller.getChannel(1)[0] << std::endl;
    }



    input_controller.close_file();

    return 0;
}
