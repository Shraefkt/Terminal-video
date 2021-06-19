#pragma once
#include <iostream>
#include <Windows.h>
using namespace std;

// OpenCV
#include <opencv2/opencv.hpp>
using namespace cv;

class Video
{
private:
	//Video settings
	char cVideoName[100];
	int resolution[2];
	COORD cBufferSize;
	bool bColor = false;
	bool bPaused = false;

	//Video objects
	VideoCapture VCvideo;
	Mat mCurrentFrame;

	bool bVideoOver = false;
	int count = 0;
	//Console 
	int nScreenWidth;
	int nScreenHeight;
	SMALL_RECT consoleWriteArea;

	CHAR_INFO * consoleBuffer;

	//Conversion to ASCII
	WORD* wColors;
	int iGscale = 66;
	wchar_t Gscale[66] = L"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~i!lI;:,^`. ";

	//display
	bool bKey[3][2];
public:
	//Constructor
	Video(char vid[], int res0,int res1, bool color) {
		int i = 0;
		while (vid[i] != '\0') {
			cVideoName[i] = vid[i];
			i++;
		}
		resolution[0] = res0;
		resolution[1] = res1;
		bColor = color;

		cBufferSize = { (SHORT)resolution[0], (SHORT)resolution[1] };
		nScreenWidth = resolution[0];
		nScreenHeight = resolution[1];
		consoleWriteArea = { 0, 0, (SHORT)nScreenWidth, (SHORT)nScreenHeight };
		consoleBuffer = new CHAR_INFO[(nScreenHeight)*nScreenWidth];
		wColors = new WORD[nScreenHeight * nScreenWidth];

		cout << "Loading: " << cVideoName;
		cout << "\nSettings:\n" << "\tResolution: " << resolution[0] << "\tColoured: " << bColor << "\n";
		cout << "Space to pause, pause/break to stop video. Enjoy!\n";
		Sleep(1500);
	}
	//Loading
	void loadVideo();
	void loadFrame();

	//Utils for conversion
	wchar_t convertToAscii(int intensity);
	int convertTocolor(Vec3b vColor);
	void loadScreen();
	
	//Console rendering
	void displayVideo();

	// Display
	void keyboard();
	//Error handling

};