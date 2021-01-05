#include "CNN_net.h"
#include "face_binary_cls1.h"
#include <opencv2/opencv.hpp>
#include <chrono>
using namespace cv;

int main()
{
    //"face_binary_cls1.h"中对应的训练好的网络权重
    float *Weight1 = conv0_weight;
    float *Bias1 = conv0_bias;
    float *Weight2 = conv1_weight;
    float *Bias2 = conv1_bias;
    float *Weight3 = conv2_weight;
    float *Bias3 = conv2_bias;
    float *Weight_fc = fc0_weight;
    float *Bias_fc = fc0_bias;

    CNN_Pool cnnPool(2, 2);               //定义一个窗口大小为2*2的池化层
    CNN_Conv cnnConv1(16, 3, 3, 3, 2, 1); //定义第一层卷积层 参数分别是 chanel数、图片颜色层数、kernel高、kernel宽、stride、padding
    cnnConv1.loadWeight(Weight1, Bias1);  //读入对应网络权重

    CNN_Conv cnnConv2(32, 16, 3, 3, 1, 0);
    cnnConv2.loadWeight(Weight2, Bias2);

    CNN_Conv cnnConv3(32, 32, 3, 3, 2, 1);
    cnnConv3.loadWeight(Weight3, Bias3);

    CNN_Fc cnnFc(2048, 2);
    cnnFc.loadWight(Weight_fc, Bias_fc);

    auto start = std::chrono::steady_clock::now();
    Mat imageBGR = imread("bg.jpg"); //read a picture
    Mat imageRGB(imageBGR.rows, imageBGR.cols, CV_8UC3); //建立一个3通道图像
    for (int i = 0; i < imageBGR.rows; ++i) //Convert BGR to RGB
	{ 
		//获取第i行首像素指针
		Vec3b *p1 = imageBGR.ptr<Vec3b>(i); 
		Vec3b *p2 = imageRGB.ptr<Vec3b>(i);
		for(int j=0; j<imageBGR.cols; ++j) 
		{ 
			//将img的bgr转为image的rgb 
			p2[j][2] = p1[j][0]; 
			p2[j][1] = p1[j][1]; 
			p2[j][0] = p1[j][2]; 
		}
    }
    unsigned char imageRGBArray[3*imageRGB.rows*imageBGR.cols];
    for(int i = 0; i<imageRGB.rows; i++)
    {
        for(int j = 0; j < imageRGB.cols; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                imageRGBArray[i*imageRGB.rows*3 + j * 3 + k] = imageRGB.at<Vec3b>(i,j)[k];
            }
        }
    }
    float image[3*imageRGB.rows*imageBGR.cols]; 
    for(int i = 0; i < 3*imageRGB.rows*imageBGR.cols; i++)
    {
        image[i] = ((float) imageRGBArray[i])/255.0f; //Casting an array of unsigned chars to an array of floats and normalize
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "duration for image operation = " << duration << "ms" << endl;
    // cout << imageRGB.at<Vec3b>(imageRGB.rows-1,imageRGB.cols-1-1)[2] << "<-imageRGBArray data" << endl;
    // cout << imageRGBArray[3*imageRGB.rows*imageBGR.cols-1-1] << "<-imageRGBArray data" << endl;
    // cout << image[3*imageRGB.rows*imageBGR.cols-1] << "<-image data" << endl;
    // cout << imageRGB.rows << " " <<imageBGR.cols << endl;

    /*float image[1 * 3 * 128 * 128] = {0.01, 0.021, 0.051, 0.1}; //此处模拟读入图片数据为一个batchsize*图片颜色层数(顺序为RGB)*高*宽的一维float数组*/

    auto start1 = std::chrono::steady_clock::now();
    float *conv_result1 = cnnConv1.cnnForwardConv(image, 1, 3, 128, 128);      //传入的参数是image数组，batchsize,图片颜色层数(顺序为RGB),高,宽
    float *pool_result1 = cnnPool.cnnForwardMaxpool(conv_result1, 16, 64, 64); //经过第一层卷积如果padding=2,stride=1,得到一个channels(16)*高(64)*宽(64)的特征图，对其池化
    float *conv_result2 = cnnConv2.cnnForwardConv(pool_result1, 1, 16, 32, 32);
    float *pool_result2 = cnnPool.cnnForwardMaxpool(conv_result2, 32, 30, 30);
    float *conv_result3 = cnnConv3.cnnForwardConv(pool_result2, 1, 32, 15, 15); //最终得到的是一个32*32*32的一维float数组(stride=1,padding=1;如果stride=2,padding=1为32*8*8)
    float *fc_result = cnnFc.cnnForwardFc(conv_result3);
    float *proba = cnnFc.cnnProbability(fc_result);
    auto end1 = std::chrono::steady_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();
    cout << "duration for CNN operation = " << duration1 << "ms" << endl;

    cout <<"P not a face:" << proba[1] << " \nP is a face:" << proba[2];

    /*模拟数据测试
    for(int i=0; i<256; i++) {
	cout<<i<<":"<<conv_result1[i]<<" ";
	if((i+1)%128==0) cout<<endl;}
    
    cout<<"------------------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int i=0; i<128; i++) {
	cout<<i<<":"<<pool_result1[i]<<" ";
	if((i+1)%64==0) cout<<endl;}*/

    delete[] conv_result1;
    delete[] pool_result1;
    delete[] conv_result2;
    delete[] pool_result2;
    delete[] conv_result3;
    delete[] fc_result;
    delete[] proba;
    return 0;
}