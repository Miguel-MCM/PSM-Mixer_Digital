#include "outputController.h"
#include <iostream>
#include <cmath>
// #include <time.h>


void MyAudioStream::load(sf::SoundBuffer* buffer) {
    // extract the audio samples from the sound buffer to our own container
    // m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
    std::cout << "fill: " << double(clock())/ double(CLOCKS_PER_SEC) << " " << m_currentSample << std::endl;

    this->buffer = buffer;
    // reset the current playing position
    m_currentSample = 0;

    // initialize the base class
    initialize(buffer->getChannelCount(), buffer->getSampleRate());
}

bool MyAudioStream::onGetData(Chunk& data) {
    // number of samples to stream every time the function is called;
    // in a more robust implementation, it should be a fixed
    // amount of time rather than an arbitrary number of samples
    // const int samplesToStream = 4096;
    std::cout << "get: " << double(clock())/ double(CLOCKS_PER_SEC) << " " << m_currentSample << std::endl;
    // set the pointer to the next audio samples to be played
    std::lock_guard<std::mutex> lock(*fill_buffer_mutex);
    data.samples = buffer->getSamples();

    data.sampleCount = buffer->getSampleCount();
    m_currentSample = buffer->getSampleCount();
    // have we reached the end of the sound?
    return true;
    }

void MyAudioStream::onSeek(sf::Time timeOffset) {
    std::lock_guard<std::mutex> lock(*fill_buffer_mutex);
    // compute the corresponding sample index according to the sample rate and channel count
    m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
}

OutputController::OutputController(int buffer_size, int num_channels, int sample_rate) :
    buffer_size(buffer_size), num_channels(num_channels),
    channels(num_channels, vector<double>(buffer_size, .0)), sample_rate(sample_rate) {
}

void OutputController::fillBuffer(const vector<sf::Int16> * audioData) {
    buffer.loadFromSamples(audioData->data(), buffer_size, num_channels, sample_rate);
    stream.load(&buffer);
}

void OutputController::play() {
    // stream.load(&buffer);
    // stream.play();
    sf::Sound sound(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing) {}
}

bool OutputController::is_playng() {
    return stream.getStatus() == MyAudioStream::Playing;
}

void OutputController::finish() {
    stream.stop();
}

// void OutputController::setStream(MyAudioStream *stream) {this->stream = stream;}
