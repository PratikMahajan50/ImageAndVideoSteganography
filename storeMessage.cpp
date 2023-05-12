#include<opencv2/opencv.hpp>
//#include <cstdlib>
#include <bits/stdc++.h>
#include<stdio.h>

/*
Algorithm

1. Get the Image
2. Get the message
3. Choose the reference channel and change channel
    a. Depending on the length of the message and the count of the value(0-255) of 3 channels
    b. change channel could be random between rest of the 2 channels
    c. store both the channel characters in the Key String
4. Determine the start index(random), end index and differentiator and store it in the keystring
5. Store the message in the image
6. output the image preview and save.


*/

struct Key
{
    char refCh,chgCh;uint8_t code; int mLen; 
} key;

void getChannels(cv::Mat* img, int len)    //Returns 2 character string containing reference channel and change channel
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
    
    for(int i=0;i<256;i++)
    {
        if(maxB<counts[0][i])
        {
            maxB=counts[0][i];
            vB=i;            
        }
        if(maxG<counts[1][i])
        {
            maxG=counts[1][i];
            vG=i;            
        }
        if(maxR<counts[2][i])
        {
            maxR=counts[2][i];
            vR=i;            
        }
    }
    //Find Max 
    
    int max;
    if(maxB>=maxG && maxB>=maxR)
    {
        key.refCh='B';
        key.chgCh= (rand()%2)?'G':'R';
        key.code=(uint8_t)vB;
        max=maxB;
    }
    else if(maxG>=maxB && maxG>=maxR)
    {
        key.refCh='G';
        key.chgCh= (rand()%2)?'B':'R';
        key.code=(uint8_t)vG;
        max=maxG;
    }
    else
    {
        key.refCh='R';
        key.chgCh= (rand()%2)?'B':'G';
        key.code=(uint8_t)vR;
        max=maxR;
    }
    
    
    if(len>(max/8))
    {
        printf("\nMessage Length is too long");
        exit(-1);
    }
    
    printf("\nReference Channel:  %c",key.refCh);
    printf("\nChange Channel:  %c",key.chgCh);
    printf("\nReference Code Number:  %u",key.code);
    
    printf("\nMax storable character: : %d",max/8);
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
std::string appendZeros(std::string bin)
{
    int len = bin.length();
    //printf("%d  ",len);
    if(bin.length()==8)
        return bin;
    for(int i=0;i<8-len;i++)
    {
        bin.insert(0,"0");
    }
    return bin;
}
std::string stringToBinary(std::string mes)
{
    int n = mes.length(),num;
    std::string fString ="";
    std::string bin;
    for(int i=0;i<n;i++)
    {
        num = int(mes[i]);
        //printf("%d\n",num);
        bin="";
        while(num>0)
        {
            (num%2)?bin.push_back('1'):bin.push_back('0');
            num/=2;
        }
        reverse(bin.begin(),bin.end());
        bin = appendZeros(bin);
        //std::cout<<bin<<std::endl;
        fString+=bin;
    }
    std::cout<<fString[0]<<std::endl;
        
    return fString;
}
int bTod(std::string n)
{
    std::string num=n;
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
std::string StringToAscii(std::string str)
{
    int n=int(str.size());
    std::string res="";
    for(int i=0;i<n;i+=8)
    {
        int d = bTod((str.substr(i,8)));
        res+=char(d);
    }
    return res;
}
void storeMessage(cv::Mat* img,char* message)
{
    bool flag=false;
    //int sP = 0,ind=0;
    int rchI=getChannelId(key.refCh),cchI=getChannelId(key.chgCh);

    //
    FILE *file1 = fopen("store.txt","w");
    
    std::string mes = stringToBinary(message);
    message = &mes[0];
    std::cout<<message;
    //std::cout<<"String length after converting to binary: "<<std::string(message).length();
    //std::string newM = StringToAscii(message);
    //std::cout<<newM;
    //fprintf(file1,message);
    uint8_t* pData = (uint8_t*) img->data;
    int cn = (*img).channels();
    int k=0;
    int len = std::string(message).length();
    std::cout<<"\n\n"<<len<<"\n";
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
                        fprintf(file1,"0");
                    }
                    else
                    {
                        if(pData[i*img->cols*cn + j*cn + cchI]%2==0)
                            pData[i*img->cols*cn + j*cn + cchI]+=1;
                        fprintf(file1,"1");

                    }
                    k++;
                    //sP++;
                    if(k==len)
                    {flag=true;break;}
                //ind++;
            } 
            
        }
        if(flag)
            break;
    }
    fclose(file1);
    
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
        printf("\n%u",pData[r*c*cn + i]);
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
    
    //1.
    cv::Mat image  = cv::imread(argv[1]);
    if(image.empty())
    {
        printf("Cant read image");
        return -1;
    }
    
    //2.
    FILE *file = fopen(argv[2],"r");
    std::string mes;char c;
    while(1)
    {
        c=fgetc(file);
        if(feof(file)) break;
        mes+=c;
    }
    std::cout<<mes;

    //3.i
    getChannels(&image,mes.length());
    key.mLen = mes.length();
    //std::cout<<mes;
    storeMessage(&image,&mes[0]);
    printf("Length: %d",(int)mes.length());
    storeKey(&image);
    cv::imwrite("Image.png",image);
    
    //generateKey();
    return 0;
}