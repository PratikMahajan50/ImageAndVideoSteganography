#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "Stego.h"


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <ImagePath>\n",argv[0]);
        return -1;
    }
    cv::Mat img;

    img  = cv::imread(argv[1]);
    uint8_t* pData = (uint8_t*)img.data;
    int cn = img.channels();

    bool flag=false;
    int index=0;
    int sP = 0;int len=0,code,diff=0;
    int rf,ch;
    int r = img.rows -1,c = img.cols;

    //Retrieve Key from Image
    rf = pData[r*c*cn];
    ch = pData[r*c*cn+1];
    code = pData[r*c*cn+2];
    int dM = pData[r*c*cn+3];

    int j=4;
    while(dM--)
    {
        len*=10;
        len+=pData[r*c*cn+j];
        j++;
    } 
    len*=8;

    std::string bin;

    for(int i = 0; i < img.rows -1; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {   
            if(pData[i*img.cols*cn + j*cn + rf]==code)
            {
                if(pData[i*img.cols*cn + j*cn + ch]%2==0)
                    bin.push_back('0');
                else
                    bin.push_back('1');
                sP++;
                if(sP==len)
                    {flag=true;break;}
                
            } 
            
        }
        if(flag)
            break;
    }
    
    bin=StringToAscii(bin);
    std::cout<<bin;
    return 0;
}