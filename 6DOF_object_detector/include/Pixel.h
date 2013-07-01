/*
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

// Modified by: Ishrat Badami, AIS, Uni-Bonn
// Email:       badami@vision.rwth-aachen.de

#pragma once

#define _copysign copysign

#include <opencv2/core/core.hpp>
//#include <opencv2/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include "HoG.h"
#include "Surfel.h"
#include "math.h"

#include "Parameters.h"

#define  PI 3.14159265f

// structure for sampled image pixel

struct Static{

    int iWidth, iHeight;
    float scale;
    cv::Point2f  pixelLocation;
    cv::Point3f  pixelLocation_real;
    cv::Point3f disVector;
    cv::Rect bbox;
    std::vector<cv::Mat> imgAppearance;
    Eigen::Matrix4f transformationMatrixOC;
    pcl::PointCloud<pcl::Normal>::Ptr normals;

    cv::Point3f bbSize3D;

};

struct Dynamic{

    Dynamic(){alpha.reserve(20);}

    std::pair< Eigen::Vector3f, Eigen::Vector3f > disVectorInQueryPixelCordinate;
    std::pair< Eigen::Vector3f, Eigen::Vector3f > tempDisVectorInQueryPixelCordinate;
    std::pair< Eigen::Quaternionf, Eigen::Quaternionf > pointPairTransformation;

    std::pair< Eigen::Matrix3f, Eigen::Matrix3f > firstQuerytoCameraTransformation;

    std::pair< Eigen::Quaternionf, Eigen::Quaternionf > transformationMatrixOQuery_at_current_node;

    std::vector< std::pair < float, float > > alpha;

};

struct PixelFeature {

    PixelFeature() {
        statFeature = new Static;
        dynFeature = new Dynamic;
    }

    Static* statFeature;
    Dynamic* dynFeature;

    //    int iWidth, iHeight;
    //    float scale;
    //    cv::Point2f  pixelLocation;
    //    cv::Point3f  pixelLocation_real;
    //    cv::Point3f disVector;
    //    cv::Rect bbox;
    //    std::vector<cv::Mat> imgAppearance;
    //    Eigen::Matrix4f transformationMatrixOC;
    //    pcl::PointCloud<pcl::Normal>::Ptr normals;

    //    cv::Point3f bbSize3D;

    //    std::pair< Eigen::Vector3f, Eigen::Vector3f > disVectorInQueryPixelCordinate;
    //    std::pair< Eigen::Vector3f, Eigen::Vector3f > tempDisVectorInQueryPixelCordinate;
    //    std::pair< Eigen::Quaternionf, Eigen::Quaternionf > pointPairTransformation;

    //    std::pair< Eigen::Matrix3f, Eigen::Matrix3f > firstQuerytoCameraTransformation;

    //    std::pair< Eigen::Quaternionf, Eigen::Quaternionf > transformationMatrixOQuery_at_current_node;

    //    std::vector< std::pair < float, float > > alpha;

    void print() const {
        std::cout << "displacement vector = " << statFeature->disVector << std::endl;
    }
};


class CRPixel {
public:
    CRPixel(cv::RNG* pRNG) : cvRNG(pRNG) {}
    void setClasses(int l) {vRPixels.resize(l);vImageIDs.resize(l);}// vImageIDs.resize(l);}

    // Extract patches from image
    void extractPixels(IplImage *img, unsigned int n, int label, CvRect* box = 0, CvPoint* vCenter = 0);
    // Extract patches from image and adding its id to the patch (in vImageIDs)
    void extractPixels(const Parameters& param, const cv::Mat &img, const cv::Mat &depthImg, unsigned int n, int label, int imageID,  CvRect* box =0, CvPoint* vCenter=0, cv::Point3f *cg = 0 , cv::Point3f *bbDimension =0, Eigen::Matrix4f *transformationOC = 0 );

    // Convert pixel coordinates to real coordinates
    static cv::Point3f P3toR3(cv::Point2f &pt, cv::Point2f &center, float depth);

    // Convert real coordinates to pixel  coordinates
    static void R3toP3(cv::Point3f &realCoordinates, cv::Point2f &center, cv::Point2f &pixelCoordinates, float &depth);

    // Compute Normals
    static void computeNormals(const cv::Mat& img, const cv::Mat& depthImg, pcl::PointCloud<pcl::Normal>::Ptr& normals);

    // Extract features from image
    static void extractFeatureChannels(const Parameters& param, const cv::Mat &img, const cv::Mat &depthImg, std::vector<cv::Mat>& vImg, pcl::PointCloud<pcl::Normal>::Ptr& normals);

    // calculate transformation from object frame to camera frame
    static void calcObject2CameraTransformation( float &pose, float &pitch, cv::Point3f &rObjCenter, Eigen::Matrix4f &transformationMatrixOC );

    // Draws transformation
    static void drawTransformation(const cv::Mat &img, const cv::Mat &depthImg , const Eigen::Matrix4f& transformationMatrixOC);

    // compute affine transformation from rotation quaternion and translation vector
    static Eigen::Matrix4f getTransformationAtQueryPixel( Eigen::Matrix3f &qObjectQuery, Eigen::Matrix4f transformationMatrixOC,  cv::Point3f &pointLocation);

    // min/max filter
    static void minfilt( std::vector< cv::Mat >& src, const cv::Mat& depthImg, const std::vector<float>scales, unsigned int kSize );
    static void maxfilt( std::vector< cv::Mat >& src, const cv::Mat& depthImg, const std::vector<float>scales, unsigned int kSize );

    static void maxfilt(uchar* data, uchar* maxvalues, unsigned int step, unsigned int size, unsigned int width);
    static void maxfilt(uchar* data, unsigned int step, unsigned int size, unsigned int width);
    static void minfilt(uchar* data, uchar* minvalues, unsigned int step, unsigned int size, unsigned int width);
    static void minfilt(uchar* data, unsigned int step, unsigned int size, unsigned int width);
    static void maxminfilt(uchar* data, uchar* maxvalues, uchar* minvalues, unsigned int step, unsigned int size, unsigned int width);
    static void maxfilt(cv::Mat src, unsigned int width);
    static void maxfilt(IplImage *src, IplImage *dst, unsigned int width);
    static void minfilt(IplImage *src, unsigned int width);
    static void minfilt(cv::Mat src, cv::Mat  dst, unsigned int width);

    std::vector<std::vector< PixelFeature > > vRPixels;
    std::vector<std::vector< int> > vImageIDs; // vector the same size as vRPixels

private:
    cv::RNG *cvRNG;

};
