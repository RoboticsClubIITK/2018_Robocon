#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <math.h>

int flSpeed = 3;
int flDirn1 = 4;
int flDirn2 = 7;
int frSpeed = 11;
int frDirn1 = 12;
int frDirn2 = 13;
int blSpeed = 0;
int blDirn1 = 0;
int blDirn2 = 0;
int brSpeed = 0;
int brDirn1 = 0;
int brDirn2 = 0;

float angle;
float distance;

ros::NodeHandle nh;
void straight()
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, 100);
    analogWrite(frSpeed, 100);
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, 100);
    analogWrite(brSpeed, 100);
}

void turn_left(int rpm)
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, rpm);
    analogWrite(frSpeed, rpm);
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, rpm);
    analogWrite(brSpeed, rpm);
}

void turn_right(int rpm)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, rpm);
    analogWrite(frSpeed, rpm);
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, rpm);
    analogWrite(brSpeed, rpm);
}

void sideway_left(int rpm)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, HIGH);
    analogWrite(flSpeed, rpm);
    analogWrite(frSpeed, rpm);
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, HIGH);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, rpm);
    analogWrite(brSpeed, rpm);
}

void sideway_right(int rpm)
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, HIGH);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, rpm);
    analogWrite(frSpeed, rpm);
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, HIGH);
    analogWrite(blSpeed, rpm);
    analogWrite(brSpeed, rpm);
}

void angle_callback(const std_msgs::Float64 &theta)
{
    angle = theta.data;
    nh.loginfo("Angle: ");
    // nh.loginfo(angle);

    int thetlim = 10;
    int rpm = 100;
    if (angle < -thetlim)
    {
        sideway_right(rpm);
        nh.loginfo("sideway right...");
    }

    else if (angle > thetlim)
    {
        nh.loginfo("sideway left...");
        sideway_left(rpm);
    }
    else
    {
        straight();
    }
}

void distance_callback(const std_msgs::Float64 &dist)
{
    float val = dist.data;

    int dellim = 10;
    int rpm = 100;
    if (val < -dellim)
    {
        turn_right(rpm);
        nh.loginfo("turning right...");
    }

    else if (val > dellim)
    {
        nh.loginfo("turning left...");
        turn_left(rpm);
    }
    else
    {
        straight();
    }
}

ros::Subscriber<std_msgs::Float64> sub_angle("/robocon2018/angle", &angle_callback);
ros::Subscriber<std_msgs::Float64> sub_distance("/robocon2018/distance", &distance_callback);

void setup()
{
    // put your setup code here, to run once:

    pinMode(flDirn1, OUTPUT);
    pinMode(flDirn2, OUTPUT);
    pinMode(flSpeed, OUTPUT);
    pinMode(frDirn1, OUTPUT);
    pinMode(frDirn2, OUTPUT);
    pinMode(frSpeed, OUTPUT);
    pinMode(blDirn1, OUTPUT);
    pinMode(blDirn2, OUTPUT);
    pinMode(blSpeed, OUTPUT);
    pinMode(brDirn1, OUTPUT);
    pinMode(brDirn2, OUTPUT);
    pinMode(brSpeed, OUTPUT);

    nh.initNode();
    nh.subscribe(sub_angle);
    nh.subscribe(sub_distance);
}

void loop()
{
    nh.spinOnce();
}