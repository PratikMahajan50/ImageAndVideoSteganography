#include<opencv2/opencv.hpp>
#include<stdio.h>

using namespace cv;
using namespace std;
int retCount(int num)
{
    int n= num,c=0;
    while(num>0)
    {
        num=num/10;
        c++;
    }
    return c;
}
int bTod(string n)
{
    string num=n;
    int d=0;
    int base=1;
    int len=num.length();
    for(int i=len-1;i>=0; i--)
    {
        if(num[i]=='1')
            d+=base;
        base*=2;
    }
    return d;
}
string StringToAscii(string str)
{
    int n=int(str.size());
    string res="";
    for(int i=0;i<n;i+=8)
    {
        int d = bTod((str.substr(i,8)));
        res+=char(d);
    }
    return res;
}
int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <ImagePath>\n",argv[0]);
        return -1;
    }
    Mat img;

    img  = imread(argv[1]);
    uint8_t* pData = (uint8_t*)img.data;
    int cn = img.channels();

    bool flag=false;
    int index=0;
    int sP = 0;int len=0,pVal,diff=0;
    int rf,ch;
    int r = img.rows -1,c = img.cols;

    rf = pData[r*c*cn];
    ch = pData[r*c*cn+1];
    pVal = pData[r*c*cn+2];
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
            if(pData[i*img.cols*cn + j*cn + rf]==pVal)
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
    

    FILE *file = fopen("store.txt","r");
    std::string mes;char ct;
    while(1)
    {
        ct=fgetc(file);
        if(feof(file)) break;
        mes+=ct;
    }

    bin=StringToAscii(bin);
    std::cout<<bin;
    return 0;
}