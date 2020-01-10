//얼굴이 인식되면 캡쳐되게 한다. 하지만 얼굴 인식하는 성능이 좋은 것 같지 않다.
#include <iostream>
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

    face_classifier.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml");
    //eye_classifier.load("C:/opencv/sources/data/haarcascades/haarcascade_eye.xml");
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
                face_classifier.detectMultiScale(grayframe, faces,
                    1.1, // increase search scale by 10% each pass
                    3,   // merge groups of three detections
                    0,
                    Size(30, 30));
                //eye_classifier.detectMultiScale(grayframe, eyes,
                //    1.1, // increase search scale by 10% each pass
                //    3,   // merge groups of three detections
                //    0,
                //    Size(30, 30));
                if (faces.size() == 1)
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
                }
                
                //if (eyes.size() == 2)
                //{
                //    for (int i = 0; i < eyes.size(); i++) {

                //        Point lb(eyes[i].x + eyes[i].width, eyes[i].y + eyes[i].height);
                //        Point tr(eyes[i].x, eyes[i].y);
                //        rectangle(frame, tr, lb, Scalar(0, 0, 255));

                //    }
                //}
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

