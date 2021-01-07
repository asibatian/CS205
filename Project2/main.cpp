#include "CNN_net.h"
#include "face_binary_cls1.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>
using namespace cv;

int main()
{
    //Load weights from "face_binary_cls1.h"
    float *Weight1 = conv0_weight;
    float *Bias1 = conv0_bias;
    float *Weight2 = conv1_weight;
    float *Bias2 = conv1_bias;
    float *Weight3 = conv2_weight;
    float *Bias3 = conv2_bias;
    float *Weight_fc = fc0_weight;
    float *Bias_fc = fc0_bias;

    CNN_Pool cnnPool(2, 2);               //Set a 2*2 pooling layer
    CNN_Conv cnnConv1(16, 3, 3, 3, 2, 1); //Set first conv layer:out-channel picture-channel kernel_h kernel_w stride padding 
    cnnConv1.loadWeight(Weight1, Bias1);  //Load weights and bias

    CNN_Conv cnnConv2(32, 16, 3, 3, 1, 0);
    cnnConv2.loadWeight(Weight2, Bias2);

    CNN_Conv cnnConv3(32, 32, 3, 3, 2, 1);
    cnnConv3.loadWeight(Weight3, Bias3);

    CNN_Fc cnnFc(2048, 2);
    cnnFc.loadWight(Weight_fc, Bias_fc);

    auto start = std::chrono::steady_clock::now();
    string ima_name = "bg.jpg";
    Mat imageBGR = imread(ima_name); //read a picture
    cout << "Picture name: " << ima_name << endl;
    // imshow("MyPictureBGR",imageBGR);
    // waitKey(1000);
    Mat imageRGB(imageBGR.rows, imageBGR.cols, CV_8UC3); //creat a 3 channel picture
    for (int i = 0; i < imageBGR.rows; ++i) //Convert BGR to RGB
	{ 
		Vec3b *p1 = imageBGR.ptr<Vec3b>(i); 
		Vec3b *p2 = imageRGB.ptr<Vec3b>(i);
		for(int j=0; j<imageBGR.cols; ++j) 
		{ 
			//convert BGR to RGB 
			p2[j][2] = p1[j][0]; 
			p2[j][1] = p1[j][1]; 
			p2[j][0] = p1[j][2]; 
		}
    }

    float imageArray[3*imageRGB.rows*imageBGR.cols]; //Cast Mat to array
    for(int k = 0; k < 3; k++)
    {
        for(int i = 0; i < imageRGB.rows; i++)
        {
            for(int j = 0; j < imageRGB.cols; j++)
            {
                imageArray[k*imageRGB.rows*imageRGB.cols + i*imageRGB.cols + j] = ((float)imageRGB.at<Vec3b>(i,j)[k])/255.0f; //store image into a array
            }
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "\nduration for image operation = " << duration << "ms" << endl;

    auto start1 = std::chrono::steady_clock::now();
    float *conv_result1 = cnnConv1.cnnForwardConv(imageArray, 1, 3, 128, 128); // input an image array，batchsize, picture channel(RGB), pic_h, pic_w
    float *pool_result1 = cnnPool.cnnForwardMaxpool(conv_result1, 16, 64, 64); // After first conv by padding=2, stride=1, get a batchsize(1)*channels(16)*h(64)*w(64), then MaxPool
    float *conv_result2 = cnnConv2.cnnForwardConv(pool_result1, 1, 16, 32, 32); // Input 1*16*32*32,then conv by padding=1,stride=1; output 1*32*30*30 to MaxPool
    float *pool_result2 = cnnPool.cnnForwardMaxpool(conv_result2, 32, 30, 30); // Input 1*32*30*30 to MaxPool, output 1*32*15*15;
    float *conv_result3 = cnnConv3.cnnForwardConv(pool_result2, 1, 32, 15, 15); // Input 1*32*15*15,o then conv by padding=2, stride=1; output 1*32*8*8 to Gemm(no need to flatten)
    float *fc_result = cnnFc.cnnForwardFc(conv_result3); // Fully-conected
    float *proba = cnnFc.cnnProbability(fc_result); // Calculate score and then Softmax
    auto end1 = std::chrono::steady_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();
    cout << "\nduration for CNN operation = " << duration1 << "ms" << endl;

    cout << "\nNotFace: p1 = " << proba[0] << endl;
    cout << "\nIsFace: p2 = " << proba[1] << endl;

    delete[] conv_result1;
    delete[] pool_result1;
    delete[] conv_result2;
    delete[] pool_result2;
    delete[] conv_result3;
    delete[] fc_result;
    delete[] proba;
    return 0;
}