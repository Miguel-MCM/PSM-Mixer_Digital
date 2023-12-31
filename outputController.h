#ifndef OUTPUTCONTROLLER_H_
#define OUTPUTCONTROLLER_H_

#include <vector>
#include <SFML/Audio.hpp>
#include <mutex>
#include <list>

using std::vector;
using std::list;

class OutputController : public sf::SoundStream{
 private:
     std::mutex output_mutex;
     virtual bool onGetData(Chunk& data);
     virtual void onSeek(sf::Time timeOffset);

     int chunk_size;
     size_t m_currentSample;
     bool should_pop;
     int buffered_samples;
 public:
     list<vector<sf::Int16>> buffers;
     OutputController(int num_channels, int sample_rate);
     void appendBuffer(vector<sf::Int16> &buffer);
     double getBufferedTime();
};


#endif  //  OUTPUTCONTROLLER_H_

