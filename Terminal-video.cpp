

#include "Video.h"
#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;


// OpenCV
#include <opencv2/opencv.hpp>
using namespace cv;
int main(int argc, char* argv[]) {
	if (argc < 3 && (argv[2] != "-c" || argv[2] != "-g")) {
		cout << "Usage:" << argv[0] << " video_path [-g(rayscale)/-c(olor)]\n";
		return 0;
	}
	cout << "Video Player in ascii by me. Thanks for using. Future versions will incooperate video calling.\n";
	if (strcmp(argv[2],"-c") == 0) {
		Video vid = Video(argv[1], 400, 100, true);
		vid.displayVideo();
	}
	else {
		Video vid = Video(argv[1], 800, 200, false);
		vid.displayVideo();
	}
	return 0;
}
