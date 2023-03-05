#include "convolution.hpp"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description :
 * Function to transform_kernel
 */
Mat transform_kernel(const Mat &src){
    Mat transformed_kernel = Mat::zeros(Size(src.cols,src.rows),src.type());
    for (int i = 0 ; i<src.rows ; i++)
    {
        for (int j = 0 ; j<src.cols ; j++)
        {
            transformed_kernel.at<float>(i,j) = src.at<float>(src.rows-1-i,src.cols-1-j);
        }
    }
        cout << transformed_kernel << endl;
    return transformed_kernel;
}

/*****************************************************************************************************************************************/

Mat Padd_Mono(const Mat &src ,int padding_margin){
    Mat padding_img = Mat::zeros(Size(src.cols+ padding_margin*2,src.rows+padding_margin*2),src.type());
    // Padding ----------------------------------------------------
    for( int i=0; i<src.rows; i++)
    {
        for(int j=0; j<src.cols; j++)
        {
            padding_img.at<uchar>(i+padding_margin,j+padding_margin) = src.at<uchar>(i,j);
        }
    }
    return padding_img;
}

/*****************************************************************************************************************************************/

Mat Padd_RGB(const Mat &src ,int padding_margin){
    Mat padding_img = Mat::zeros(Size(src.cols+ padding_margin*2,src.rows+padding_margin*2),src.type());

    // Padding ----------------------------------------------------
    for( int i=0; i<src.rows; i++)
    {
        for(int j=0; j<src.cols; j++)
        {
            // copy rgb to rgb
            for(int c=0; c<3; c++)
            {
                padding_img.at<Vec3b>(i+padding_margin,j+padding_margin)[c] = src.at<Vec3b>(i,j)[c];
            }
        }
    }
    return padding_img;
}

/*****************************************************************************************************************************************/

Mat Convolute_2d(const Mat &src , const Mat &kernel, int target_app ,int padding_margin)
{
    Mat convoluted_img = Mat::zeros(Size(src.cols,src.rows),src.type());
    if (src.type() == 16)
    {
        convoluted_img = Convolute_2d_RGB(src, kernel,target_app,padding_margin);
    }
    else if (src.type() == 0)
    {
        convoluted_img = Convolute_2d_Mono(src, kernel,target_app,padding_margin);
    }
        else
    {
        cout << "Error: Image type not supported" << endl;
        return src;
    }

    return convoluted_img;
}

/*****************************************************************************************************************************************/

Mat Convolute_2d_Mono(const Mat &src , const Mat &kernel, int target_app ,int padding_margin)
{
    Mat padded_img = Padd_Mono(src,padding_margin);

    Mat new_kernel =  transform_kernel(kernel);

    Mat convoluted_img = Mat::zeros(Size(src.cols,src.rows),src.type());

    int h = kernel.rows /2;
    int w = kernel.cols /2;

    float conv_sum;
// convolute image
    for (int i = padding_margin; i < (padded_img.rows-padding_margin); i++)
    {
        for (int j = padding_margin; j <(padded_img.cols-padding_margin); j++)
        {
            conv_sum = 0;
            for(int m = 0; m < kernel.rows; m++ )
            {
                for(int n = 0; n < kernel.cols ; n++)
                {
                    conv_sum += padded_img.at<uchar>(i+m-h,j+n-w) * new_kernel.at<float>(m,n);
                }
            }
            // absolute of conv_sum
            conv_sum = abs(conv_sum)/target_app;
            convoluted_img.at<uchar>(i-padding_margin,j-padding_margin) = (int)conv_sum;
        }
    }
    return convoluted_img;
}

/*****************************************************************************************************************************************/

Mat Convolute_2d_RGB(const Mat &src , const Mat &kernel, int target_app ,int padding_margin)
{
    Mat padded_img = Padd_RGB(src,padding_margin);

    Mat new_kernel =  transform_kernel(kernel);

    Mat convoluted_img = Mat::zeros(Size(src.cols,src.rows),src.type());

    int h = kernel.rows /2;
    int w = kernel.cols /2;

    float conv_sum_r,conv_sum_g,conv_sum_b;
// convolute image
    for (int i =padding_margin;i<(padded_img.rows-padding_margin) ; i++)
    {
        for (int j = padding_margin ; j<(padded_img.cols-padding_margin) ; j++)
        {
            conv_sum_r = 0;
            conv_sum_g = 0;
            conv_sum_b = 0;
            for(int m = 0 ; m<kernel.rows;m++ )
            {
                for(int n = 0 ; n<kernel.cols ; n++)
                {
                    conv_sum_r += padded_img.at<Vec3b>(i+m-h,j+n-w)[0] * new_kernel.at<float>(m,n);
                    conv_sum_g += padded_img.at<Vec3b>(i+m-h,j+n-w)[1] * new_kernel.at<float>(m,n);
                    conv_sum_b += padded_img.at<Vec3b>(i+m-h,j+n-w)[2] * new_kernel.at<float>(m,n);
                }
            }
            // absolute of conv_sum
            conv_sum_r = abs(conv_sum_r)/target_app;;
            conv_sum_g = abs(conv_sum_g)/target_app;;
            conv_sum_b = abs(conv_sum_b)/target_app;;
            convoluted_img.at<Vec3b>(i-padding_margin,j-padding_margin)[0] = (int)conv_sum_r;
            convoluted_img.at<Vec3b>(i-padding_margin,j-padding_margin)[1] = (int)conv_sum_g;
            convoluted_img.at<Vec3b>(i-padding_margin,j-padding_margin)[2] = (int)conv_sum_b;
        }
    }
    return convoluted_img;
}
