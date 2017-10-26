#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <math.h>
#include <Windows.h>
using namespace std;
using namespace cv;

//bool use_mask;
//Mat img; Mat templ; Mat mask; Mat result;
//const char* image_window = "Source Image";
//const char* result_window = "Result window";
//int match_method;
//int max_Trackbar = 5;
//void MatchingMethod(int, void*);
Mat CalDiffMap(const Mat & objImg, const Mat & refImg);
Mat cvCalDiffMap(const Mat & objImg, const Mat & refImg);
Mat cvSplitDiff(const Mat &objImg, const Mat &objImg_original, const Mat &refImg, int top, int left);

int main(int argc, char** argv)
{
	//if (argc < 3)
	//{
	//	cout << "Not enough parameters" << endl;
	//	cout << "Usage:\n./MatchTemplate_Demo <image_name> <template_name> [<mask_name>]" << endl;
	//	return -1;
	//}
	//img = imread(argv[1], IMREAD_COLOR);
	//templ = imread(argv[2], IMREAD_COLOR);
	//if (argc > 3) {
	//	use_mask = true;
	//	mask = imread(argv[3], IMREAD_COLOR);
	//}
	//if (img.empty() || templ.empty() || (use_mask && mask.empty()))
	//{
	//	cout << "Can't read one of the images" << endl;
	//	return -1;
	//}
	//namedWindow(image_window, WINDOW_AUTOSIZE);
	//namedWindow(result_window, WINDOW_AUTOSIZE);
	//const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	//createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
	//MatchingMethod(0, 0);

	//Mat M(2, 2, CV_8UC3, Scalar(0, 0, 255));
	//cout << "M= " << endl << " " << M << endl << endl;
	//M.create(4, 4, CV_8UC(2));
	//cout << "M= " << endl << " " << M << endl << endl;

	//Mat E = Mat::eye(4, 4, CV_64F);
	//cout << "E= " << endl << " " << E << endl << endl;

	//Mat O = Mat::ones(2, 2, CV_32F);
	//cout << "O= " << endl << " " << O << endl << endl;

	//Mat Z = Mat::zeros(3, 3, CV_8UC1);
	//cout << "Z= " << endl << " " << Z << endl << endl;

	//Mat C = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	//cout << "C= " << endl << " " << C << endl << endl;

	//Mat D = C(Range(0, 2), Range(0, 2));
	//cout << "D= " << endl << " " << D << endl << endl;

	//Mat RowClone = C.row(1).clone();
	//cout << "RowClone= " << endl << " " << RowClone << endl << endl;

	//Mat R = Mat(512, 512, CV_8UC3);
	//randu(R, Scalar::all(0), Scalar::all(255));
	////cout << "R= " << endl << " " << R << endl << endl;
	//imshow("R", R);

	//vector<Point2f> vPoints;
	//for (size_t i = 0; i < 20; i++)
	//	//vPoints[i] = Point2f((float)(i * 5), (float)(i % 7));
	//	vPoints.push_back(Point2f((float)(i * 5), (float)(i % 7)));
	//cout << "A vector of 2D Points= " << endl << vPoints << endl << endl;

	Mat objImg = imread("C:\\Users\\zdlco\\Documents\\MATLAB\\Image\\obj.bmp", CV_8U);
	Mat refImg = imread("C:\\Users\\zdlco\\Documents\\MATLAB\\Image\\ref.bmp", CV_8U);

	//��ʼ��ʱ
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);

	Mat diffImg = cvSplitDiff(objImg, objImg, refImg, 0, 0);
	Mat showImg;
	normalize(diffImg, showImg, -127, 127, NORM_MINMAX);

	showImg.convertTo(showImg, CV_8U, 1, 128);
	medianBlur(showImg, showImg, 5);

	imshow("diff", showImg);

	//ֹͣ��ʱ
	QueryPerformanceCounter(&t2);
	printf("time:%fms\n\n", (t2.QuadPart - t1.QuadPart)*1000.0 / tc.QuadPart);
	
	waitKey(0);
	system("pause");
	return 0;
}

//void MatchingMethod(int, void*)
//{
//	Mat img_display;
//	img.copyTo(img_display);
//	int result_cols = img.cols - templ.cols + 1;
//	int result_rows = img.rows - templ.rows + 1;
//	result.create(result_rows, result_cols, CV_32FC1);
//	bool method_accepts_mask = (CV_TM_SQDIFF == match_method || match_method == CV_TM_CCORR_NORMED);
//	if (use_mask && method_accepts_mask)
//	{
//		matchTemplate(img, templ, result, match_method, mask);
//	}
//	else
//	{
//		matchTemplate(img, templ, result, match_method);
//	}
//	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
//	double minVal; double maxVal; Point minLoc; Point maxLoc;
//	Point matchLoc;
//	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
//	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
//	{
//		matchLoc = minLoc;
//	}
//	else
//	{
//		matchLoc = maxLoc;
//	}
//	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(128),2, 8, 0);
//	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//	imshow(image_window, img_display);
//	imshow(result_window, result);
//	return;
//}

Mat cvCalDiffMap(const Mat &objImg, const Mat & refImg)
{
	Mat diffMap;          //ƫ��ͼ
	int cellsize = 19;    //���С
	int left = -20;       //�����ƫ����
	int right = 30;       //�����ƫ����
	double err_last = 0;  //�ϴ�ƥ������
	int step_last = 0;    //�ϴ�ƥ���ƫ��
	int m = objImg.rows;  //ͼƬ����
	int n = objImg.cols;  //ͼƬ����

	diffMap = Mat::zeros(m, n, CV_8S);

	//ѭ��ƥ��
	for (int i = (cellsize + 1) / 2; i <= m - (cellsize - 1) / 2; i++)
	{
		for (int j = (cellsize + 1) / 2; j <= n - (cellsize - 1) / 2; j++)
		{
			Mat cellMatrix = objImg(Range(i - (cellsize + 1) / 2, i + (cellsize - 1) / 2)
				, Range(j - (cellsize + 1) / 2, j + (cellsize - 1) / 2));//ģ�嵥Ԫ
			int beststep = 0;//���ƫ����
			double minerr = 100;//��С���
			int step;
			int x1, y1, x2, y2;
			double err;

			//opencvƥ���㷨�ñ���
			double minVal; double maxVal; Point minLoc; Point maxLoc;
			Point matchLoc;

			//������С��Χ����
			if (err_last != 0 && j>30 && j<1000)//��һ�β���С��Χ����
			{
				x1 = max(0, j + step_last - 3 - (cellsize + 1) / 2);
				y1 = max(0, i - (cellsize + 1) / 2);
				x2 = min(n, j + step_last + 3 + (cellsize - 1) / 2);
				y2 = min(m, i + (cellsize - 1) / 2);

				Mat locSubRefImg = refImg(Range(y1, y2), Range(x1, x2));
				int locResult_cols = locSubRefImg.cols - cellMatrix.cols + 1;
				int locResult_rows = locSubRefImg.rows - cellMatrix.rows + 1;
				Mat locResultImg;
				locResultImg.create(locResult_rows, locResult_cols, CV_32FC1);
				matchTemplate(locSubRefImg, cellMatrix, locResultImg, 0);
				normalize(locResultImg, locResultImg, 0, 1, NORM_MINMAX, -1, Mat());
				minMaxLoc(locResultImg, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
				matchLoc = minLoc;
				if (x1 == 0)
				{
					beststep = matchLoc.x - j + (cellsize + 1) / 2;
				}
				else
				{
					beststep = matchLoc.x + step_last - 3;
				}
				minerr = minVal;

				if (abs(abs(minerr) - abs(err_last)) < 0.00000001)
				{
					diffMap.at<char>(i, j) = beststep;
					step_last = beststep;
					err_last = minerr;
					continue;
				}
			}

			//���С��Χ�Ѳ���������ȫ��Χ����

			////��ʼ��ʱ
			//LARGE_INTEGER t1, t2, tc;
			//QueryPerformanceFrequency(&tc);
			//QueryPerformanceCounter(&t1);

			x1 = max(0, j + left - (cellsize + 1) / 2);
			y1 = max(0, i - (cellsize + 1) / 2);
			x2 = min(n, j + right + (cellsize - 1) / 2);
			y2 = min(m, i + (cellsize - 1) / 2);

			Mat subRefImg = refImg(Range(y1, y2), Range(x1, x2));
			int result_cols = subRefImg.cols - cellMatrix.cols + 1;
			int result_rows = subRefImg.rows - cellMatrix.rows + 1;
			Mat resultImg;
			resultImg.create(result_rows, result_cols, CV_32FC1);
			matchTemplate(subRefImg, cellMatrix, resultImg, 0);  //Ĭ��ʹ�÷���0
			normalize(resultImg, resultImg, 0, 1, NORM_MINMAX, -1, Mat());
			minMaxLoc(resultImg, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
			matchLoc = minLoc;
			if (x1 == 0)
			{
				beststep = matchLoc.x - j + (cellsize + 1) / 2;
			}
			else
			{
				beststep = matchLoc.x + left;
			}
			minerr = minVal;

			////ֹͣ��ʱ
			//QueryPerformanceCounter(&t2);
			//printf("time:%fms\n\n", (t2.QuadPart - t1.QuadPart)*1000.0 / tc.QuadPart);

			if (minerr > 0.000000005)//����ֱ�Ӹ�ֵ�ٽ�����ƫ��ֵ
			{
				diffMap.at<char>(i, j) = step_last;
			}
			else
			{
				diffMap.at<char>(i, j) = beststep;
				step_last = beststep;
				err_last = minerr;
			}
		}
		//printf("%d\n\n", i);
	}
	return diffMap;

}

Mat CalDiffMap(const Mat & objImg, const Mat & refImg)
{
	Mat diffMap;          //ƫ��ͼ
	int cellsize = 19;    //���С
	int left = -20;       //�����ƫ����
	int right = 30;       //�����ƫ����
	double err_last = 0;  //�ϴ�ƥ������
	int step_last = 0;    //�ϴ�ƥ���ƫ��
	int m = objImg.rows;  //ͼƬ����
	int n = objImg.cols;  //ͼƬ����

	diffMap = Mat::zeros(m, n, CV_8S);

	//ѭ��ƥ��
	for (int i = (cellsize + 1) / 2; i <= m - (cellsize - 1) / 2; i++)
	{
		for (int j = (cellsize + 1) / 2; j <= n - (cellsize - 1) / 2; j++)
		{
			Mat cellMatrix = objImg(Range(i - (cellsize + 1) / 2, i + (cellsize - 1) / 2)
				, Range(j - (cellsize + 1) / 2, j + (cellsize - 1) / 2));//ģ�嵥Ԫ
			int beststep = 0;//���ƫ����
			double minerr = 100;//��С���
			int step;
			int x1, y1, x2, y2;
			double err;

			//������С��Χ����
			if (err_last != 0)//��һ�β���С��Χ����
			{
				for (int minstep = -3; minstep <= 3; minstep++)//�����ƥ��㸡����Χ
				{
					step = minstep + step_last;
					x1 = j + step - (cellsize + 1) / 2;
					y1 = i - (cellsize + 1) / 2;
					x2 = j + step + (cellsize - 1) / 2;
					y2 = i + (cellsize - 1) / 2;
					if (x1 >= 0 && y1 >= 0 && x2 <= n && y2 <= m)
					{
						//ʹ��ŷʽ������������
						err = 0;
						for (int k = y1; k < y2; k++)
						{
							for (int l = x1; l < x2; l++)
							{
								err += pow(cellMatrix.at<uchar>(k - y1, l - x1) - refImg.at<uchar>(k, l), 2);
							}
						}
						err = err / (cellsize*cellsize * 255 * 255);

						//�ҵ���Сerr�����ҳ�����Ӧ��step
						if (err < minerr)
						{
							minerr = err;
							beststep = step;
						}
					}
				}
				if (abs(minerr - err_last) < 0.005)
				{
					diffMap.at<char>(i, j) = beststep;
					step_last = beststep;
					err_last = minerr;
					continue;
				}
			}

			//���С��Χ�Ѳ���������ȫ��Χ����
			minerr = 100;
			beststep = 0;
			for (int istep = left; istep <= right; istep++)
			{
				x1 = j + istep - (cellsize + 1) / 2;
				y1 = i - (cellsize + 1) / 2;
				x2 = j + istep + (cellsize - 1) / 2;
				y2 = i + (cellsize - 1) / 2;
				if (x1 >= 0 && y1 >= 0 && x2 <= n && y2 <= m)
				{
					//ʹ��ŷʽ������������
					err = 0;
					for (int k = y1; k < y2; k++)
					{
						for (int l = x1; l < x2; l++)
						{
							err += pow(cellMatrix.at<uchar>(k - y1, l - x1) - refImg.at<uchar>(k, l), 2);
						}
					}
					err = err / (cellsize*cellsize * 255 * 255);

					//�ҵ���Сerr�����ҳ�����Ӧ��step
					if (err < minerr)
					{
						minerr = err;
						beststep = istep;
					}
				}
			}

			if (minerr > 0.075)//����ֱ�Ӹ�ֵ�ٽ�����ƫ��ֵ
			{
				diffMap.at<char>(i, j) = step_last;
			}
			else
			{
				diffMap.at<char>(i, j) = beststep;
				step_last = beststep;
				err_last = minerr;
			}
		}
		//printf("%d\n\n", i);
	}
	return diffMap;
}


/***************ʹ���Ĳ������ѷ�����ƫ��*****************/
/***************��֧�ֳ���Ϊ2��N�η��ķ���***************
* objImg: ����ͼ��
* objImg_original: ԭ����ͼ��
* ref: �ο�ͼ��
* top: ��obj���϶�����ref�е�����
* left: ��obj���󶨵���ref�е�����*/
Mat cvSplitDiff(const Mat &objImg, const Mat &objImg_original, const Mat &refImg, int top, int left)
{
	Mat diffMap;
	int leftmax = -20;
	int rightmax = 30;
	int mobj = objImg.rows;
	int nobj = objImg.cols;
	int nref = refImg.cols;
	int mref = refImg.rows;
	int beststep = 0;//���ƫ����
	double minerr = 100;//��С���

	//opencvƥ���㷨�ñ���
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	/* ʹ��OpenCV��������ƥ��� */
	int y1 = top;
	int y2 = top + mobj;
	int x1 = max(0, left + leftmax);
	int x2 = min(nref, left + nobj + rightmax);
	Mat subRefImg = refImg(Range(y1, y2), Range(x1, x2));
	int result_cols = subRefImg.cols - objImg.cols + 1;
	int result_rows = subRefImg.rows - objImg.rows + 1;
	Mat resultImg;
	resultImg.create(result_rows, result_cols, CV_32FC1);
	matchTemplate(subRefImg, objImg, resultImg, 0);  //Ĭ��ʹ�÷���0
	normalize(resultImg, resultImg, 0, 1, NORM_MINMAX, -1, Mat());
	minMaxLoc(resultImg, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	if (x1 == 0)
	{
		beststep = matchLoc.x + left;
	}
	else
	{
		beststep = matchLoc.x + leftmax;
	}
	minerr = minVal;

	///* ʹ��ŷʽ�����������ƥ��� */
	//Mat nmdRef, nmdObj;
	//refImg.convertTo(nmdRef, CV_64F);
	//objImg.convertTo(nmdObj, CV_64F);
	//normalize(nmdRef, nmdRef, 0, 1, NORM_MINMAX, -1, Mat());
	//normalize(nmdObj, nmdObj, 0, 1, NORM_MINMAX, -1, Mat());

	//for (int step = leftmax; step <= rightmax; step++)
	//{
	//	if ((left + step) >= 0 && (left + nobj + step) <= nref)
	//	{
	//		int y1 = top;
	//		int y2 = top + mobj;
	//		int x1 = left + step;
	//		int x2 = left + nobj + step;

	//		double err = 0;
	//		for (int i = y1; i < y2; i++)
	//		{
	//			for (int j = x1; j < x2; j++)
	//			{
	//				double pixeldiff = nmdObj.at<double>(i - y1, j - x1) - nmdRef.at<double>(i, j);
	//				err += pow(pixeldiff, 2) / (mobj*nobj);
	//			}
	//		}

	//		//�ҳ���Сerr�����ҳ�����Ӧ��step
	//		if (err < minerr)
	//		{
	//			minerr = err;
	//			beststep = step;
	//		}
	//	}
	//}


	/* ��������ƥ���ѡ����һ������ */
	if (mobj<=120 && abs(minerr) < 0.000000005)//minerr����ָ�ֵ������������
	{
		//����ƥ��ȸߣ�ֱ�����
		diffMap = beststep * Mat::ones(mobj, nobj, CV_8S);//�˴�����ʹ��CV_8U��������Ϣ��ʧ
	}
	else if (mobj <= 15)
	{
		//ƥ��ȵͣ������ٷ��ѣ�ֱ�Ӿ�ϸ����
		int cellsize = 19;//���С
		int err_last = 0; //�ϴ�ƥ������
		int step_last = 0;//�ϴ�ƥ���ƫ����
		diffMap.create(mobj, nobj, CV_8S);
		for (int i = top; i < top + mobj; i++)
		{
			for (int j = left; j < left + nobj; j++)
			{
				int oy1 = i - (cellsize + 1) / 2;
				int oy2 = i + (cellsize - 1) / 2;
				int ox1 = j - (cellsize + 1) / 2;
				int ox2 = j + (cellsize - 1) / 2;
				int rx1 = max(0, ox1 + leftmax);
				int rx2 = min(nref, ox2 + rightmax);
				if (oy1 >= 0 && oy2 <= mref && ox1 >= 0 && ox2 <= nref)
				{
					Mat cellMatrix = objImg_original(Range(oy1, oy2), Range(ox1, ox2));
					Mat searchRefImg = refImg(Range(oy1, oy2), Range(rx1, rx2));
					int r_cols = searchRefImg.cols - cellMatrix.cols + 1;
					int r_rows = searchRefImg.rows - cellMatrix.rows + 1;
					Mat searchImg;
					searchImg.create(r_rows, r_cols, CV_64FC1);
					matchTemplate(searchRefImg, cellMatrix, searchImg, 0);
					normalize(searchImg, searchImg, 0, 1, NORM_MINMAX, -1, Mat());
					minMaxLoc(searchImg, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
					matchLoc = minLoc;
					if (rx1 == 0)
					{
						beststep = matchLoc.x + leftmax + ox1;
					}
					else
					{
						beststep = matchLoc.x + leftmax;
					}
					minerr = minVal;

					diffMap.at<uchar>(i - top, j - left) = beststep;
				}
			}
		}
	}
	else
	{
		//���ѳ��Ŀ�
		int curm = mobj / 2;
		int curn = nobj / 2;
		int top1 = top;
		int left1 = left;
		int top2 = top;
		int left2 = left + curn;
		int top3 = top + curm;
		int left3 = left;
		int top4 = top + curm;
		int left4 = left + curn;

		Mat subobj1 = objImg(Range(0, curm), Range(0, curn));
		Mat subobj2 = objImg(Range(0, curm), Range(left2 - left1, left2 - left1 + curn));
		Mat subobj3 = objImg(Range(top3 - top1, top3 - top1 + curm), Range(0, curn));
		Mat subobj4 = objImg(Range(top4 - top2, top4 - top2 + curm), Range(left4 - left3, left4 - left3 + curn));
		
		Mat diffMap1 = cvSplitDiff(subobj1, objImg_original, refImg, top1, left1);
		Mat diffMap2 = cvSplitDiff(subobj2, objImg_original, refImg, top2, left2);
		Mat diffMap3 = cvSplitDiff(subobj3, objImg_original, refImg, top3, left3);
		Mat diffMap4 = cvSplitDiff(subobj4, objImg_original, refImg, top4, left4);

		Mat hDiff1, hDiff2;
		hconcat(diffMap1, diffMap2, hDiff1);
		hconcat(diffMap3, diffMap4, hDiff2);
		vconcat(hDiff1, hDiff2, diffMap);
	}
	printf("process:%f%%\n", 100*(top+mobj+nobj + left) / (double)(mref+nref));
	return diffMap;
}