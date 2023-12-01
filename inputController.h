#ifndef INPUTCONTROLLER_H_
#define INPUTCONTROLLER_H_

#include <sndfile.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

const int BUFFER_SIZE = 44100/10;

class InputController {
 private:
    string file_path;
    SF_INFO file_info;
    SNDFILE *file;

    vector<vector<int16_t>> channels;
    vector<int16_t> buffer;
 public:
    bool setFile(string path);
    bool read_file();
    void close_file();
    inline SF_INFO getFileInfo() {return file_info;}
    vector<int16_t>* getChannel(int channel);
};

#endif  //  INPUTCONTROLLER_H_
