#ifndef INCLUDE_INPUTCONTROLLER_H
#define INCLUDE_INPUTCONTROLLER_H

#include <sndfile.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class InputController {
 private:
    string file_path;
    SF_INFO file_info;
    SNDFILE *file;

    const int BUFFER_SIZE = 4096;
    vector<vector<double>> channels;
    vector<double> buffer;
 public:
    bool setFile(string path);
    bool read_file();
    void close_file();
    vector<double> getChannel(int channel);
};

#endif  //  INCLUDE_INPUTCONTROLLER_H
