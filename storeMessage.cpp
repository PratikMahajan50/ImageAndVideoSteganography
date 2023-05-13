#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <stdio.h>
#include "Stego.h"

int main(int argc, char** argv)
{
    Key key;
    if(argc != 3)
    {
        printf("Usage: %s <ImagePath> <MessageFilePath>\n",argv[0]);
        return -1;
    }
    
    
    cv::Mat image  = cv::imread(argv[1]);
    if(image.empty())
    {
        printf("Cant read image");
        return -1;
    }
    
    
    FILE *file = fopen(argv[2],"r");
    std::string mes;char c;
    while(1)
    {
        c=fgetc(file);
        if(feof(file)) break;
        mes+=c;
    }
    getChannels(&image,mes.length(), &key);
    key.mLen = mes.length();
    
    storeMessage(&image,&mes[0],&key);
    storeKey(&image,&key);
    cv::imwrite("Image.png",image);
    printf("Done\n");
    return 0;
}