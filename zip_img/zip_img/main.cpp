/*****************************************
*  Author: Root
*****************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "PicReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#define PI 3.14159
using namespace std;

void round_f(double org[64], int Q[64], int round_[64])
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            double r = org[8 * i + j] / Q[8 * i + j];
            round_[8 * i + j] = int(round(r));
        }
    }
}
double alpha(int i)
{
    if (i == 0) {
        return (1.0 / sqrt(8));
    }
    else
        return 1.0 / 2;
}

void tran_2(char p[])
{
    int length = strlen(p);
    //从右往左进行运算改变（拟实现2进制运算）
    for (int i = length; i > -1; i--) {
        if (p[i] == '0') {
            p[i] = '1';
            break;
        }
        if (p[i] == '1') {
            p[i] = '0';
        }
    }
}//进行2进制加法模拟

void plus_1(char p[])
{
    int length = strlen(p);
    //cerr << "8"<<"  "<<length << endl;
    p[length] = '0';
    p[length+1] = '\0';
}//在后续添加0

void str_fan(char p[], int length)
{
    for (int i = 0; i < length / 2; i++) {
        char a = p[i];
        p[i] = p[length - i - 1];
        p[length - i - 1] = a;
    }
}

void length_2_f(char p[],int n)
{
    int i = 0;
    if (n < 0) {
        n = -n;
        while ((n / 2) != 0)//将十进制数不断除2取余数，直到最后除2等于0，循环结束
        {
            p[i] = n % 2+48;//将十进制数除2后的余数存放在数组中
            i++;
            n /= 2;//不断改变n的值
           
        }
        p[i] = n+48;
        return;
    }
    if (n > 0) {
        while ((n / 2) != 0)//将十进制数不断除2取余数，直到最后除2等于0，循环结束
        {
            p[i] = n % 2+48;//将十进制数除2后的余数存放在数组中
            i++;
            n /= 2;//不断改变x的值

        }  
        p[i] = n+48;
        return;
    }
    if (n == 0) {
        p[0] = '0';
    }
}
const char Standard_DC_Luminance_NRCodes[] = { 0, 0, 7, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };//亮度Y
const unsigned char Standard_DC_Luminance_Values[] = { 4, 5, 3, 2, 6, 1, 0, 7, 8, 9, 10, 11 };

//-------------------------------------------------------------------------------
const char Standard_DC_Chrominance_NRCodes[] = { 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };//C
const unsigned char Standard_DC_Chrominance_Values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

//-------------------------------------------------------------------------------
const char Standard_AC_Luminance_NRCodes[] = { 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };
const unsigned char Standard_AC_Luminance_Values[] =
{
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
    0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
    0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
    0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
    0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
    0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
    0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
    0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

//-------------------------------------------------------------------------------
const char Standard_AC_Chrominance_NRCodes[] = { 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };
const unsigned char Standard_AC_Chrominance_Values[] =
{
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
    0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
    0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 
    0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
    0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
    0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
    0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
    0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
    0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

int QY[64] = { 16,11,10,16,24,40,51,61,
               12,12,14,19,26,58,60,55,
               14,13,16,24,40,57,69,56,
               14,17,22,29,51,87,80,62,
               18,22,37,56,68,109,103,77,
               24,35,55,64,81,104,113,92,
               49,64,78,87,103,121,120,101,
               72,92,95,98,112,100,103,99 };

int QC[64] = { 17,18,24,47,99,99,99,99,
               18,21,26,66,99,99,99,99,
               24,26,56,99,99,99,99,99,
               47,66,99,99,99,99,99,99,
               99,99,99,99,99,99,99,99,
               99,99,99,99,99,99,99,99,
               99,99,99,99,99,99,99,99,
               99,99,99,99,99,99,99,99 };

const char ZigZag[64] =
{
 0, 1, 5, 6,14,15,27,28,
 2, 4, 7,13,16,26,29,42,
 3, 8,12,17,25,30,41,43,
 9,11,18,24,31,40,44,53,
 10,19,23,32,39,45,52,54,
 20,22,33,38,46,51,55,60,
 21,34,37,47,50,56,59,61,
 35,36,48,49,57,58,62,63
};
int main(int argc, char* argv[]) {
    ///cerr << "**";
    if (argc != 3) {
        cerr << "Please make sure the number of parameters is correct." << endl;
        return -1;
    }

    if (!strcmp(argv[1], "-compress")) {
        
        //进行压缩步骤
        //huffman过程
        int num_value = 0;
        /*建AC_C的树*/
  
        char(*AC_C_TREE)[20] = new(nothrow) char[251][20]{ '\0' };//前者巧用下标法记录value，后者20记录01编码    
        if (AC_C_TREE == NULL) {
            cerr << "开辟出错";
            return -1;
        }
     
        char dode[50] = { '0' };
        num_value = 0;
        for (int i = 0; i < 16; i++) {
            int a = Standard_AC_Chrominance_NRCodes[i];
            while (a != 0) {
                strcpy(AC_C_TREE[Standard_AC_Chrominance_Values[num_value]], dode);
                tran_2(dode);
                num_value++;
                a--;
            }
            if (a == 0) {
                plus_1(dode);//需要末尾加尾0
            }
        }

        /*建AC_Y的树*/
        char(*AC_Y_TREE)[25] = new(nothrow) char[251][25]{ '\0' };
        //cerr << "44444";
        if (AC_Y_TREE[0] == NULL) {
            cerr << "开辟出错";
            return -1;
        }
        char bode[50] = { '0' };
        num_value = 0;
        for (int i = 0; i < 16; i++) {
            int a = Standard_AC_Luminance_NRCodes[i];
            while (a != 0) {
                strcpy(AC_Y_TREE[Standard_AC_Luminance_Values[num_value]], bode);
                tran_2(bode);
                num_value++;
                a--;
            }
            if (a == 0) {
                plus_1(bode);//需要末尾加尾0
            }
        }

      
        /*DC-Y的树*/
        char DC_Y_TREE[12][16] ;//前者巧用下标法记录value，后者16记录01编码
        char code[16] = {'0'};
        num_value = 0;
        for (int i = 0; i < 12; i++) {
            int a = Standard_DC_Luminance_NRCodes[i];
            //cerr << "0" << endl;
            while (a !=0) {                
                strcpy(DC_Y_TREE[Standard_DC_Luminance_Values[num_value]], code);
                tran_2(code);
                num_value++;
                a--;
            }
            if (a == 0) {
                plus_1(code);//需要末尾加尾0
            }
        }

        /*建DC_C的树*/
        char DC_C_TREE[12][16];//前者巧用下标法记录value，后者16记录01编码
        char aode[16] = { '0' };
        num_value = 0;
        for (int i = 0; i < 12; i++) {
            int a = Standard_DC_Chrominance_NRCodes[i];
            //cerr << "0" << endl;
            while (a != 0) { 
                strcpy(DC_C_TREE[Standard_DC_Chrominance_Values[num_value]], aode);
                tran_2(aode);
                num_value++;
                a--;
            }
            if (a == 0) {
                plus_1(aode);//需要末尾加尾0
            }
        }

        cerr << "begin"<<endl;
        PicReader imread;
        BYTE* data = nullptr;
        UINT x, y;

        
        imread.readPic(argv[2]);
        imread.getData(data, x, y);
        //x是列，y是行
        /*写文件头*/
        ofstream out("lena.jpg", ios::binary);
        if (!out.is_open()) {
            cout << "创建文件失败" << endl;
            return -1;
        }
        out << char(0xff)<<  char(0xd8) << char(0xff) << char(0xe0) <<  char(0x00)<<  char(0x10) << char(0x4A) <<  char(0x46) <<  char(0x49) <<  char(0x46) <<  char(0x00);
        out << char(0x01) <<  char(0x01) << char(0);//??
        out << char(0x00)<< char(0x01)<< char(0x00) << char(0x01) <<  char(0) <<  char(0);
        //打表
        out << char(0xFF)<< char(0xdb) << char(0x00)<< char(0x84);
        out << char(0x00);

        //zz型输出
        int Q_Y[64], Q_C[64];
        for (int i = 0; i < 64; i++) {
            Q_Y[ZigZag[i]] = QY[i];
            Q_C[ZigZag[i]] = QC[i];
        }
        for (int i = 0; i < 64; i++) {
            out << char(Q_Y[i]);
        }
        out << char(0x01);
        for (int i = 0; i < 64; i++) {
            out << char(Q_C[i]);
        }
        //开始扫描
        out <<  char(0xFF)<<  char(0xC0) <<  char(0x00)<< char(0x11) <<  char(0x08);
        out <<  char(0x02) <<  char(0x00)<<  char(0x02)<<  char(0x00);//图像高宽512的hex为200
        out <<  char(0x03)<<  char(0x01)<<  char(0x11) << char(0x00) <<  char(0x02) <<  char(0x11) << char(0x01) <<  char(0x03) << char(0x11) << char(0x01);
        //写Huffman树
        out <<  char(0xFF) <<  char(0xC4)<< char(0x01)<<char(0xA2);
        //111111直流亮度huffman树
        out<<  char(0x00);
        //直流0 huffman表
        for (int i = 0; i < 16; i++) {
            out << char(Standard_DC_Luminance_NRCodes[i]);
        }
        for (int i = 0; i < 12; i++) {
            out << char(Standard_DC_Luminance_Values[i]);
        }

        //22222交流亮度huffman树
        out<<char(0x10);
        //交流1 huffmanID 0
        for (int i = 0; i < 16; i++){
             out<< char(Standard_AC_Luminance_NRCodes[i]);
        }
        for(int i = 0; i < 162; i++) {
             out<< char(Standard_AC_Luminance_Values[i]);
        }

        //33333直流色度huffman树
        out<<  char(0x01);
        //直流0 huffmanID 1
        for (int i = 0; i < 16; i++) {
            out << char(Standard_DC_Chrominance_NRCodes[i]);
        }
        for (int i = 0; i < 12; i++) {
            out <<  char(Standard_DC_Chrominance_Values[i]);
        }
        //44444交流色度huffman树
        out << char(0x11);
        //交流1 huffmanID 1
        for (int i = 0; i < 16; i++) {
            out << char(Standard_AC_Chrominance_NRCodes[i]);
        }
        for (int i = 0; i < 162; i++) {
            out <<char(Standard_AC_Chrominance_Values[i]);
        }

        out <<  char(0xFF) <<  char(0xDA) <<  char(0x00) <<  char(0x0C) << char(0x03);
        out<<  char(0x01)<<  char(0x00)<<  char(0x02)<<  char(0x11)<<  char(0x03)<<  char(0x11);
        out << char(0x00) << char(0x3F) <<  char(0x00);
        /*写文件头结束*/
       
        string str;
        int Y_DC_LAST = 0, CB_DC_LAST = 0, CR_DC_LAST = 0;
        /*分区域小模块处理*/
        for (int p = 0; p < int(x); p += 8) {//列
            for (int q = 0; q < int(y); q += 8) {//行(总的大循环)
                /*再对每一小块进行转换*/
                double img_Y[64] = { 0 };
                double img_Cb[64] = { 0 };
                double img_Cr[64] = { 0 };
                int n = 0; 
                for (int a = p; a < p + 8; a++) {
                    for (int b = q; b < q + 8; b++) {
                        int i_data = a*x*4+b*4;
                        img_Y[n] = 0.299 * data[i_data]+0.5870 * data[i_data+1]+0.114 * data[i_data+2]  -128;
                        img_Cb[n] = -0.1687 * data[i_data] -0.3313 * data[i_data + 1] + 0.50000 * data[i_data + 2];
                        img_Cr[n]= 0.50000 * data[i_data] - 0.4187 * data[i_data + 1] - 0.0813 * data[i_data + 2];
                        n++;
             
                    }                   
                }
               
                double alpha_u = 0, alpha_v = 0;
                double Y_new[64] = { 0 }, Cb_new[64] = { 0 }, Cr_new[64] = { 0 };
                for (int u = 0; u < 8; u++) {
                    for (int v = 0; v < 8; v++) {
                        alpha_u = alpha(u);
                        alpha_v = alpha(v);
                        double F_Y = 0,F_Cr=0,F_Cb=0;
                        double sum_Y = 0,sum_Cr=0,sum_Cb=0;
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 8; j++) {
                                sum_Y += img_Y[8 * i + j] * cos((2 * i + 1) * u * PI / 16) * cos((2 * j + 1) * v * PI/16);
                                sum_Cr += img_Cr[8 * i + j] * cos((2 * i + 1) * u * PI / 16) * cos((2 * j + 1) * v * PI/16);
                                sum_Cb += img_Cb[8 * i + j] * cos((2 * i + 1) * u * PI / 16) * cos((2 * j + 1) * v * PI/16);
                            }
                        }
                        F_Y = alpha_u * alpha_v * sum_Y;
                        F_Cr = alpha_u * alpha_v * sum_Cr;
                        F_Cb = alpha_u * alpha_v * sum_Cb;

                        Y_new[8 * u + v] = F_Y;
                        Cb_new[8 * u + v] = F_Cb;
                        Cr_new[8 * u + v] = F_Cr;
                    }
                }

                int roundY[64] = { 0 },roundCb[64] = { 0 }, roundCr[64] = { 0 };             
                round_f(Y_new, QY, roundY);
                round_f(Cr_new, QC, roundCr);
                round_f(Cb_new, QC, roundCb);
         
                
                //zigzag处理               
                int Y_ZZ[64] = {0}, Cr_ZZ[64] = {0}, Cb_ZZ[64] = {0};
                for (int j = 0; j < 64; j++) {
                    Y_ZZ[int(ZigZag[j])] = roundY[j];
                    Cr_ZZ[int(ZigZag[j])] = roundCr[j];
                    Cb_ZZ[int(ZigZag[j])] = roundCb[j];
                }

                //RLE编码
                char rle_code_y[1000] = { 0 }, rle_code_cr[1000] = { 0 }, rle_code_cb[1000] = { 0 };
                int num_16 = 0,num_0=0,length_2=0;//num_0记录0的个数,length_2记录字符串长度
                
                //Y操作
                for (int i = 0; i < 64; i++) {
                    num_0 = 0, num_16 = 0, length_2 = 0;
                    char ycode[12] = { 0 };
                    if (i == 0) {//对于dc数据的特殊处理
                        int Y_DC = Y_ZZ[0] - Y_DC_LAST;//先作差
                        Y_DC_LAST = Y_ZZ[0];//保留此次的下次需要做减法
                        if (Y_DC == 0) {
                            strcpy(rle_code_y, "110");
                        }
                        else {
                            num_0 = 0;

                            length_2_f(ycode, Y_DC);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                            length_2 = strlen(ycode);//求出长度，解决负号
                            if (Y_DC < 0) {
                                for (int n = 0; n < length_2; n++) {
                                    if (ycode[n] == '1')
                                        ycode[n] = '0';
                                    else
                                        ycode[n] = '1';
                                }
                            }

                            str_fan(ycode, length_2);
                            num_16 = num_0 * 16 + length_2;
                            strcat(rle_code_y, DC_Y_TREE[num_16]);
                            strcat(rle_code_y, ycode);
                        }
                        
                    }
                    if (i != 0) {//对于Ac数据的处理
                        if (Y_ZZ[i] != 0) {//前面没0
                           
                            length_2_f(ycode, Y_ZZ[i]);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                            length_2 = strlen(ycode);
                            if (Y_ZZ[i] < 0) {
                                for (int n = 0; n < length_2; n++) {
                                    if (ycode[n] == '1')
                                        ycode[n] = '0';
                                    else
                                        ycode[n] = '1';
                                }
                            }
                            str_fan(ycode, length_2);
                            num_16 = num_0 * 16 + length_2;
                            strcat(rle_code_y, AC_Y_TREE[num_16]);
                            strcat(rle_code_y, ycode);
                        }
                        if (Y_ZZ[i] == 0) {//前面有0
                            
                            int n = i;
                            while (Y_ZZ[n] == 0) {
                                num_0++;
                                n++;
                                i++;
                                if (num_0 >= 15)
                                    break;
                            }//while结束后，指向一个非0数的
                            if (num_0 >=15) {
                                strcat(rle_code_y, "1010");//y拷贝1010，c拷贝00
                                break;
                            }
                            else {
                                length_2_f(ycode, Y_ZZ[i]);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                                length_2 = strlen(ycode);
                                if (Y_ZZ[i] < 0) {
                                    for (int n = 0; n < length_2; n++) {
                                        if (ycode[n] == '1')
                                            ycode[n] = '0';
                                        else
                                            ycode[n] = '1';
                                    }
                                }
                                str_fan(ycode, length_2);
                                num_16 = num_0 * 16 + length_2;
                                strcat(rle_code_y, AC_Y_TREE[num_16]);
                                strcat(rle_code_y, ycode);
                            }
                            
                        }                       
                    }                  
                }
                //y结束

                // cb的开始
                for (int i = 0; i < 64; i++) {
                    num_0 = 0, num_16 = 0, length_2 = 0;
                    char cbcode[12] = { 0 };
                    //cerr << CB_DC_LAST << "  " << i << "  " << Cb_ZZ[0] << "  " << Cb_ZZ[1] << "  " << Cb_ZZ[2] << endl;
                    if (i == 0) {//对于dc数据的特殊处理
                        int cb_DC = Cb_ZZ[i] - CB_DC_LAST;//先作差
                        CB_DC_LAST = Cb_ZZ[i];//保留此次的下次需要做减法
                        if (cb_DC == 0) {
                            strcpy(rle_code_cb, "00");
                        }
                        else {
                            num_0 = 0;
                            length_2_f(cbcode, cb_DC);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                            length_2 = strlen(cbcode);//求出长度，解决负号
                            if (cb_DC < 0) {
                                for (int n = 0; n < length_2; n++) {
                                    if (cbcode[n] == '1')
                                        cbcode[n] = '0';
                                    else
                                        cbcode[n] = '1';
                                }
                            }

                            str_fan(cbcode, length_2);
                            num_16 = num_0 * 16 + length_2;
                            strcat(rle_code_cb, DC_C_TREE[num_16]);
                            strcat(rle_code_cb, cbcode);
                        }
                        
                    }
                    if (i != 0) {//对于Ac数据的处理
                        if (Cb_ZZ[i] != 0) {//前面没0                            
                            length_2_f(cbcode, Cb_ZZ[i]);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                            length_2 = strlen(cbcode);
                            if (Cb_ZZ[i] < 0) {
                                for (int n = 0; n < length_2; n++) {
                                    if (cbcode[n] == '1')
                                        cbcode[n] = '0';
                                    else
                                        cbcode[n] = '1';
                                }
                            }
                            str_fan(cbcode, length_2);
                            num_16 = num_0 * 16 + length_2;
                            strcat(rle_code_cb, AC_C_TREE[num_16]);
                            strcat(rle_code_cb, cbcode);
                        }
                        if (Cb_ZZ[i] == 0) {//前面有0
                            int n = i;
                            while (Cb_ZZ[n] == 0) {
                                num_0++;
                                n++;
                                i++;
                                if (num_0 >=15)
                                    break;
                            }//while结束后，指向一个非0数的
                            if (num_0 >=15) {
                                strcat(rle_code_cb, "00");//y拷贝1010，c拷贝00
                                break;
                            }
                            else {
                                length_2_f(cbcode, Cb_ZZ[i]);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                                length_2 = strlen(cbcode);
                                if (Cb_ZZ[i] < 0) {
                                    for (int n = 0; n < length_2; n++) {
                                        if (cbcode[n] == '1')
                                            cbcode[n] = '0';
                                        else
                                            cbcode[n] = '1';
                                    }
                                }
                                str_fan(cbcode, length_2);
                                num_16 = num_0 * 16 + length_2;
                                strcat(rle_code_cb, AC_C_TREE[num_16]);
                                strcat(rle_code_cb, cbcode);
                            }

                        }
                    }
                }
                //cb序列化结束
               
                 //cr开始
                for (int i = 0; i < 64; i++) {
                    num_0 = 0, num_16 = 0, length_2 = 0;
                    char crcode[12] = { 0 };
                    if (i == 0) {//对于dc数据的特殊处理
                        int CR_DC = Cr_ZZ[0] - CR_DC_LAST;//先作差
                        CR_DC_LAST = Cr_ZZ[0];//保留此次的下次需要做减法
                        if (CR_DC == 0) {
                            strcpy(rle_code_cr, "00");
                        }
                        else {
                            num_0 = 0;
                            length_2_f(crcode, CR_DC);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                            length_2 = strlen(crcode);//求出长度，解决负号
                            if (CR_DC < 0) {
                                for (int n = 0; n < length_2; n++) {
                                    if (crcode[n] == '1')
                                        crcode[n] = '0';
                                    else
                                        crcode[n] = '1';
                                }
                            }
                            str_fan(crcode, length_2);
                            num_16 = num_0 * 16 + length_2;
                            strcat(rle_code_cr, DC_C_TREE[num_16]);
                            strcat(rle_code_cr, crcode);
                        }
                        
                    }
                    if (i != 0) {//对于Ac数据的处理
                        if (Cr_ZZ[i] != 0) {//前面没0
                            length_2_f(crcode, Cr_ZZ[i]);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                            length_2 = strlen(crcode);
                            if (Cr_ZZ[i] < 0) {
                                for (int n = 0; n < length_2; n++) {
                                    if (crcode[n] == '1')
                                        crcode[n] = '0';
                                    else
                                        crcode[n] = '1';
                                }
                            }
                            str_fan(crcode, length_2);
                            num_16 = num_0 * 16 + length_2;
                            strcat(rle_code_cr, AC_C_TREE[num_16]);
                            strcat(rle_code_cr, crcode);
                        }
                        if (Cr_ZZ[i] == 0) {//前面有0
                            int n = i;
                            while (Cr_ZZ[n] == 0) {
                                num_0++;
                                n++;
                                i++;
                                if (num_0>=15)
                                    break;
                            }//while结束后，指向一个非0数的
                            if (num_0 >=15) {
                                strcat(rle_code_cr, "00");//y拷贝1010，c拷贝00
                                break;
                            }
                            else {
                                length_2_f(crcode, Cr_ZZ[i]);//现在的ycode是反的//若Y_DC是负数还需要换01；                   
                                length_2 = strlen(crcode);
                                if (Cr_ZZ[i] < 0) {
                                    for (int n = 0; n < length_2; n++) {
                                        if (crcode[n] == '1')
                                            crcode[n] = '0';
                                        else
                                            crcode[n] = '1';
                                    }
                                }
                                str_fan(crcode, length_2);
                                num_16 = num_0 * 16 + length_2;
                                strcat(rle_code_cr, AC_C_TREE[num_16]);
                                strcat(rle_code_cr, crcode);
                            }

                        }
                    }
                }
                //cr结束

                str += rle_code_y;
                str += rle_code_cb;
                str += rle_code_cr;
            }
        }//至此是8*8小块操作完毕
        for (unsigned int i = 0; i < str.length();) {
            unsigned char a = 0, c = 0;
            for (unsigned int m = i,n=0; m < i+8; m++) {
                if (str[m] != '\0') {
                    a = 0;
                    a |= ((str[m] - '0') << (7 - n));
                    c |= a;     
                    n++;
                }
                if (m >= str.length())
                    break;
            }
            
            out << c;
            if (c == unsigned char(0xff)) {
                out << char(0x00);
       
            }
            i += 8;
            if (i >= str.length() - 1) {
                break;
            }
        }
        out <<char(0xFF) <<char(0xD9);
        delete[] AC_C_TREE;
        delete[] AC_Y_TREE;
        delete[] data;
        data = nullptr;
        out.close();//关闭文件
    }

    if (!strcmp(argv[1], "-read")) {
        //展示图像PicReader imread;
        PicReader imread;
        BYTE* data = nullptr;
        UINT x, y;
        imread.readPic(argv[2]);
        imread.getData(data, x, y);
        imread.showPic(data, x, y);
        data = nullptr;
        delete[] data;
   }
    cerr << "complete!!!";
    return 0;
}


