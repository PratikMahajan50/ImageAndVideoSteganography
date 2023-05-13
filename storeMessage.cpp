#include<opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include<stdio.h>


struct Key
{
    char refCh,chgCh;uint8_t code; int mLen; 
} key;
struct eVals
{
    char ch;uint8_t code;
};
/*
void getChannels(cv::Mat* img, int len):

This method finds out the reference channel and change channel. This method scans the image pixel by pixel through BGR color 
channels and stores the frequency of each color code from 0 to 255 of each color channel. Then finds out all the codes whose 
counts are greater than 8 times the message length and stores in a vector<refChannel,code>.If the vector is empty then it 
means that we cannot store the messsage in this image. Then we choose a random value from this vector which will become the 
refrance channel and the target color code. Then randomly select the change channel from remaining channel.
*/
void getChannels(cv::Mat* img, int len)
{
    int maxB=0,maxG=0,maxR=0,vB,vG,vR;
    int counts[3][256];
    for(int i=0;i<256;i++)
    {
        counts[0][i]=0;     //B
        counts[1][i]=0;     //G
        counts[2][i]=0;     //R
    }
    
    uint8_t* pData = (uint8_t*) img->data;
    int cn = (*img).channels();
    
    for(int i = 0; i < (*img).rows -1; i++)
    {
        for(int j = 0; j < (*img).cols; j++)
        {
            counts[0][(int)pData[i*(*img).cols*cn + j*cn + 0]]++; //B
            counts[1][(int)pData[i*(*img).cols*cn + j*cn + 1]]++; //G
            counts[2][(int)pData[i*(*img).cols*cn + j*cn + 2]]++; //R
        }
    }
    
    std::vector<eVals> v;
    int r=0;
    for(int i=0;i<256;i++)
    {
        if(counts[0][i]>=len*8)
        {
            v.push_back((eVals){'B',(uint8_t)i});
            r++;
        }
        if(counts[1][i]>=len*8)
        {
            v.push_back((eVals){'G',(uint8_t)i});
            r++;
        }
        if(counts[2][i]>=len*8)
        {
            v.push_back((eVals){'R',(uint8_t)i});
            r++;
        }
    }
    if(r==0)
    {
        printf("\nMessage Length is too long and cannot be embbeded in the image, select a different image");
        exit(-1);
    }
    
    //Choose a random from above
    srand(time(0));
    r = rand()%r;
    key.refCh=v[r].ch;
    key.code=v[r].code;

    if(key.refCh=='B'){key.chgCh=(rand()%2)?'G':'R';}
    else if(key.refCh=='G'){key.chgCh=(rand()%2)?'B':'R';}
    else  {key.chgCh=(rand()%2)?'B':'G';}
    
}
int getChannelId(char c)
{
    switch(c)
    {
        case 'B':return 0;
        case 'G':return 1;
        case 'R':return 2; 
    }
    return -1;
}

/*
std::string appendZeros(std::string bin):

After conversion of the characters to binary values, at times the length of the generated binary string is less than 8. So to 
balance it we append preceeding zeros.
*/
std::string appendZeros(std::string bin)
{
    int len = bin.length();
    if(bin.length()==8)
        return bin;
    for(int i=0;i<8-len;i++)
    {
        bin.insert(0,"0");
    }
    return bin;
}

/*
std::string stringToBinary(std::string mes):

Converts the message string to binary values and returns in string format of 0s and 1s.
*/
std::string stringToBinary(std::string mes)
{
    int n = mes.length(),num;
    std::string fString ="";
    std::string bin;
    for(int i=0;i<n;i++)
    {
        num = int(mes[i]);
        bin="";
        while(num>0)
        {
            (num%2)?bin.push_back('1'):bin.push_back('0');
            num/=2;
        }
        reverse(bin.begin(),bin.end());
        bin = appendZeros(bin);
        fString+=bin;
    }
    return fString;
}

/*
void storeMessage(cv::Mat* img,char* message):

This methods stores the message in the image. Firtsly converts the string to binary. Then scans the image pixel by pixel.
If the color code of the reference channel matches the calculated color code, then make the change in the color code of 
the change channel. Following is the logic to append the data using lsb technique
    - If the next value of the message to be store is 0 then we check if the color code that we are operating on is even 
      or odd. If it is odd than do -1 to make it even and thus the lsb bit becomes 0.
    - same if the next value of the message is 1, then we check if the color code we are operating is even or odd. If is 
      even, then add 1 to make the lsb bit 1.
*/
void storeMessage(cv::Mat* img,char* message)
{
    bool flag=false;
    
    int rchI=getChannelId(key.refCh),cchI=getChannelId(key.chgCh);
    
    std::string mes = stringToBinary(message);
    message = &mes[0];
    uint8_t* pData = (uint8_t*) img->data;
    int cn = (*img).channels();
    int k=0;
    int len = std::string(message).length();
    for(int i = 0; i < img->rows -1; i++)
    {
        for(int j = 0; j < img->cols; j++)
        {
            if(pData[i*img->cols*cn + j*cn + rchI]==key.code)
            {
                    if(message[k]=='0')
                    {
                        if(pData[i*img->cols*cn + j*cn + cchI]%2==1)
                            pData[i*img->cols*cn + j*cn + cchI]-=1;
                    }
                    else
                    {
                        if(pData[i*img->cols*cn + j*cn + cchI]%2==0)
                            pData[i*img->cols*cn + j*cn + cchI]+=1;
                    }
                    k++;
                    
                    if(k==len)
                    {flag=true;break;}
                
            } 
            
        }
        if(flag)
            break;
    }
    
    
}
char* encMessage(char* message);

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
/*
void storeKey(cv::Mat *img):

This method stores the genearted key in the last row of the Image.
*/
void storeKey(cv::Mat *img)
{
    // RF CH Code 
    // lL lD 
    uint8_t* pData = (uint8_t*) img->data;int r = img->rows -1,c = img->cols;
    int cn = (*img).channels();
    pData[r*c*cn] = (uint8_t)getChannelId(key.refCh);
    pData[r*c*cn + 1]=(uint8_t)getChannelId(key.chgCh);
    pData[r*c*cn + 2]= key.code;
    pData[r*c*cn + 3] = (uint8_t)retCount(key.mLen);
    
    int i=4,j=0;
    std::string num = std::to_string(key.mLen);
    
    while(j<pData[r*c*cn + 3])
    {
        pData[r*c*cn + i]=((uint8_t)num[j])-48;
        i++;j++;
    }

}


int main(int argc, char** argv)
{
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
    getChannels(&image,mes.length());
    key.mLen = mes.length();
    
    storeMessage(&image,&mes[0]);
    storeKey(&image);
    cv::imwrite("Image.png",image);
    printf("Done\n");
    return 0;
}