// Modified by: Ishrat Badami, AIS, Uni-Bonn
// Email:       badami@vision.rwth-aachen.de

#include "Pixel.h"
#include "Surfel.h"



using namespace std;

void Surfel::imagesToPointCloud( const cv::Mat& depthImg, const cv::Mat& colorImg, pcl::PointCloud< pcl::PointXYZRGB >::Ptr& cloud) {

    //cloud->header = depthImg.header;//
    cloud->is_dense = true;
    cloud->height = colorImg.rows;// height;
    cloud->width = colorImg.cols; // width;
    cloud->sensor_origin_ = Eigen::Vector4f( 0.f, 0.f, 0.f, 0.f );
    cloud->sensor_orientation_ = Eigen::Quaternionf::Identity();
    //  cloud->sensor_orientation_ = Eigen::Vector4f( std::cos(PI/16), 0.f, 0.f, -std::sin(PI/16) );
    cloud->points.resize( colorImg.rows * colorImg.cols ) ;

    const float invfocalLength = 1.f / 525.f;
    const float centerX = colorImg.cols / 2.f;
    const float centerY = colorImg.rows / 2.f;

    //   const float* depthdata = reinterpret_cast<const float*>(&depthImg.data[0]);
    //   const unsigned char* colordata = &colorImg.data[0];

    cv::Point pt;
    int idx = 0;
    for ( int y = 0; y < colorImg.rows; y++ ) {
        for ( int x = 0; x < colorImg.cols; x++ ) {

            pcl::PointXYZRGB& p = cloud->points[ idx ];

            pt.x = x;
            pt.y = y;
            float dist = depthImg.at< unsigned short >( pt ) / 1000.0f; // convert it into meter

            //      if ( dist == 0 ) {
            //        p.x = std::numeric_limits< float >::quiet_NaN();
            //        p.y = std::numeric_limits< float >::quiet_NaN();
            //        p.z = std::numeric_limits< float >::quiet_NaN();
            //      }
            //      else {

            float xf = x;
            float yf = y;
            p.x = ( xf - centerX ) * dist * invfocalLength;
            p.y = ( yf - centerY ) * dist * invfocalLength;
            p.z = dist;
            //      }
            //        float x_old = ( xf - centerX ) * dist * invfocalLength;
            //        float y_old = ( yf - centerY ) * dist * invfocalLength;
            //        float z_old = dist;
            //
            //        // rotate with angle theta
            //        float theta = -0.38f;
            //        p.x = x_old;
            //        p.y = y_old * cos(theta) - z_old * sin(theta) - .26;
            //        p.z = y_old * sin(theta) + z_old * cos(theta);

            //      }

            //depthdata++;

            float r = colorImg.at<cv::Vec3b>( pt )[ 2 ];
            float g = colorImg.at<cv::Vec3b>( pt )[ 1 ];
            float b = colorImg.at<cv::Vec3b>( pt )[ 0 ];

            pcl::RGB rgb;
            rgb.r = (uint8_t)r;
            rgb.g = (uint8_t)g;
            rgb.b = (uint8_t)b;
            rgb.a = 1;

            p.rgba = rgb.rgba;

            //    p.r = ( unsigned int )colorImg.at<cv::Vec3b>( pt )[ 0 ];
            //    p.g = ( unsigned int )colorImg.at<cv::Vec3b>( pt )[ 1 ];
            //    p.b = ( unsigned int )colorImg.at<cv::Vec3b>( pt )[ 2 ];

            //      int rgb = (r << 16) + (g << 8) + b;
            //      p.rgb = *(reinterpret_cast<float*>(&rgb));


            idx++;

        }
    }


    //  // remove noise
    ////  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZRGB>);
    ////  pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> cloud_cluster_rgb(cloud_cluster);
    //  pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> sor1;
    //  sor1.setInputCloud (cloud);
    //  sor1.setMeanK (200);
    //  sor1.setStddevMulThresh (1.0);
    //  sor1.filter (*cloud);


}




void Surfel::imagesToPointCloud_( cv::Mat& depthImg, cv::Mat& colorImg, pcl::PointCloud< pcl::PointXYZRGB >::Ptr& cloud, cv::Mat &mask ) {

    //cloud->header = depthImg.header;//
    cloud->is_dense = true;
    cloud->height = colorImg.rows;// height;
    cloud->width = colorImg.cols; // width;
    cloud->sensor_origin_ = Eigen::Vector4f( 0.f, 0.f, 0.f, 0.f );
    cloud->sensor_orientation_ = Eigen::Quaternionf::Identity();
    //  cloud->sensor_orientation_ = Eigen::Vector4f( std::cos(PI/16), 0.f, 0.f, -std::sin(PI/16) );
    cloud->points.resize( colorImg.rows * colorImg.cols ) ;

    const float invfocalLength = 1.f / 525.f;
    const float centerX = colorImg.cols / 2.f;
    const float centerY = colorImg.rows / 2.f;

    //   const float* depthdata = reinterpret_cast<const float*>(&depthImg.data[0]);
    //   const unsigned char* colordata = &colorImg.data[0];

    cv::Point pt;
    int idx = 0;
    for ( int y = 0; y < colorImg.rows; y++ ) {
        for ( int x = 0; x < colorImg.cols; x++ ) {

            if( mask.at<unsigned char>(y,x) > 0 ) {

                pcl::PointXYZRGB& p = cloud->points[ idx ];

                pt.x = x;
                pt.y = y;
                float dist = depthImg.at< unsigned short >( pt ) / 1000.0f; // convert it into meter

                //      if ( dist == 0 ) {
                //        p.x = std::numeric_limits< float >::quiet_NaN();
                //        p.y = std::numeric_limits< float >::quiet_NaN();
                //        p.z = std::numeric_limits< float >::quiet_NaN();
                //      }
                //      else {

                float xf = x;
                float yf = y;
                p.x = ( xf - centerX ) * dist * invfocalLength;
                p.y = ( yf - centerY ) * dist * invfocalLength;
                p.z = dist;
                //      }
                //        float x_old = ( xf - centerX ) * dist * invfocalLength;
                //        float y_old = ( yf - centerY ) * dist * invfocalLength;
                //        float z_old = dist;
                //
                //        // rotate with angle theta
                //        float theta = -0.38f;
                //        p.x = x_old;
                //        p.y = y_old * cos(theta) - z_old * sin(theta) - .26;
                //        p.z = y_old * sin(theta) + z_old * cos(theta);

                //      }

                //depthdata++;

                float r = colorImg.at<cv::Vec3b>( pt )[ 2 ];
                float g = colorImg.at<cv::Vec3b>( pt )[ 1 ];
                float b = colorImg.at<cv::Vec3b>( pt )[ 0 ];

                pcl::RGB rgb;
                rgb.r = (uint8_t)r;
                rgb.g = (uint8_t)g;
                rgb.b = (uint8_t)b;
                rgb.a = 1;

                p.rgba = rgb.rgba;
                idx++;

            }
        }
    }
}





void Surfel::houghPointCloud( std::vector< cv::Mat >& houghImg,  const std::vector< float > &scales, pcl::PointCloud< pcl::PointXYZRGB >::Ptr& cloud ) {

//    cloud->is_dense = true;
//    cloud->height = houghImg[0].rows;// height;
//    cloud->width =  houghImg[0].cols; // width;
//    cloud->sensor_origin_ = Eigen::Vector4f( 0.f, 0.f, 0.f, 0.f );
//    cloud->sensor_orientation_ = Eigen::Quaterniond::Identity();
//    //  cloud->sensor_orientation_ = Eigen::Vector4f( std::cos(PI/16), 0.f, 0.f, -std::sin(PI/16) );
//    cloud->points.resize(  houghImg[0].rows *  houghImg[0].cols ) ;

//    cloud->points.reserve(cloud->points.size() + 10000);

    const float invfocalLength = 1.f / 525.f;
    const float centerX =  houghImg[0].cols / 2.f;
    const float centerY =  houghImg[0].rows / 2.f;

    // normalize hough votes in 3D

    std::vector< double > max_val_temp( scales.size() );

    std::vector< double > min_val_temp( scales.size() );

    // detect the maximum
    for( unsigned int scNr = 0; scNr <  scales.size(); scNr++)
        cv::minMaxLoc( houghImg[scNr], &min_val_temp[scNr], &max_val_temp[scNr], 0, 0, cv::Mat() );

    std::vector< double >::iterator it;
    it = std::max_element( max_val_temp.begin(),max_val_temp.end() );
//    int max_index = std::distance( max_val_temp.begin(), it );
//    float maxValue = max_val_temp[max_index];
    float maxValue = *it;
    std::vector< double >::iterator it_;
    it_ = std::min_element( min_val_temp.begin(),min_val_temp.end() );
//    int min_index = std::distance( min_val_temp.begin(), it_ );
//    float maxValue = min_val_temp[max_index];
    float minValue = *it_;

    float scaleInterval = ( scales[ scales.size() - 1 ] - scales[ 0 ]) / float( scales.size() - 1 );

    for ( unsigned int scNr = 0; scNr < houghImg.size(); scNr++ ) {

        cv::Mat tmp;
        cv::convertScaleAbs( houghImg[ scNr ], tmp, 255.f/(maxValue - minValue), -minValue );


        float scale = scales[ 0 ] + scNr * scaleInterval;
        float dist = 1 / ( scale + 0.00001f );

        for ( int y = 0; y < tmp.rows; y++ ) {
            for ( int x = 0; x < tmp.cols; x++ ) {

                uint8_t weight =  tmp.at<uint8_t>( y, x  );

                if(y == 0 || x == 0 || y == tmp.rows-1 || x == tmp.cols-1 || weight > 25 ) {
                    pcl::PointXYZRGB p;

                    p.x = ( x - centerX ) * dist * invfocalLength;
                    p.y = ( y - centerY ) * dist * invfocalLength;
                    p.z = dist;

                    float b, g, r;

                    b = (255 - weight);
                    g = y == 0 || x == 0 || y == tmp.rows-1 || x == tmp.cols-1 ? 255 : 0;
                    r = weight;

                    pcl::RGB rgb;

                    rgb.r = ( uint8_t )r;
                    rgb.g = ( uint8_t )g;
                    rgb.b = ( uint8_t )b;
                    rgb.a = 255;

                    p.rgba = rgb.rgba;

                    cloud->push_back( p );

                }
            }
        }
    }
}

void Surfel::computeSurfel(pcl::PointCloud<pcl::Normal>::Ptr normals, cv::Point2f pt1, cv::Point2f pt2, cv::Point2f center, SurfelFeature &sf, float depth1, float depth2) {

    pcl::Normal n1 = normals->at(pt1.x, pt1.y);
    pcl::Normal n2 = normals->at(pt2.x, pt2.y);

    Eigen::Vector3d v1 = n1.getNormalVector3fMap().cast<double>();
    Eigen::Vector3d v2 = n2.getNormalVector3fMap().cast<double>();

    cv::Point3f  ptR1 = CRPixel::P3toR3(pt1, center, depth1);
    cv::Point3f  ptR2 = CRPixel::P3toR3(pt2, center, depth2);

    cv::Point3f temp = ptR1 - ptR2;
    Eigen::Vector3d distVec( temp.x, temp.y, temp.z );

    v1.normalize();
    v2.normalize();
    Eigen::Vector3d distVec_norm = distVec;
    distVec_norm.normalize();

    sf.fVector[0] = std::acos(v1.dot(v2));
    sf.fVector[1] = std::acos(v1.dot(distVec_norm));
    sf.fVector[2] = std::acos(v2.dot(distVec_norm));
    sf.fVector[3] = std::sqrt(distVec.dot(distVec));

}


void Surfel::calcSurfel2CameraTransformation(cv::Point3f& s1, cv::Point3f& s2, pcl::Normal& n1, pcl::Normal& n2, Eigen::Matrix4d& TransformationSC1, Eigen::Matrix4d& TransformationSC2) {

    cv::Point3f distance = s1 - s2;
    Eigen::Vector3d t1(s1.x,s1.y,s1.z);
    Eigen::Vector3d t2(s2.x,s2.y,s2.z);
    Eigen::Vector3d dis(distance.x, distance.y, distance.z);
    dis.normalize();

    // First coordinate system
    TransformationSC1                 = Eigen::Matrix4d::Identity();

    Eigen::Vector3d normal1           = n1.getNormalVector3fMap().cast<double>();
    normal1.normalize();
    Eigen::Vector3d u1                = normal1.cross(dis);
    u1.normalize();
    Eigen::Vector3d v1                = normal1.cross(u1);

    //    cout<< normal1 << endl;
    //    cout<< u1 << endl;
    //    cout<< v1 << endl;
    //    cout<< s1.getVector3dMap() << endl;

    TransformationSC1.block<3,1>(0,2) = normal1;
    TransformationSC1.block<3,1>(0,1) = u1;
    TransformationSC1.block<3,1>(0,0) = v1;
    TransformationSC1.block<3,1>(0,3) = t1;

    //    cout<< TransformationSC1 << endl;


    // Second cordinate syastem

    TransformationSC2                 = Eigen::Matrix4d::Identity();

    Eigen::Vector3d normal2           = n2.getNormalVector3fMap().cast<double>();
    normal2.normalize();
    Eigen::Vector3d u2                = normal2.cross(dis);
    u2.normalize();
    Eigen::Vector3d v2                = normal2.cross(u2);

    //    cout<< normal2 << endl;
    //    cout<< u2 << endl;
    //    cout<< v2 << endl;
    //    cout<< s2.getVector3dMap() << endl;

    TransformationSC2.block<3,1>(0,2) = normal2;
    TransformationSC2.block<3,1>(0,1) = u2;
    TransformationSC2.block<3,1>(0,0) = v2;
    TransformationSC2.block<3,1>(0,3) = t2;

    //    cout<< TransformationSC2 << endl;

}


void  Surfel::addCoordinateSystem( Eigen::Matrix4d& transformationMatrixOC, boost::shared_ptr< pcl::visualization::PCLVisualizer >& viewer, string id) {

    //visualize coordinates in object frame
    pcl::Normal OX(transformationMatrixOC.matrix()(0,0), transformationMatrixOC.matrix()(1,0), transformationMatrixOC.matrix()(2,0));
    pcl::Normal OY(transformationMatrixOC.matrix()(0,1), transformationMatrixOC.matrix()(1,1), transformationMatrixOC.matrix()(2,1));
    pcl::Normal OZ(transformationMatrixOC.matrix()(0,2), transformationMatrixOC.matrix()(1,2), transformationMatrixOC.matrix()(2,2));
    pcl::Normal Origin(transformationMatrixOC.matrix()(0,3), transformationMatrixOC.matrix()(1,3), transformationMatrixOC.matrix()(2,3));

    pcl::PointXYZ O_(Origin.normal_x, Origin.normal_y, Origin.normal_z);
    pcl::PointXYZ X_(0.2 *OX.normal_x + Origin.normal_x, 0.2 *OX.normal_y + Origin.normal_y, 0.2 * OX.normal_z + Origin.normal_z);
    pcl::PointXYZ Y_(0.2 *OY.normal_x + Origin.normal_x, 0.2 *OY.normal_y + Origin.normal_y, 0.2 * OY.normal_z + Origin.normal_z);
    pcl::PointXYZ Z_(0.2 *OZ.normal_x + Origin.normal_x, 0.2 *OZ.normal_y + Origin.normal_y, 0.2 * OZ.normal_z + Origin.normal_z);

    viewer->addLine( O_, X_, 255, 0, 0, ( "lineX" + id).c_str() );
    viewer->addLine( O_, Y_, 0, 255, 0, ( "liney" + id).c_str() );
    viewer->addLine( O_, Z_, 0, 0, 255, ( "linez" + id).c_str() );

}

void Surfel::addCoordinateSystem(const Eigen::Matrix4d& transformationMatrixOC, boost::shared_ptr< pcl::visualization::PCLVisualizer >& viewer, string id) {

    Eigen::Matrix4d transformationMatrixOC1  =  transformationMatrixOC;
    addCoordinateSystem( transformationMatrixOC1, viewer, id);
}



//void imagesToPointCloud(const sensor_msgs::Image& depthImg, const sensor_msgs::Image colorImg,
//                        pcl::PointCloud<pcl::PointXYZRGB>::Ptr& cloud) {
//
//  cloud->header = depthImg.header;
//  cloud->is_dense = true;
//  cloud->height = depthImg.height;
//  cloud->width = depthImg.width;
//  cloud->sensor_origin_ = Eigen::Vector4f(0.f, 0.f, 0.f, 0.f);
//  cloud->sensor_orientation_ = Eigen::Quaterniond::Identity();
//  cloud->points.resize(depthImg.width * depthImg.height);
//
//  const float invfocalLength = 1.f / 525.f;
//  const float centerX = 319.5f;
//  const float centerY = 239.5f;
//
//  const float* depthdata = reinterpret_cast<const float*>(&depthImg.data[0]);
//  const unsigned char* colordata = &colorImg.data[0];
//  int idx = 0;
//  for (unsigned int y = 0; y < depthImg.height; y++) {
//    for (unsigned int x = 0; x < depthImg.width; x++) {
//
//      pcl::PointXYZRGB& p = cloud->points[idx];
//
//      float dist = (*depthdata);
//
//      if (isnan(dist)) {
//        p.x = std::numeric_limits<float>::quiet_NaN();
//        p.y = std::numeric_limits<float>::quiet_NaN();
//        p.z = std::numeric_limits<float>::quiet_NaN();
//      }
//      else {
//
//        float xf = x;
//        float yf = y;
//        p.x = (xf - centerX) * dist * invfocalLength;
//        p.y = (yf - centerY) * dist * invfocalLength;
//        p.z = dist;
//      }
//
//      depthdata++;
//
//      int r = (*colordata++);
//      int g = (*colordata++);
//      int b = (*colordata++);
//
//      int rgb = (r << 16) + (g << 8) + b;
//      p.rgb = *(reinterpret_cast<float*>(&rgb));
//
//      idx++;
//
//    }
//  }
//}
