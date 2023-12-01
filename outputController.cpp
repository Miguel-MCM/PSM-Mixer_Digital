#include "outputController.h"
#include <iostream>
// #include <time.h>
OutputController::OutputController(int num_channels, int sample_rate) {
    initialize(num_channels, sample_rate);
    chunk_size = sample_rate/10;
    m_currentSample = 0;
    should_pop = false;
}

void OutputController::appendBuffer(vector<sf::Int16> &buffer) {
    buffers.push_back(buffer);
}

bool OutputController::onGetData(Chunk &data) {
    if (should_pop) {
        buffers.pop_front();
        should_pop = false;
        m_currentSample = 0;
            if (buffers.empty()) {
                std::cout << "demoraaa\n";
                return false;
            }
    }
    data.samples = &(buffers.front()[m_currentSample]);

    if (m_currentSample + chunk_size < buffers.front().size()) {
        data.sampleCount = chunk_size;
        m_currentSample += chunk_size;
        // std::cout << buffers.front().size() - m_currentSample << " b\n";
    } else {
        data.sampleCount = buffers.front().size() - m_currentSample;
        // buffers.pop_front();
        should_pop = true;

        // std::cout << buffers.front().size() - m_currentSample << " c\n";
    }

    return true;
}

void OutputController::onSeek(sf::Time timeOffset) {
    m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
}
