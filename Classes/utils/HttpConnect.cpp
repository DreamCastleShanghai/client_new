//
//  HttpConnect.cpp
//  SapSession
//
//  Created by csh on 15/12/10.
//
//

#include "HttpConnect.h"

#define QRCODE_TEXT					"http://www.ultramundum.org/index.htm";		// Text to encode into QRCode
#define OUT_FILE					"Resources/test.bmp"								// Output file name
#define OUT_FILE_PIXEL_PRESCALER	8											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

#define PIXEL_COLOR_R				0x0											// Color of bmp pixels
#define PIXEL_COLOR_G				0xff
#define PIXEL_COLOR_B				0xff

// BMP defines

typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;

string getSign(std::string value)
{
    string appsecret = "c174cb1fda3491285be953998bb867a0";
    string tempStr = appsecret+value+appsecret;
    string sign = MD5(tempStr).md5();
    for(int i=0;i<sign.length();i++)
    {
        if(sign[i]>='a'&&sign[i]<='z')
            sign[i]-=32;
        else if
            (sign[i]>='A'&&sign[i]<='Z')sign[i]+=32;
    }
    return sign;
}

string getSign(std::map<std::string,std::string> key_value)
{
    string appsecret = "c174cb1fda3491285be953998bb867a0";
    string tempStr = "";
    std::map<std::string,std::string>::iterator itr;
    for (itr=key_value.begin(); itr!=key_value.end(); itr++) {
        tempStr = tempStr+itr->first+itr->second;
    }
    tempStr = appsecret+tempStr+appsecret;
    CCLog("tempStr===%s",tempStr.c_str());
    string sign = MD5(tempStr).md5();
    for(int i=0;i<sign.length();i++)
    {
        if(sign[i]>='a'&&sign[i]<='z')
            sign[i]-=32;
        else if
            (sign[i]>='A'&&sign[i]<='Z')sign[i]+=32;
    }
    return sign;
}

CAImage* createQRImage(std::string code, int width, int height)
{
    CAImage* image = NULL;
//
//    QRcode* _qrcode = QRcode_encodeString(code.c_str(), 2, QR_ECLEVEL_L, QR_MODE_8, 0);
//    if (_qrcode != nullptr) {
//        int wScale = width / _qrcode->width, hScale = height / _qrcode->width;
//        unsigned char*	pData;
//        int length = wScale * _qrcode->width * hScale * _qrcode->width;
//        if (!(pData = (unsigned char*)malloc(length)))
//        {
//            CCLog("Out of memory");
//            QRcode_free(_qrcode);
//            return NULL;
//        }
//        memset(pData, 0xff, length);
//        
//        for (int i =0; i<_qrcode->width; ++i)
//        {
//            for (int j = 0; j<_qrcode->width; ++j)
//            {
//                if(_qrcode->data[i * _qrcode->width +j] & 1)
//                {
//                    for (int n = 0; n < hScale; n++)
//                    {
//                        for (int m = 0; m < wScale; m++)
//                        {
//                            pData[i * _qrcode->width * hScale * wScale + n * _qrcode->width * wScale + j * wScale + m] = 0;
//                        }
//                        
//                    }
//
//                }
//                else
//                {
//                    for (int n = 0; n < hScale; n++)
//                    {
//                        for (int m = 0; m < wScale; m++)
//                        {
//                            pData[i * _qrcode->width * hScale * wScale + n * _qrcode->width * wScale + j * wScale + m] = 255;
//                        }
//                        
//                    }
//                }
//            }
//        }
//        
//        image = CAImage::createWithRawDataNoCache(pData, CrossApp::CAImage::PixelFormat_A8, wScale * _qrcode->width, hScale * _qrcode->width);
//        QRcode_free(_qrcode);
//        free(pData);
//    }
    return image;
}