#ifndef OUTPUTCONTROLLER_H_
#define OUTPUTCONTROLLER_H_

#include <vector>
#include <SFML/Audio.hpp>
// #include <mutex>
#include <list>

using std::vector;
using std::list;

class OutputController : public sf::SoundStream{
 private:
      virtual bool onGetData(Chunk& data);
      virtual void onSeek(sf::Time timeOffset);

      list<vector<sf::Int16>> buffers;
      int chunk_size;
      size_t m_currentSample;
 public:
      OutputController(int num_channels, int sample_rate);
      void appendBuffer(vector<sf::Int16> &buffer);
};


#endif  //  OUTPUTCONTROLLER_H_

