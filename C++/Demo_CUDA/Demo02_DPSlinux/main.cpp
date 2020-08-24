#include <iostream>
#include "time.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "FSC_FitsX.h"

using namespace std;

int main()
{
    const char *imgPath = "/home/frozensky/Codes/testDPS_linux/img.fits";

    // Timer timer0
    clock_t timer0 = clock();

    // Load fits file
    FSC_FitsX fscFitsX;
    fscFitsX.OpenFitsFile(imgPath);

    // Timer timer1
    clock_t timer1 = clock();
    double timer_LoadFile = (double)(timer1 - timer0) / CLOCKS_PER_SEC;
    printf("Time (Load fits file): %f seconds\n", timer_LoadFile);

    // Extract stars
    cv::Mat cvMat8U;


    // List fits header
    for (int i = 0; i < fscFitsX.GetHDUNum(); i++)
    {
        printf("%-8s = %s\r\n", fscFitsX.GetHDUKey(i).c_str(), fscFitsX.GetHDUValue(i).c_str());
    }

    return true;
}