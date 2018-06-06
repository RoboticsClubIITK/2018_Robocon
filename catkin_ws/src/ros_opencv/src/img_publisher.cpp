#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <stdio.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "img_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/image", 1);

    cv::VideoCapture capture(0);
    cv::Mat frame;
    sensor_msgs::ImagePtr msg;

    ros::Rate loop_rate(5);
    while(nh.ok())
    {
        capture >> frame;
        if(!frame.empty())
        {
            msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
            pub.publish(msg);
            printf("publishing image\n");

            cv::waitKey(1);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
}