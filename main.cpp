#include <iostream>
#include <cmath>
#include "filters.h"
#include "inputController.h"

int main() {
    InputController input_controller;

    string file_path = "Queen-–-Bohemian-Rhapsody-_Official-Video-Remastered_.wav";

    if (!input_controller.setFile(file_path)) {
        std::cout << "Deu Erro\n";
        return -1;
    }
    input_controller.teste();
    input_controller.close_file();

    return 0;
}
