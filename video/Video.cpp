#include "Video.h"

cv::Vec3f Center_and_Size(const cv::Mat & frame, int minR, int maxR, int minG, int maxG, int minB, int maxB, float A_W, float D, float E)

{




	// Check for invalid input
	if (!frame.data)
		std::cout << "Could not open or find the image" << std::endl;

	//Mat nonZeroFrame;
	//findNonZero(frame, nonZeroFrame);
	cv::Mat frame1(frame);
	
	std::vector<cv::Mat> channels;


	//размываем
	cv::GaussianBlur(frame1, frame1, cv::Size(9, 9), 2);

	//разбиваем по каналам
	cv::split(frame1, channels);

	//делаем маски оп цветам
	//Blue
	cv::threshold(channels[0], channels[0], maxB, 255, cv::THRESH_TOZERO_INV);
	cv::threshold(channels[0], channels[0], minB, 255, cv::THRESH_BINARY);

	//Green
	cv::threshold(channels[1], channels[1], maxG, 255, cv::THRESH_TOZERO_INV);
	cv::threshold(channels[1], channels[1], minG, 255, cv::THRESH_BINARY);

	//Red
	cv::threshold(channels[2], channels[2], maxR, 255, cv::THRESH_TOZERO_INV);
	cv::threshold(channels[2], channels[2], minR, 255, cv::THRESH_BINARY);


	cv::Mat res;
	// соединяем маски
	res = channels[0] & channels[1] & channels[2];
	//размываем контуры маски
	cv::GaussianBlur(res, res, cv::Size(15, 15), 2);
	//преобразовываем размытую маску обратно к маске
	res = res > 0;

	//
	//
	/*cv::namedWindow("original", CV_WINDOW_AUTOSIZE);
	cv::waitKey(0);
	cv::imshow("frame1", frame1);
	cv::imshow("maskB", channels[0]);
	cv::imshow("maskG", channels[1]);
	cv::imshow("maskR", channels[2]);
	cv::imshow("res", res);
	cvDestroyWindow("original");
	*/
	std::vector<std::vector<cv::Point> > contours;

	// ищем контуры
	findContours(res.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//находим максимальный по площади контур 
	int id = -1;
	double max_area = 0;
	for (int i = 0; i<contours.size(); ++i)
	{
		double area = cv::contourArea(contours[i]);
		if (area > max_area)
		{
			id = i;
			area = max_area;
		}
	}
	if (id < 0)
	{
		throw std::out_of_range("there is not any cnotour");
	}

	cv::Rect rect = cv::boundingRect(cv::Mat(contours[id]));

	float B_H = A_W / frame1.cols * frame.rows;

	float K_width = A_W / frame1.cols, K_height = B_H / frame1.rows;
	float X = (-frame1.cols / 2.0) + (rect.x + (rect.width / 2.0)), Y = (frame1.rows / 2.0) - (rect.y - (rect.height / 2.0));
	
	float K_Z = (D) / (frame1.cols + frame1.rows); //(2.0 * D) / (frame1.cols + frame1.rows);
	float Z = K_Z * (rect.height + rect.width); //(rect.height + rect.width) / 2.0;
	

	if (Z > E)
		return cv::Vec3f( X, Y, Z);
	else 
		return cv::Vec3f(X, Y, E);
}

double Speed(double FPS, double x1, double y1, double x, double y)
{
	
	double p = std::pow((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y), 0.5), time = 2 / FPS;
	return p / time;
}

