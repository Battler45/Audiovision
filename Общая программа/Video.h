#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


#include <opencv/cv.h>
//#include <opencv2/highgui.h>
#include <iostream>


//#include <opencv2/core.hpp>
//#include <opencv2/core/utility.hpp>

//#include <opencv2/calib3d.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

//using namespace cv;
/*
vector<Vec3i> Centers_and_Sizes(string filename = "adv_time/test2.mp4", int minR = 110, int maxR = 170, int minG = 180, int maxG = 230, int minB = 150, int maxB = 220)
{
vector<Vec3i> result;

Mat frame;

// окно для отображения картинки
//namedWindow("original", CV_WINDOW_AUTOSIZE);

// получаем информацию о видео-файле
VideoCapture capture(filename);


while (capture.read(frame)) {


vector<Mat> channels;


//размываем
GaussianBlur(frame, frame, Size(9, 9), 2);

//разбиваем по каналам
split(frame, channels);

//делаем маски оп цветам
//Blue
threshold(channels[0], channels[0], maxB, 255, THRESH_TOZERO_INV);
threshold(channels[0], channels[0], minB, 255, THRESH_BINARY);

//Green
threshold(channels[1], channels[1], maxG, 255, THRESH_TOZERO_INV);
threshold(channels[1], channels[1], minG, 255, THRESH_BINARY);

//Red
threshold(channels[2], channels[2], maxR, 255, THRESH_TOZERO_INV);
threshold(channels[2], channels[2], minR, 255, THRESH_BINARY);


Mat res;
// соединяем маски
res = channels[0] & channels[1] & channels[2];
//размываем контуры маски
GaussianBlur(res, res, Size(15, 15), 2);
//преобразовываем размытую маску обратно к маске
res = res > 0;


vector<vector<Point> > contours;

// ищем контуры
findContours(res.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

//находим максимальный по площади контур
int id = -1;
double max_area = 0;
for (int i = 0; i<contours.size(); ++i)
{
double area = contourArea(contours[i]);
if (area > max_area)
{
id = i;
area = max_area;
}
}
if (id < 0)
{
throw out_of_range("there is not any cnotour");
}

//создаем прямоугольник на основе максимального по площади контура
Rect rect = boundingRect(Mat(contours[id]));

;
;
;
// добавляем координаты центра, (height + width) /2
result.push_back(Vec3i(rect.x + (rect.width / 2), rect.y - (rect.height / 2), (rect.height + rect.width) / 2));



//rectangle(frame, rect, Scalar(0, 0, 255), 5);

//imshow("Hough Circle Transform Demo", frame);
//imshow("result", res);

/*
imshow("maskB", channels[0]);
imshow("maskG", channels[1]);
imshow("maskR", channels[2]);
*/
//cout << pow(circles[0][0] * circles[0][0] + circles[0][1] * circles[0][1], 0.5);
/*char c = waitKey(0);
if (c == 27) { // если нажата ESC - выходим
break;
}*/
/*
}

// удаляем окно
//destroyWindow("original");
return result;
}
*/

cv::Vec3f Center_and_Size(const cv::Mat & frame, int minR, int maxR, int minG, int maxG, int minB, int maxB, float A_W, float D, float E);

double Speed(double FPS, double x1, double y1, double x, double y);


