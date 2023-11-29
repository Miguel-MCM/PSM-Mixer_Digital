#include "outputController.h"
#include <iostream>
#include <cmath>

OutputController::OutputController(int buffer_size, int num_channels, int sample_rate) :
    buffer_size(buffer_size), num_channels(num_channels),
    channels(num_channels, vector<double>(buffer_size, .0)), sample_rate(sample_rate) {
}

int OutputController::callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {

    // Ignorar o buffer de entrada (inputBuffer) se não for usado

    // Obtém o ponteiro para os dados de áudio a serem reproduzidos
    double *out = static_cast<double*>(outputBuffer);

    OutputController* this_ptr = static_cast<OutputController*>(userData);


    // Preenche o buffer de saída com os dados dos canais
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        for (int channel = 0; channel < this_ptr->num_channels; channel++) {
            *out++ = this_ptr->channels[channel][i];
        }
    }

    return paContinue;
}

void OutputController::play() {

    // Inicia o stream de áudio
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Erro ao iniciar o stream de áudio: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return;
    }

    // Aguarda um tempo (em segundos) antes de encerrar o programa
    Pa_Sleep(5000);
    return;
}

bool OutputController::open_stream() {
    // Inicializa o PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "Erro ao inicializar o PortAudio: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    // Configuração do stream de áudio
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = num_channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    // Abre um stream de áudio
    err = Pa_OpenStream(&stream, nullptr, &outputParameters, sample_rate, buffer_size, paNoFlag, callback, this);
    if (err != paNoError) {
        std::cerr << "Erro ao abrir o stream de áudio: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return false;
    }
    return true;
}

bool OutputController::close_stream() {
    // Para o stream de áudio
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Erro ao parar o stream de áudio: " << Pa_GetErrorText(err) << std::endl;
    }

    // Fecha o stream de áudio
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "Erro ao fechar o stream de áudio: " << Pa_GetErrorText(err) << std::endl;
    }

    // Finaliza o PortAudio
    err = Pa_Terminate();
    if (err != paNoError) {
        std::cerr << "Erro ao finalizar o PortAudio: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

void OutputController::update_channels(const vector<vector<double>> &new_channels) {
    channels = new_channels;
}
