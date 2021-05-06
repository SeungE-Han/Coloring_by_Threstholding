#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void calc_Histo(const Mat& image, Mat& hist, int bins, int range_max = 256) { //히스토그램 계산 함수
	int histSize[] = { bins }; //밝기값 범위(0~255)
	float range[] = { 0, (float)range_max }; //히스토그램 화소값 범위(0~255)
	int channels[] = { 0 }; //채널 목록- 단일 채널
	const float* ranges[] = { range }; //모든 채널 화소 범위

	calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges); //히스토그램 계산
}

void draw_histo(Mat hist, Mat& hist_img, Size size = Size(256, 200)) { //히스토그램 그리는 함수
	hist_img = Mat(size, CV_8U, Scalar(255)); //하얀색
	float bin = (float)hist_img.cols / hist.rows; //한 계급 너비
	normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX); //히스토그램의 최댓값을 높이의 최댓값으로

	for (int i = 0; i < hist.rows; i++) {
		float start_x = i + bin; //막대 사각형 시작 x 좌표
		float end_x = (i + 1)* bin; //막대 사각형 종료 x 좌표
		Point2f pt1(start_x, 0);
		Point2f pt2(end_x, hist.at<float>(i)); //y축으로 누적된 만큼 그림

		if (pt2.y > 0)
			rectangle(hist_img, pt1, pt2, Scalar(0), -1); //검은색으로 막대 사각형 그리기
	}
	flip(hist_img, hist_img, 0); //x축 기준 뒤집기
}

int main() {
	Mat img1 = imread("hwfig5-1.jpg", IMREAD_GRAYSCALE); //영상 읽기
	CV_Assert(!img1.empty()); //예외처리
	Mat img2 = Mat(size(img1), CV_8UC3, Scalar(0));

	Mat hist, hist_img;
	calc_Histo(img1, hist, 256); //히스토그램 계산
	draw_histo(hist, hist_img); //히스토그램 그리기

	int peak1 = 65, peak2 = 125, peak3 = 190, peak4 = 250; //육안으로 확인한 4개의 peak
	int th1 = (peak1 + peak2) / 2; //평균을 구하여 문턱치값 구하기
	int th2 = (peak2 + peak3) / 2;
	int th3 = (peak3 + peak4) / 2; 
	Point pt(0, 0);

	for (pt.y = 0; pt.y < img1.rows; pt.y++) {
		for (pt.x = 0; pt.x < img1.cols; pt.x++) {
			if (img1.at<uchar>(pt.y, pt.x) < th1) { //th1보다 작으면
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(255, 0, 0); //파란색
			}
			else if (img1.at<uchar>(pt.y, pt.x) < th2 && img1.at<uchar>(pt.y, pt.x) > th1) { //th1보다 크거나 th2보다 작으면
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(0, 255, 0); //초록색
			}
			else if (img1.at<uchar>(pt.y, pt.x) < th3 && img1.at<uchar>(pt.y, pt.x) > th2) { //th2보다 크거나 th3보다 작으면
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(0, 0, 255); //빨간색
			}
			else { //나머지
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(0, 255, 255); //노란색
			}
		}
	}

	imshow("img1", img1);
	imshow("img2", img2);
	imshow("hist_img", hist_img);
	waitKey(0);
	return 0;
}