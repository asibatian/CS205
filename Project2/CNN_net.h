#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class CNN_Conv //定义CNN中的卷积层
{
private:
    int out_channels, in_channels, kernel_size_h, kernel_size_w;
    int stride;
    int padding;
    float ****p = NULL; //定义存储权值的四维矩阵指针
    float *bias = NULL;

public:
    CNN_Conv()
    {
        out_channels = 0;
        in_channels = 0;
        kernel_size_h = 0;
        kernel_size_w = 0;
        stride = 0;
        padding = 0;
    }
    CNN_Conv(int out_channels, int in_channels, int kernel_size_h, int kernel_size_w, int stride, int padding)
    {
        this->out_channels = out_channels;
        this->in_channels = in_channels;
        this->kernel_size_h = kernel_size_h;
        this->kernel_size_w = kernel_size_w;
        this->stride = stride;
        this->padding = padding;

        //初始化权值矩阵  out_channels*in_channels*kernel_size_h*kernel_size_w
        this->p = new float ***[out_channels];
        for (int i = 0; i < out_channels; i++)
        {
            p[i] = new float **[in_channels];
            for (int j = 0; j < in_channels; j++)
            {
                p[i][j] = new float *[kernel_size_h];
                for (int k = 0; k < kernel_size_h; k++)
                {
                    p[i][j][k] = new float[kernel_size_w];
                }
            }
        }
    }
    ~CNN_Conv()
    {
        for (int i = 0; i < out_channels; i++)
        {
            for (int j = 0; j < in_channels; j++)
            {
                for (int k = 0; k < kernel_size_h; k++)
                {
                    delete[] p[i][j][k];
                }
            }
        }
        for (int i = 0; i < out_channels; i++)
        {
            for (int j = 0; j < in_channels; j++)
            {
                delete[] p[i][j];
            }
        }
        for (int i = 0; i < out_channels; i++)
        {
            delete[] p[i];
        }
        delete[] p;
    }

    //加载预训练的CNN网络参数
    void loadWeight(float *Weight, float *bias)
    {
        this->bias = bias;
        int count = 0;
        //cout<<"load1"<<endl;
        for (int i = 0; i < out_channels; i++)
        {
            for (int j = 0; j < in_channels; j++)
            {
                for (int k = 0; k < kernel_size_h; k++)
                {
                    //cout<<"load2"<<endl;
                    for (int m = 0; m < kernel_size_w; m++)
                    {
                        //cout<<"load3"<<endl;
                        p[i][j][k][m] = *(Weight + i * (in_channels * kernel_size_h * kernel_size_w) + j * (kernel_size_h * kernel_size_w) + k * kernel_size_w + m);
                    }
                }
            }
            //cout<<count++<<endl;
        }
    }
    //conv层forward实现，包括BN核Relu层
    float *cnnForwardConv(float *image, int batch_size, int channel, int image_h, int image_w)
    {
        if (channel != in_channels)
        {
            cout<<"卷积核inchannels与图像channels不匹配"<<endl;//差错处理，卷积核inchannels与图像channels不匹配
        }
        int featuremap_h = (image_h + 2 * padding - kernel_size_h) / stride + 1;
        int featuremap_w = (image_w + 2 * padding - kernel_size_w) / stride + 1;
        float *conv_result = new float[batch_size * out_channels * featuremap_h * featuremap_w];
        int count = 0; //记录conv_result的下标

        //conv层
        for (int batch = 0; batch < batch_size; batch++)
        {
            for (int outChannel = 0; outChannel < out_channels; outChannel++) //测试
            {
                for (int row = 0 - padding; row < image_h + padding - kernel_size_h + 1; row += stride) //测试
                {
                    for (int col = 0 - padding; col < image_w + padding - kernel_size_w + 1; col += stride)
                    {
                        float sum_rgb = 0;
                        for (int inChnanel = 0; inChnanel < in_channels; inChnanel++) //测试
                        {
                            float sum_single = 0;
                            for (int i = 0; i < kernel_size_h; i++)
                            {
                                for (int j = 0; j < kernel_size_w; j++)
                                {
                                    int hi = row + i; // cout<<"hi:"<<hi<<endl;
                                    int wi = col + j; //cout<<"wi:"<<wi<<endl;
                                    float pix;
                                    if (hi < 0 || wi < 0||hi == image_h || wi ==image_w || hi > image_h || wi >image_w)
                                    {
                                        pix = 0;
                                    }
                                    else
                                    {
                                        pix = image[batch * (channel * image_h * image_w) + inChnanel * (image_w * image_h) + hi * image_w + wi];
                                    }
                                    //cout<<"pix:"<<pix<<"wight"<<p[outChannel][inChnanel][i][j]<<endl;
                                    sum_single += pix * p[outChannel][inChnanel][i][j];
                                    //cout<<"sum_single:"<<sum_single<<endl;
                                }
                            }
                            sum_rgb += sum_single;
                        }
                        //relu激活函数
                        sum_rgb += bias[outChannel];
                        if (sum_rgb > 0)
                            *(conv_result + count) = sum_rgb;
                        else
                            *(conv_result + count) = 0;
                        //cout << count << "sum_rgb:" << sum_rgb << endl;
                        count++;
                    }
                }
            }
        }
        cout << "conv_count:" << count << endl; //应该是batch_size*out_channels * featuremap_h * featuremap_w
        return conv_result;
    }
};

class CNN_Pool
{
private:
    int pool_h, pool_w;

public:
    CNN_Pool()
    {
        pool_h = 0;
        pool_w = 0;
    }
    CNN_Pool(int pool_h, int pool_w)
    {
        this->pool_h = pool_h;
        this->pool_w = pool_w;
    }
    //maxpool层实现
    float *cnnForwardMaxpool(float *image, int channels, int image_h, int image_w)
    {
        float *pool_result = new float[channels * (image_h / pool_h) * (image_w / pool_w)];
        int count = 0;
        for (int inchanel = 0; inchanel < channels; inchanel++)
        {
            //对每层特征图做池化
            for (int row = 0; row + pool_h - 1 < image_h; row += pool_h)
            {
                for (int col = 0; col + pool_w - 1 < image_w; col += pool_w)
                {
                    //找最大值
                    float max = 0;
                    for (int i = 0; i < pool_h; i++)
                    {
                        for (int j = 0; j < pool_w; j++)
                        {
                            int hi = row + i;
                            int wi = col + j;
                            float temp = image[inchanel * image_h * image_w + hi * image_w + wi];
                            if (temp > max)
                                max = temp;
                        }
                    }
                    *(pool_result + count) = max;
                    count++;
                }
            }
        }
        cout << "maxpool_count:" << count << endl; //应该是 channels*(image_h/pool_h)*(image_w/pool_w)
        return pool_result;
    }
};

class CNN_Fc
{
private:
    float *p = NULL;
    float *bias = NULL;
    int input_size, num_cls;

public:
    CNN_Fc()
    {
        input_size = 0;
        num_cls = 0;
    }
    CNN_Fc(int input_size, int num_cls)
    {
        this->input_size = input_size;
        this->num_cls = num_cls;
    }

    void loadWight(float* Wight,float* Bias)
    {
        p=Wight;
        bias=Bias;
    }

    float* cnnForwardFc(float *vector)
    {
        float* fc_result=new float [num_cls];
        int count=0;
        for(int i=0;i<num_cls;i++){
            float score=0;
            for(int j=0;j<input_size;j++){
                score+=p[i*input_size+j]*vector[j];
            }
            score+=bias[i];
            *(fc_result+count)=score;
            count++;
        }
        cout << "fc_count:" << count << endl;
        return fc_result;
    }

    float* cnnProbability(float *score){
        float* proba=new float[num_cls];
        
        float escore=0;
        for(int i=0;i<num_cls;i++){
            escore+=exp(score[i]);
            cout<<exp(score[i])<<endl;
        }
        for(int i=0;i<num_cls;i++){
            *(proba+i)=exp(score[i])/escore;
        }
        return proba;
    }
};