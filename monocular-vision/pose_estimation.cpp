#include "include/feature_matches.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

void findFuture(Mat &img1, Mat &img2, vector<KeyPoint> &kp1, vector<KeyPoint> &kp2,
                vector<DMatch> &matches_all, vector<DMatch> &matches_gf);

void poseEstimation(vector<KeyPoint> kp1, vector<KeyPoint> kp2, vector<DMatch> matches, Mat& R, Mat& t);

int main(int argc, char** argv){
  if(argc != 3){
    cout << "usage: poseEstimation_2d2d img1 img2" << endl;
    return -1;
  }

  // 读取图像
  Mat img_1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  Mat img_2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);

  vector<KeyPoint> kp1, kp2;
  vector<DMatch> matches_all, matches_gf;
  findFuture(img_1, img_2, kp1, kp2, matches_all, matches_gf);
  cout << "一共找到了" << matches_gf.size() << "组匹配点" << endl;

  // 估计两图像运动
  Mat R, t;
  poseEstimation(kp1, kp2, matches_gf, R, t);

}

void findFuture(Mat &img1, Mat &img2, vector<KeyPoint> &kp1, vector<KeyPoint> &kp2,
                vector<DMatch> &matches_all, vector<DMatch> &matches_gf){
   Mat d1, d2;
   // ORB初始化
   Ptr<ORB> orb = ORB::create(1000);
   orb->setFastThreshold(0);
   orb->detectAndCompute(img1, Mat(), kp1, d1);
   orb->detectAndCompute(img2, Mat(), kp2, d2);

   BFMatcher matcher(NORM_HAMMING);
   matcher.match(d1, d2, matches_all);

   // GMS filter
   int num_inliers = 0;
   vector<bool> vbInliers;
   feature_matches feature(kp1, img1.size(), kp2, img2.size(), matches_all);
   num_inliers = feature.GetInlierMask(vbInliers, false, true);

   cout << "get total" << num_inliers << "matches" << endl;

   for(size_t i = 0; i < vbInliers.size(); i++){
     if(vbInliers[i] == true){
       matches_gf.push_back(matches_all[i]);
       // cout << matches_gf[i].distance << endl;
     }
   }


}

Point2d pixel2cam ( const Point2d& p, const Mat& K )
{
    return Point2d
           (
               ( p.x - K.at<double> ( 0,2 ) ) / K.at<double> ( 0,0 ),
               ( p.y - K.at<double> ( 1,2 ) ) / K.at<double> ( 1,1 )
           );
}

void poseEstimation(vector<KeyPoint> kp1, vector<KeyPoint> kp2, vector<DMatch> matches, Mat& R, Mat& t){
    // 相机内参
    Mat K = (Mat_<double> (3,3) << 329.0, 0, 249.7, 0, 329.0, 235.7, 0, 0, 1);

    // 把匹配点转换为vector<point2f>的形式
    vector<Point2f> points1;
    vector<Point2f> points2;

    for(int i = 0; i < (int)matches.size(); i++){
      points1.push_back(kp1[matches[i].queryIdx].pt);
      points2.push_back(kp2[matches[i].queryIdx].pt);
    }

    // 计算基础矩阵
    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat(points1, points2, CV_FM_8POINT);
    cout << "fundamental_mat is " << endl << fundamental_matrix << endl;

    // 计算本质矩阵
    Point2d principal_point(249.7, 235.7); // 相机光心
    double focal_length = 329.0; // 相机焦距
    Mat essential_matrix;
    essential_matrix = findEssentialMat(points1, points2, focal_length, principal_point);
    cout << "essential_matrix is " << endl << essential_matrix << endl;

    // 计算单应矩阵
    Mat homography_matrix;
    homography_matrix = findHomography(points1, points2, RANSAC, 3);
    cout << "homography_matrix is " << endl << homography_matrix << endl;

    // 从本质矩阵中恢复旋转和平移信息
    recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point);
    cout << "R is " << endl << R << endl;
    cout << "t is " << endl << t << endl;
}
