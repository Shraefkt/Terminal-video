#include "Video.h"
void Video::loadVideo()
{
    try
    {
        VCvideo = VideoCapture(cVideoName);
    }
    catch (const std::exception&)
    {
        cout << cVideoName << "does not exist.\n";
        cout << "Check filename and give full directory path\n";
        exit(0);
    }
    if (!VCvideo.isOpened()) {
        cout << cVideoName << endl;
        exit(0);
    }
    loadFrame();
    VCvideo.set(CAP_PROP_FRAME_WIDTH, resolution[0]);
    VCvideo.set(CAP_PROP_FRAME_HEIGHT, resolution[1]);
}

void Video::loadFrame()
{
    VCvideo >> mCurrentFrame;
    if (mCurrentFrame.empty()) {
        bVideoOver = true;
        return;
    }
    resize(mCurrentFrame, mCurrentFrame, Size(resolution[0], resolution[1]), 0, 0, INTER_CUBIC);
}


wchar_t Video::convertToAscii(int iIntensity)
{
    return Gscale[int(iIntensity / (256 / float(iGscale)))];
}

int Video::convertTocolor(Vec3b vColor)
{
    uchar colors[3];
    for (int i = 0; i < 3; i++) {
        if (vColor[i] % 20 > 10)
            colors[i] = vColor[i] - vColor[i] % 20 + 20;
        else
            colors[i] = vColor[i] - vColor[i] % 20;
    }
    return RGB(colors[0], colors[1], colors[2]);
}

void Video::loadScreen()
{
    for (int y = 0; y < nScreenHeight; y++) {
        for (int x = 0; x < nScreenWidth; x++) {
            Vec3b vColor = mCurrentFrame.at<Vec3b>(y, x);
            consoleBuffer[(y * nScreenWidth) + x].Attributes = bColor ? convertTocolor(vColor) : 7;
        }
    }
    //Intensity
    cvtColor(mCurrentFrame, mCurrentFrame, COLOR_BGR2GRAY);
    for (int y = 0; y < nScreenHeight; y++) {
        for (int x = 0; x < nScreenWidth; x++) {
            Scalar intensity = mCurrentFrame.at<uchar>(y, x);
            consoleBuffer[(y * nScreenWidth) + x].Char.AsciiChar = bColor ? L' ' : convertToAscii(intensity[0]);
        }
    }
}

void Video::displayVideo()
{
    loadVideo();
    //Create screen buffer
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleWindowInfo(hConsole, TRUE, &consoleWriteArea);
    SetConsoleScreenBufferSize(hConsole, { (SHORT)resolution[0],(SHORT)resolution[1]});

    CONSOLE_FONT_INFOEX info = { 0 };
    info.cbSize = sizeof(info);
    info.dwFontSize.Y = 5; // leave X as zero
    info.FontWeight = FW_NORMAL;
    wcscpy_s(info.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, NULL, &info);
    SetConsoleActiveScreenBuffer(hConsole);

    DWORD dwBytesWritten = 0;
    while (1) {
        if (!bPaused) {
            this_thread::sleep_for(50ms);
            loadFrame();
            count++;
            if (bVideoOver)
                break;
            if (count % 2 == 0) {
                loadScreen();

                //Display Frame
                WriteConsoleOutputA(hConsole, consoleBuffer, cBufferSize, { 0,0 }, &consoleWriteArea);
            }
        }
        //keyboard
        keyboard();
    }
    CloseHandle(hConsole);
}

void Video::keyboard()
{
    for (int k = 0; k < 4; k++) {                           //space,pause,left,right
        bKey[k][0] = (0x8000 & GetAsyncKeyState((unsigned char)("\x20\x13"[k]))) != 0;
    }
    //space [pause video]
    if (bKey[0][0]) {
        if (!bKey[0][1]) {
            bPaused = !bPaused;
            bKey[0][1] = true;
        }
    }
    else {
        bKey[0][1] = false;
    }
    // stopWatching
    if (bKey[1][0]) {
        bVideoOver = true;
        bPaused = false;
    }
    //
}
