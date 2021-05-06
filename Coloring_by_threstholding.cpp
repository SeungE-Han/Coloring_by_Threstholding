#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void calc_Histo(const Mat& image, Mat& hist, int bins, int range_max = 256) { //������׷� ��� �Լ�
	int histSize[] = { bins }; //��Ⱚ ����(0~255)
	float range[] = { 0, (float)range_max }; //������׷� ȭ�Ұ� ����(0~255)
	int channels[] = { 0 }; //ä�� ���- ���� ä��
	const float* ranges[] = { range }; //��� ä�� ȭ�� ����

	calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges); //������׷� ���
}

void draw_histo(Mat hist, Mat& hist_img, Size size = Size(256, 200)) { //������׷� �׸��� �Լ�
	hist_img = Mat(size, CV_8U, Scalar(255)); //�Ͼ��
	float bin = (float)hist_img.cols / hist.rows; //�� ��� �ʺ�
	normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX); //������׷��� �ִ��� ������ �ִ�����

	for (int i = 0; i < hist.rows; i++) {
		float start_x = i + bin; //���� �簢�� ���� x ��ǥ
		float end_x = (i + 1)* bin; //���� �簢�� ���� x ��ǥ
		Point2f pt1(start_x, 0);
		Point2f pt2(end_x, hist.at<float>(i)); //y������ ������ ��ŭ �׸�

		if (pt2.y > 0)
			rectangle(hist_img, pt1, pt2, Scalar(0), -1); //���������� ���� �簢�� �׸���
	}
	flip(hist_img, hist_img, 0); //x�� ���� ������
}

int main() {
	Mat img1 = imread("hwfig5-1.jpg", IMREAD_GRAYSCALE); //���� �б�
	CV_Assert(!img1.empty()); //����ó��
	Mat img2 = Mat(size(img1), CV_8UC3, Scalar(0));

	Mat hist, hist_img;
	calc_Histo(img1, hist, 256); //������׷� ���
	draw_histo(hist, hist_img); //������׷� �׸���

	int peak1 = 65, peak2 = 125, peak3 = 190, peak4 = 250; //�������� Ȯ���� 4���� peak
	int th1 = (peak1 + peak2) / 2; //����� ���Ͽ� ����ġ�� ���ϱ�
	int th2 = (peak2 + peak3) / 2;
	int th3 = (peak3 + peak4) / 2; 
	Point pt(0, 0);

	for (pt.y = 0; pt.y < img1.rows; pt.y++) {
		for (pt.x = 0; pt.x < img1.cols; pt.x++) {
			if (img1.at<uchar>(pt.y, pt.x) < th1) { //th1���� ������
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(255, 0, 0); //�Ķ���
			}
			else if (img1.at<uchar>(pt.y, pt.x) < th2 && img1.at<uchar>(pt.y, pt.x) > th1) { //th1���� ũ�ų� th2���� ������
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(0, 255, 0); //�ʷϻ�
			}
			else if (img1.at<uchar>(pt.y, pt.x) < th3 && img1.at<uchar>(pt.y, pt.x) > th2) { //th2���� ũ�ų� th3���� ������
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(0, 0, 255); //������
			}
			else { //������
				img2.at<Vec3b>(pt.y, pt.x) = Vec3b(0, 255, 255); //�����
			}
		}
	}

	imshow("img1", img1);
	imshow("img2", img2);
	imshow("hist_img", hist_img);
	waitKey(0);
	return 0;
}