
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

const string path_timestamps{R"(E:\Datasets\brasov_03\1722253940836.txt)"};
const string path_frames{R"(E:\Datasets\brasov_03\frames)"};

namespace Tools {
    // (First:) Extract video to frames via: ffmpeg -i <videoName>.mp4 frames/%04d.png
    // Rename the frames to the according timestamps
    void renameFrames(const string &path_timestamps, const string &path_frames) {
        vector<long long> vTimestampsCam;
        vTimestampsCam.reserve(5000);

        // Read the frame's timestamp which were extracted by the app
        ifstream fTimes;
        fTimes.open(path_timestamps.c_str());
        while (!fTimes.eof()) {
            string s;
            getline(fTimes, s);

            if (!s.empty()) {
                int pos = s.find(' ');
                string item = s.substr(0, pos);

                long long t = stoll(item);
                vTimestampsCam.push_back(t);
            }
        }

        // Rename the frame image files accordingly
        for (int i = 0; i < vTimestampsCam.size(); ++i) {
            std::stringstream ss;
            ss << std::setw(4) << std::setfill('0') << i + 1;
            string before{path_frames + "\\" + ss.str() + ".png"};
            string after{path_frames + "\\" + to_string(vTimestampsCam[i]) + ".png"};

            if (rename(before.c_str(), after.c_str()) != 0) {
                cerr << "Failed to rename file " << before << " to " << after << endl;
                return;
            }
        }
    }
}

int main(int argc, char **argv) {
    cout << "Renaming frame files...";
    Tools::renameFrames(path_timestamps, path_frames);
    cout << "done!" << endl;
}