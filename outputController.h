#ifndef OUTPUTCONTROLLER_H_
#define OUTPUTCONTROLLER_H_

#include <vector>
#include <SFML/Audio.hpp>
#include <mutex>

using std::vector;

class MyAudioStream : public sf::SoundStream {
 public:
    void load(sf::SoundBuffer * buffer);
    inline void set_mutex(std::mutex *fill_buffer_mutex) {this->fill_buffer_mutex = fill_buffer_mutex;}
    inline size_t getCurrentSample() {return m_currentSample;}
   //  MyAudioStream();
 private:
    virtual bool onGetData(Chunk& data);
    virtual void onSeek(sf::Time timeOffset);
    std::mutex *fill_buffer_mutex;
    sf::SoundBuffer *buffer;

    vector<sf::Int16> m_samples;
    size_t m_currentSample;
};

class OutputController {
 private:
    int buffer_size;
    int num_channels;
    int sample_rate;
    bool keep_playing = false;

    vector<vector<double>> channels;
    sf::SoundBuffer buffer;
    MyAudioStream stream;
    std::mutex *fill_buffer_mutex;


 public:
    inline void set_mutex(std::mutex *fill_buffer_mutex) {
      this->fill_buffer_mutex = fill_buffer_mutex;
      stream.set_mutex(fill_buffer_mutex);}
    OutputController(int buffer_size, int channels, int sample_rate);
   //  void setStream(MyAudioStream *stream);
    void play();
    void fillBuffer(const vector<sf::Int16> * audioData);
    bool is_playng();
    void finish();
    inline size_t getCurrentSample() {return stream.getCurrentSample();}
};


#endif  //  OUTPUTCONTROLLER_H_

