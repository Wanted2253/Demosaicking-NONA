#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "guidedfilter.h"
using namespace std;
using namespace cv;
void weight_generator_diff(vector<vector<double>> &weights, vector<vector<double>> &distance, double k ) {
	double sum1,sum2,factor1,factor2;
	for(int i=0;i<weights.size();i++){
		sum = 0;
		sum2 = 0;
		factor1 = 1;
		factor2 = -1;
		for(int j=0;j<9;j++){
			sum1 += pow(distance[i][j],k);
		}
		for(int j=9;j<18 ;j++){
			sum2 += pow(distance[i][j],k);
		}
		factor1 /= sum;
		factor2 /= sum2;
		for(int j=0;j<9;j++){
			weights[i][j] = factor1*pow(distance[i][j],k);
		}
		for(int j=9;j<18;j++){
			weights[i][j] = factor2*pow(distance[i][j],k);
		}
	}
	return;
}
Mat diffilter_H(Mat Src1ch){
	Mat result = Mat::zeros(Src1ch.rows, Src1ch.cols, CV_32F);
	int colend = (Src1ch.cols / 3) - 1;
	int rowk;
	int colk;
	int left_startrow,left_startcol,right_startrow,right_startcol;
	int l,r,w;
	vector<vector<double>> weights_h( 9 , vector<double> (18, 0));
	vector<vector<double>> dist_h( 9 , vector<double> (18, 0));
	distance_generator(dist_h,1);
	weight_generator_diff(weights_h,dist_h,k);
	for (int row = 0; row < result.rows; row++) {
		for (int col = 0; col < 3; col++) {
			rowk = row % 3;
			colk = col % 3;
			left_startrow = row  - rowk;
			left_startcol = col + 3 - colk;
			right_startrow = row - rowk;
			right_startcol = col + 3 - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_h[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(right_startrow + i, right_startcol + j)))*(weights_h[w][r++]));
				}
			}
		}
	}
	for (int row = 0; row < result.rows; row++) {
		for (int col = result.cols - 1; col > result.cols - 4; col--) {
			rowk = row % 3;
			colk = col % 3;
			left_startrow = row  - rowk;
			left_startcol = col - 3 - colk;
			right_startrow = row - rowk;
			right_startcol = col - 3 - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_h[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(right_startrow + i, right_startcol + j)))*(weights_h[w][r++]));
				}
			}
		}
	}
	for (int row = 0; row < result.rows; row++) {
		for (int col = 3; col < result.cols - 3; col++) {
			rowk = row % 3;
			colk = col % 3;
			left_startrow = row - rowk;
			left_startcol = col - 3 - colk;
			right_startrow = row - rowk;
			right_startcol = col + 3 - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_h[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(right_startrow + i, right_startcol + j)))*(weights_h[w][r++]));
				}
			}
		}
	}
	return result;
}

Mat diffilter_V(Mat Src1ch){
	Mat result = Mat::zeros(Src1ch.rows, Src1ch.cols, CV_32F);
	int rowend = (Src1ch.rows / 3) - 1;
	int rowk;
	int colk;
	int left_startrow,left_startcol,right_startrow,right_startcol;
	int l,r,w;
	vector<vector<double>> weights_v( 9 , vector<int> (18, 0));
	distancegenerator(dist_h,0);
	vector<vector<double>> dist_v( 9 , vector<int> (18, 0));
	weightgenerator_diff(weights_v,dist_v,k);
	for (int col = 0; col < result.cols; col++) {
		for (int row = 0; row < 3; row++) {
			rowk = row % 3;
			colk = col % 3;
			top_startrow = row + 3 - rowk;
			top_startcol = col - colk;
			bottom_startrow = row + 3 - rowk;
			bottom_startcol = col - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(top_startrow + i, top_startcol + j)))*(weights_v[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(bottom_startrow + i, bottom_startcol + j)))*(weights_v[w][r++]));
				}
			}
		}
	}
	for (int col = 0; col < result.cols; col++) {
		for (int row = result.rows - 1; row > result.rows - 4; row--) {
			rowk = row % 3;
			colk = col % 3;
			top_startrow = row - 3 - rowk;
			top_startcol = col - colk;
			bottom_startrow = row - 3 - rowk;
			bottom_startcol = col - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_v[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(bottom_startrow + i, bottom_startcol + j)))*(weights_v[w][r++]));
				}
			}
		}
	}
	for (int row = 3; row < result.rows - 3; row++) {
		for (int col = 0; col < result.cols; col++) {
			rowk = row % 3;
			colk = col % 3;
			top_startrow = row - 3 - rowk;
			top_startcol = col - colk;
			bottom_startrow = row + 3 - rowk;
			bottom_startcol = col - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(top_startrow + i, top_startcol + j)))*(weights_v[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(bottom_startrow + i, bottom_startcol + j)))*(weights_v[w][r++]));
				}
			}
		}
	}
	return result;
}
void distance_generator(vector<vector<double>> &distance,bool horizontalflag){
	int rowk,colk;
	int rowk2,colk2;
	int sum;
	for(int i=0;i<distance.size();i++){
		colk = i % 3;
		rowk = i / 3;
		sum = 0;
		for(int j=0;j<9;j++){
			colk2 = j % 3;
			rowk2 = j / 3;
			if(horizontalflag){
				if(colk2<=colk){
					sum = abs(colk2-colk) + abs(rowk2-rowk) + 3;
				}
				else{
					sum = 3 - abs(colk2-colk) + abs(rowk2-rowk);
				} 
			}
			else{
				sum = abs(colk2-colk) + abs(rowk2-rowk);
				if(rowk2<=rowk){
					sum = abs(colk2-colk) + abs(rowk2-rowk) + 3;
				}
				else{
					sum = 3 - abs(rowk2-rowk) + abs(colk2-colk);
				}
			}
			distance[i][j] = sum;
		}
		for(int j=9;j<18;j++){
			colk2 = (j-9) % 3;
			rowk2 = (j-9) / 3;
			if(horizontalflag){
				if(colk2>=colk){
					sum = abs(colk2-colk) + abs(rowk2-rowk) + 3;
				}
				else{
					sum = 3 - abs(colk2-colk) + abs(rowk2-rowk);
				} 
			}
			else{
				sum = abs(colk2-colk) + abs(rowk2-rowk);
				if(rowk2>=rowk){
					sum = abs(colk2-colk) + abs(rowk2-rowk) + 3;
				}
				else{
					sum = 3 - abs(rowk2-rowk) + abs(colk2-colk);
				}
			}
			distance[i][j] = sum;
		}
	}
}
void weight_generator(vector<vector<double>> &weights, vector<vector<double>> &distance, double k ) {
	double sum = 0;
	double factor = 1;
	for(int i=0;i<weights.size();i++){
		sum = 0;
		for(int j=0;j<weights[0].size();j++){
			sum += pow(distance[i][j],k);
		}
		factor = 1.00/sum;
		for(int j=0;j<weights[0].size();j++){
			weights[i][j] = factor*pow(distance[i][j],k);
		}
	}
	return;
}
Mat method4_h(Mat Src1ch, float k) {
	Mat result = Mat::zeros(Src1ch.rows, Src1ch.cols, CV_32F);
	int colend = (Src1ch.cols / 3) - 1;
	int rowk;
	int colk;
	int left_startrow,left_startcol,right_startrow,right_startcol;
	int l,r,w;
	vector<vector<double>> weights_h( 9 , vector<double> (18, 0));
	vector<vector<double>> dist_h( 9 , vector<double> (18, 0));
	distance_generator(dist_h,1);
	weight_generator(weights_h,dist_h,k);
	for (int row = 0; row < result.rows; row++) {
		for (int col = 0; col < 3; col++) {
			rowk = row % 3;
			colk = col % 3;
			left_startrow = row  - rowk;
			left_startcol = col + 3 - colk;
			right_startrow = row - rowk;
			right_startcol = col + 3 - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_h[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(right_startrow + i, right_startcol + j)))*(weights_h[w][r++]));
				}
			}
		}
	}
	for (int row = 0; row < result.rows; row++) {
		for (int col = result.cols - 1; col > result.cols - 4; col--) {
			rowk = row % 3;
			colk = col % 3;
			left_startrow = row  - rowk;
			left_startcol = col - 3 - colk;
			right_startrow = row - rowk;
			right_startcol = col - 3 - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_h[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(right_startrow + i, right_startcol + j)))*(weights_h[w][r++]));
				}
			}
		}
	}
	for (int row = 0; row < result.rows; row++) {
		for (int col = 3; col < result.cols - 3; col++) {
			rowk = row % 3;
			colk = col % 3;
			left_startrow = row - rowk;
			left_startcol = col - 3 - colk;
			right_startrow = row - rowk;
			right_startcol = col + 3 - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_h[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(right_startrow + i, right_startcol + j)))*(weights_h[w][r++]));
				}
			}
		}
	}
	return result;
}
Mat method4_v(Mat Src1ch, float k) {
	Mat result = Mat::zeros(Src1ch.rows, Src1ch.cols, CV_32F);
	int rowend = (Src1ch.rows / 3) - 1;
	int rowk;
	int colk;
	int left_startrow,left_startcol,right_startrow,right_startcol;
	int l,r,w;
	vector<vector<double>> weights_v( 9 , vector<int> (18, 0));
	distancegenerator(dist_h,0);
	vector<vector<double>> dist_v( 9 , vector<int> (18, 0));
	weightgenerator(weights_v,dist_v,k);
	for (int col = 0; col < result.cols; col++) {
		for (int row = 0; row < 3; row++) {
			rowk = row % 3;
			colk = col % 3;
			top_startrow = row + 3 - rowk;
			top_startcol = col - colk;
			bottom_startrow = row + 3 - rowk;
			bottom_startcol = col - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(top_startrow + i, top_startcol + j)))*(weights_v[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(bottom_startrow + i, bottom_startcol + j)))*(weights_v[w][r++]));
				}
			}
		}
	}
	for (int col = 0; col < result.cols; col++) {
		for (int row = result.rows - 1; row > result.rows - 4; row--) {
			rowk = row % 3;
			colk = col % 3;
			top_startrow = row - 3 - rowk;
			top_startcol = col - colk;
			bottom_startrow = row - 3 - rowk;
			bottom_startcol = col - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(left_startrow + i, left_startcol + j)))*(weights_v[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(bottom_startrow + i, bottom_startcol + j)))*(weights_v[w][r++]));
				}
			}
		}
	}
	for (int row = 3; row < result.rows - 3; row++) {
		for (int col = 0; col < result.cols; col++) {
			rowk = row % 3;
			colk = col % 3;
			top_startrow = row - 3 - rowk;
			top_startcol = col - colk;
			bottom_startrow = row + 3 - rowk;
			bottom_startcol = col - colk;
			l = 0;
			r = 9;
			w = (rowk * 3)  + colk;
			for(int i = 0;i < 3;i++){
				for(int j = 0; j< 3;j++){
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(top_startrow + i, top_startcol + j)))*(weights_v[w][l++]));
					result.at<uchar>(row, col) += ((Src1ch.at<uchar>(bottom_startrow + i, bottom_startcol + j)))*(weights_v[w][r++]));
				}
			}
		}
	}
	return result;
}

void toSingleChannelnona(cv::Mat& src, cv::Mat& dst) {
	if (src.channels() != 3) {
		std::cerr << "to_SingleChannel need 3 channel image" << std::endl;
		return;
	}
	dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	int channelNum;
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			int rowk = row / 3;
			int colk = col / 3;
			if (rowk % 2 == 0) { // opencv: BGR
				channelNum = (colk % 2 == 0) ? 2 : 1;//even rows -- even cols:R=channel:2; odd cols:G=channel:1 
			}
			else {
				channelNum = (colk % 2 == 0) ? 1 : 0;// odd rowd -- even cols:G=channel:1; odd cols:B=channel:0
			}
			dst.at<uchar>(row, col) = src.at<Vec3b>(row, col).val[channelNum];
		}
	}
	return;
}
//   0 1 
// 0 R G
// 1 G B
void bayer_splitnona(cv::Mat& Bayer, cv::Mat& Dst) {
	if (Bayer.channels() != 1) {
		std::cerr << "bayer_split allow only 1 channel raw bayer image " << std::endl;
		return;
	}
	Dst = cv::Mat::zeros(Bayer.rows, Bayer.cols, CV_8UC3);
	int channelNum;

	for (int row = 0; row < Bayer.rows; row++) {
		for (int col = 0; col < Bayer.cols; col++) {
			int rowk = row / 3;
			int colk = col / 3;
			if (rowk % 2 == 0) { // opencv: BGR
				channelNum = (colk % 2 == 0) ? 2 : 1;//even rows -- even cols:R=channel:2; odd cols:G=channel:1 
			}
			else {
				channelNum = (colk % 2 == 0) ? 1 : 0;// odd rowd -- even cols:G=channel:1; odd cols:B=channel:0
			}
			Dst.at<Vec3b>(row, col).val[channelNum] = Bayer.at<uchar>(row, col);
		}
	}
	return;
}
void bayer_masknona(cv::Mat& Bayer, cv::Mat& Dst) {
	Mat temp = cv::Mat::ones(Bayer.size(), CV_8U);
	bayer_splitnona(temp, Dst);// or split(Bayer, temp)
}

void demosaic_nona(cv::Mat& Bayer, cv::Mat& Dst, float sigma = 1.0, float k = 1.0) {
	cv::Mat Src = Bayer.clone();
	if (Bayer.channels() == 1) { //input 1 channel -> 3 channel Bayer
		bayer_splitnona(Bayer, Src); // or split(Bayer, Src);
	}

	Mat Src1ch;
	toSingleChannelnona(Src, Src1ch); // or merge(Src, Src1ch);
	Src1ch.convertTo(Src1ch, CV_32F, 1.0 / 255.0); //normalize

	Mat tempMask;
	bayer_masknona(Src, tempMask);
	tempMask.convertTo(tempMask, CV_32F); //normalize
	vector<Mat> mask(3);
	split(tempMask, mask);
	tempMask.release();

	// split channel to BGR 
	Src.convertTo(Src, CV_32F, 1.0 / 255.0); //normalize
	vector<Mat> bgr(3);
	vector<Mat> finalBGR(3);
	split(Src, bgr);
	split(Src, finalBGR);

	// ==== 1.Green interpolation ===
	Mat maskGr = Mat::zeros(Src.rows, Src.cols, CV_32F);
	Mat maskGb = Mat::zeros(Src.rows, Src.cols, CV_32F);
	for (int row = 0; row < Src.rows; row++) {
		int col = 0;
		float* targetGr = maskGr.ptr<float>(row);
		float* targetGb = maskGb.ptr<float>(row);
		int rowk = row / 3;
		if (rowk % 2 == 0) {
			col = 3;
		}
		for (; col < Src.cols; col += 6) {
			if (rowk % 2 == 0) { // R G
				for (int i = 0; i < 3; i++) {
					targetGr[col + i] = 1.0;
				}
			}
			else { //G B
				for (int i = 0; i < 3; i++) {
					targetGb[col + i] = 1.0;
				}
			}
		}
	}

	float VHkernel[3] = { 0.5, 0, 0.5 }; //bilinear interpolation at 1D
	cv::Mat HK(1, 3, CV_32F, VHkernel);
	cv::Mat VK(3, 1, CV_32F, VHkernel);
	Mat rawH = method4_h(Src1ch,k);
	Mat rawV = method4_v(Src1ch,k);


	// Creating guide images
	Mat GuideG_H = bgr[1] + rawH.mul(mask[2]) + rawH.mul(mask[0]);
	Mat GuideR_H = bgr[2] + rawH.mul(maskGr);
	Mat GuideB_H = bgr[0] + rawH.mul(maskGb);
	Mat GuideG_V = bgr[1] + rawV.mul(mask[2]) + rawV.mul(mask[0]);
	Mat GuideR_V = bgr[2] + rawV.mul(maskGb);
	Mat GuideB_V = bgr[0] + rawV.mul(maskGr);



	// Tentative image
	int h = 5; //horizontal
	int v = 0; //vertical
	float eps = 0;
	Mat tentativeR_H = guided_filter_modified(bgr[2], GuideG_H, mask[2], h, v, eps);
	Mat tentativeGr_H = guided_filter_modified(bgr[1].mul(maskGr), GuideR_H, maskGr, h, v, eps);// need mul(mask) because Green has two location
	Mat tentativeGb_H = guided_filter_modified(bgr[1].mul(maskGb), GuideB_H, maskGb, h, v, eps);
	Mat tentativeB_H = guided_filter_modified(bgr[0], GuideG_H, mask[0], h, v, eps);
	// vertical part
	Mat tentativeR_V = guided_filter_modified(bgr[2], GuideG_V, mask[2], v, h, eps);
	Mat tentativeGr_V = guided_filter_modified(bgr[1].mul(maskGb), GuideR_V, maskGb, v, h, eps); //Gr <-> Gb
	Mat tentativeGb_V = guided_filter_modified(bgr[1].mul(maskGr), GuideB_V, maskGr, v, h, eps);
	Mat tentativeB_V = guided_filter_modified(bgr[0], GuideG_V, mask[0], v, h, eps);

	//release Guided Image
	GuideG_H.release();
	GuideR_H.release();
	GuideB_H.release();
	GuideG_V.release();
	GuideR_V.release();
	GuideB_V.release();

	// Residual
	Mat residualGr_H = (bgr[1] - tentativeGr_H).mul(maskGr);
	Mat residualGb_H = (bgr[1] - tentativeGb_H).mul(maskGb);
	Mat residualR_H = (bgr[2] - tentativeR_H).mul(mask[2]);
	Mat residualB_H = (bgr[0] - tentativeB_H).mul(mask[0]);
	Mat residualGr_V = (bgr[1] - tentativeGr_V).mul(maskGb);
	Mat residualGb_V = (bgr[1] - tentativeGb_V).mul(maskGr);
	Mat residualR_V = (bgr[2] - tentativeR_V).mul(mask[2]);
	Mat residualB_V = (bgr[0] - tentativeB_V).mul(mask[0]);

	// Residual interpolation
	residualGr_H = method4_h(residualGr_H,k);
	residualGb_H = method4_h(residualGb_H,k);
	residualR_H = method4_h(residualR_H,k);
	residualB_H = method4_h(residualB_H,k);
	residualGr_V = method4_v(residualGr_V,k);
	residualGb_V = method4_v(residualGb_V,k);
	residualR_V = method4_v(residualR_V,k);
	residualB_V = method_v(residualB_V,k);


	// Add tentative image
	Mat Gr_H = (tentativeGr_H + residualGr_H).mul(mask[2]);
	Mat Gb_H = (tentativeGb_H + residualGb_H).mul(mask[0]);
	Mat R_H = (tentativeR_H + residualR_H).mul(maskGr);
	Mat B_H = (tentativeB_H + residualB_H).mul(maskGb);
	Mat Gr_V = (tentativeGr_V + residualGr_V).mul(mask[2]);
	Mat Gb_V = (tentativeGb_V + residualGb_V).mul(mask[0]);
	Mat R_V = (tentativeR_V + residualR_V).mul(maskGb);
	Mat B_V = (tentativeB_V + residualB_V).mul(maskGr);

	// Vertical and horizontal color difference 
	Mat dif_H = bgr[1] + Gr_H + Gb_H - bgr[2] - bgr[0] - R_H - B_H;
	Mat dif_V = bgr[1] + Gr_V + Gb_V - bgr[2] - bgr[0] - R_V - B_V;

	//Release the rest
	tentativeR_H.release();
	tentativeGr_H.release();
	tentativeGb_H.release();
	tentativeB_H.release();
	tentativeR_V.release();
	tentativeGr_V.release();
	tentativeGb_V.release();
	tentativeB_V.release();

	residualGr_H.release();
	residualGb_H.release();
	residualR_H.release();
	residualB_H.release();
	residualGr_V.release();
	residualGb_V.release();
	residualR_V.release();
	residualB_V.release();

	Gr_H.release();
	Gb_H.release();
	R_H.release();
	B_H.release();
	Gr_V.release();
	Gb_V.release();
	R_V.release();
	B_V.release();

	// Combine Vertical and Horizontal Color Differences
	// color difference gradient
	float difkernel[3] = { 1, 0, -1 };
	Mat V_diff_gradient, H_diff_gradient;
	H_diff_gradient = diffilter_H(dif_H);
	V_diff_gradient = diffilter_V(dif_V);


	H_diff_gradient = cv::abs(H_diff_gradient);
	V_diff_gradient = cv::abs(V_diff_gradient);

	// Directional weight
	Mat K = Mat::ones(5, 5, CV_32F);
	Mat hWeightSum, vWeightSum;
	filter2D(H_diff_gradient, hWeightSum, -1, K);
	filter2D(V_diff_gradient, vWeightSum, -1, K);
	Mat Wkernel = (Mat_<float>(1, 5) << 1, 0, 0, 0, 0);
	Mat Ekernel = (Mat_<float>(1, 5) << 0, 0, 0, 0, 1);
	Mat Nkernel = (Mat_<float>(5, 1) << 1, 0, 0, 0, 0);
	Mat Skernel = (Mat_<float>(5, 1) << 0, 0, 0, 0, 1);
	Mat wWeight, eWeight, nWeight, sWeight;
	filter2D(hWeightSum, wWeight, -1, Wkernel);
	filter2D(hWeightSum, eWeight, -1, Ekernel);
	filter2D(vWeightSum, nWeight, -1, Nkernel);
	filter2D(vWeightSum, sWeight, -1, Skernel);
	divide(1.0, (wWeight.mul(wWeight) + 1e-32), wWeight);
	divide(1.0, (eWeight.mul(eWeight) + 1e-32), eWeight);
	divide(1.0, (nWeight.mul(nWeight) + 1e-32), nWeight);
	divide(1.0, (sWeight.mul(sWeight) + 1e-32), sWeight);

	// combine directional color differences
	Mat weightedF = getGaussianKernel(9, sigma, CV_32F);
	Nkernel = (Mat_<float>(9, 1) << 1, 1, 1, 1, 1, 0, 0, 0, 0);
	Nkernel = Nkernel.mul(weightedF);
	float s = sum(Nkernel)[0];
	Nkernel /= s;
	Skernel = (Mat_<float>(9, 1) << 0, 0, 0, 0, 1, 1, 1, 1, 1);
	Skernel = Skernel.mul(weightedF) / s;
	transpose(Nkernel, Wkernel);
	transpose(Skernel, Ekernel);
	Mat fMulGradientSum_N, fMulGradientSum_S, fMulGradientSum_W, fMulGradientSum_E;
	filter2D(dif_V, fMulGradientSum_N, -1, Nkernel);
	filter2D(dif_V, fMulGradientSum_S, -1, Skernel);
	filter2D(dif_H, fMulGradientSum_W, -1, Wkernel);
	filter2D(dif_H, fMulGradientSum_E, -1, Ekernel);
	Mat totalWeight = nWeight + eWeight + wWeight + sWeight;
	Mat diff;
	divide(nWeight.mul(fMulGradientSum_N) + sWeight.mul(fMulGradientSum_S) + wWeight.mul(fMulGradientSum_W) + eWeight.mul(fMulGradientSum_E), totalWeight, diff);//(InputArray src1, InputArray src2, OutputArray dst, float scale=1, int dtype=-1)

	// Calculate Green by adding bayer raw data 
	finalBGR[1] = diff + Src1ch; //raw CFA data
	Mat imask = (mask[1] == 0); //[0, 1, 1] -> [255, 0, 0]
	imask.convertTo(imask, CV_32F, 1.0 / 255.0);
	finalBGR[1] = finalBGR[1].mul(imask) + bgr[1];

	// clip to 0~1

	threshold(finalBGR[1], finalBGR[1], 1.0, 1.0, THRESH_TRUNC); // > 1 to 1
	threshold(finalBGR[1], finalBGR[1], 0.0, 0.0, THRESH_TOZERO); // < 0 to 0

	// === 2.Red and Blue ===
	h = 5; //horizontal
	v = 5; //vertical
	eps = 0.0;

	// R interpolation
	Mat tentativeR = guided_filter_modified(bgr[2], finalBGR[1], mask[2], h, v, eps);
	Mat residualR = (bgr[2] - tentativeR).mul(mask[2]);
	Mat bilinearKernel = (Mat_<float>(3, 3) << 0.25, 0.5, 0.25, 0.5, 1.0, 0.5, 0.25, 0.5, 0.25);
	filter2D(residualR, residualR, -1, bilinearKernel);
	finalBGR[2] = residualR + tentativeR;

	// B interpolation
	Mat tentativeB = guided_filter_modified(bgr[0], finalBGR[1], mask[0], h, v, eps);
	Mat residualB = (bgr[0] - tentativeB).mul(mask[0]);
	filter2D(residualB, residualB, -1, bilinearKernel);
	finalBGR[0] = residualB + tentativeB;
	// Merge to single 3 channel Img
	merge(finalBGR, Dst);
	Dst.convertTo(Dst, CV_8U, 255.0);
}