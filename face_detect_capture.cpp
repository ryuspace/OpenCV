//눈이 인식되면 현재 웹캠을 캡쳐한다. 눈을 인식하는 것이 얼굴을 인식하는 것 보다 검출률이 더 좋다.
//Issue : 조금 더 얼굴인식을 정확하게 할 방법을 고려해보자.
//해결 : 1.최솟값과 최댓값의 차이를 구해 비율을 정한 후 너무 밝은 이미지는 전반적으로 어둡게, 너무 어두운 이미지는 전반적으로 밝게 보정해줘서
//       정확성을 높였다.
//       2.간간히 눈의 좌표가 말이 안되는 곳에 잡힐 때가 있다. 우선 발견한 것은 눈을 수직으로 인식한다. 눈의 y좌표가 40이상이면 검출이 
//       안되게 했다.
//         
#include <iostream
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#pragma warning(disable: 4996)

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{

    int a = 0;
    VideoCapture capture(0);
    char buf[256];
    int index = 0;
    if (!capture.isOpened()) {

        std::cerr << "Could not open camera" << std::endl;
        return -1;

    }

    namedWindow("webcam", 1);
    CascadeClassifier face_classifier, eye_classifier;

    //face_classifier.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml");
    eye_classifier.load("C:/opencv/sources/data/haarcascades/haarcascade_eye.xml");
    Mat frame;
    while (1) {

        bool frame_valid = true;
        try {
            // get a new frame from webcam
            capture >> frame;
        }
        catch (Exception & e) {
            cerr << "Exception occurred. Ignoring frame... " << e.err << std::endl;
            frame_valid = false;
        }
        if (frame_valid) {
            try {

                Mat grayframe;
                cvtColor(frame, grayframe, cv::COLOR_BGR2GRAY);
                equalizeHist(grayframe, grayframe);
                vector<Rect> faces,eyes;
                double minval, maxval;
                minMaxIdx(frame, &minval, &maxval);
                double ratio = (maxval - minval) / 255.0;
                frame = (frame-minval)/ratio;
                //화소의 최댓값과 최솟값의 차이를 구한다.
                //만약 ratio가 1보다 작으면 전체적으로 어두운 거고 1보다 크면 전체적으로 밝은 것.
                //이 구한 ratio로 나눠 화면을 보정해준다. 화면이 전체적으로 어두운 경우 전체적으로 밝아지는 효과를 볼 수 있다.

                //face_classifier.detectMultiScale(grayframe, faces,
                //    1.1, // increase search scale by 10% each pass
                //    3,   // merge groups of three detections
                //    0,
                //    Size(30, 30));

                eye_classifier.detectMultiScale(grayframe, eyes,
                    1.1, // increase search scale by 10% each pass
                    3,   // merge groups of three detections
                    0,
                    Size(30, 30));
                if (eyes.size() == 2)
                {
                    capture.read(frame);
                    sprintf(buf, "c:/temp/img_%06d.jpg", index);
                    cout << buf << '\n';
                    imwrite(buf, frame);
                    index++;
                    if (abs(eyes[0].y - eyes[1].y) <= 40)//테스트 해본 결과 고개를 돌려도 눈의 y좌표가 50이상 차이날 수 없다.
                    {
                        for (int i = 0; i < eyes.size(); i++) {

                            Point lb(eyes[i].x + eyes[i].width, eyes[i].y + eyes[i].height);
                            Point tr(eyes[i].x, eyes[i].y);
                            cout << i << " " << " " << eyes[i].x << " " << eyes[i].y << '\n';
                            rectangle(frame, tr, lb, Scalar(0, 0, 255));

                        }
                    }
                    
                }
                /*if (faces.size() == 1)
                {
                    capture.read(frame);
                    sprintf(buf, "c:/temp/img_%06d.jpg", index);
                    cout << buf << '\n';
                    imwrite(buf, frame);
                    index++;
                    for (int i = 0; i < faces.size(); i++) {

                        Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
                        Point tr(faces[i].x, faces[i].y);
                        rectangle(frame, tr, lb, Scalar(0, 255, 0));

                    }
                }*/
                
                imshow("webcam", frame);
                //writer.write(frame);


            }

            catch (Exception & e) {

                cerr << "Exception occurred. Ignoring frame... " << e.err << std::endl;

            }
        }

        if (waitKey(30) >= 0) break;

    }

    return 0;

}


