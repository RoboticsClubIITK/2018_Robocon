#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <math.h>

float angle;

int lSpeed = 3;
int lDirn1 = 4;
int lDirn2 = 7;
int rSpeed = 11;
int rDirn1 = 12;
int rDirn2 = 13;

ros::NodeHandle nh;

void straight()
{
    digitalWrite(lDirn1, HIGH);
    digitalWrite(lDirn2, LOW);
    digitalWrite(rDirn1, HIGH);
    digitalWrite(rDirn2, LOW);
    analogWrite(lSpeed, 150);
    analogWrite(rSpeed, 150);
}

void turn_left_updated()
{
    digitalWrite(lDirn1, HIGH);
    digitalWrite(lDirn2, LOW);
    digitalWrite(rDirn1, HIGH);
    digitalWrite(rDirn2, LOW);
    analogWrite(lSpeed, 50);
    analogWrite(rSpeed, 125);
}

void turn_right_updated()
{
    digitalWrite(lDirn1, HIGH);
    digitalWrite(lDirn2, LOW);
    digitalWrite(rDirn1, HIGH);
    digitalWrite(rDirn2, LOW);
    analogWrite(lSpeed, 125);
    analogWrite(rSpeed, 50);
}

void message_callback(const std_msgs::Float64 &theta)
{
    angle = theta.data;
    nh.loginfo("Angle: ");
    // nh.loginfo(angle);

    if (angle < -8)
    {
        turn_right_updated();
        nh.loginfo("turning right...");
    }
    else if (angle > 8)
    {
        nh.loginfo("turning left...");
        turn_left_updated();
    }
    else
    {
        straight();
    }
}

ros::Subscriber<std_msgs::Float64> sub("/robocon2018/error", &message_callback);

void setup()
{
    pinMode(lDirn1, OUTPUT);
    pinMode(lDirn2, OUTPUT);
    pinMode(lSpeed, OUTPUT);
    pinMode(rDirn1, OUTPUT);
    pinMode(rDirn2, OUTPUT);
    pinMode(rSpeed, OUTPUT);

    nh.initNode();
    nh.subscribe(sub);
}

void loop()
{
    nh.spinOnce();
}