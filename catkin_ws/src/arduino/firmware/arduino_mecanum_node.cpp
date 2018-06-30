#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <math.h>

#define FR 38
#define FL 65
#define BR 50
#define BL 81



int flSpeed = 2;
int flDirn1 = 23;
int flDirn2 = 22;
int frSpeed = 3;
int frDirn1 = 25;
int frDirn2 = 24;
int blSpeed = 4;
int blDirn1 = 27;
int blDirn2 = 26;
int brSpeed = 5;
int brDirn1 = 29;
int brDirn2 = 28;

float angle;
float distance;

ros::NodeHandle nh;
void straight(float rpmr,float rpml)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, (int)(FL*rpml));
    analogWrite(frSpeed, (int)(FR*rpmr));
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, (int)(BL*rpml));
    analogWrite(brSpeed, (int)(BR*rpmr));
}

void backward(float rpmr,float rpml)
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, HIGH);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, HIGH);
    analogWrite(flSpeed, (int)(FL*rpml));
    analogWrite(frSpeed, (int)(FR*rpmr));
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, HIGH);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, HIGH);
    analogWrite(blSpeed, (int)(BL*rpml));
    analogWrite(brSpeed, (int)(BR*rpmr));
}

void turn_left(float rpmr,float rpml)
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, HIGH);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, (int)(FL*rpml));
    analogWrite(frSpeed, (int)(FR*rpmr));
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, HIGH);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, (int)(BL*rpml));
    analogWrite(brSpeed, (int)(BR*rpmr));
}

void turn_right(float rpmr,float rpml)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, HIGH);
    analogWrite(flSpeed, (int)(FL*rpml));
    analogWrite(frSpeed, (int)(FR*rpmr));
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, HIGH);
    analogWrite(blSpeed, (int)(BL*rpml));
    analogWrite(brSpeed, (int)(BR*rpmr));
}

void sideway_right(float rpmr,float rpml)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, HIGH);
    analogWrite(flSpeed, (int)(FL*rpml));
    analogWrite(frSpeed, (int)(FR*rpmr));
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, HIGH);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, (int)(BL*rpml));
    analogWrite(brSpeed, (int)(BR*rpmr));
}

void sideway_left(float rpmr,float rpml)
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, HIGH);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, (int)(FL*rpml));
    analogWrite(frSpeed, (int)(FR*rpmr));
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, HIGH);
    analogWrite(blSpeed, (int)(BL*rpml));
    analogWrite(brSpeed, (int)(BR*rpmr));
}

void distance_callback(const std_msgs::Float64 &dist)
{
    float distance = dist.data;
    //nh.loginfo("Dist: %f",val);

    int distlim = 10;
    float rpmr=2,rpml=2;
    //if(distance>50)
    //{
    if (distance < -distlim)
    {
        sideway_right(rpmr,rpml);
        nh.loginfo("sideway right...");
    }

    else if (distance > distlim)
    {
        sideway_left(rpmr,rpml);
        nh.loginfo("sideway left...");
    }
    else
    {
        straight(rpmr,rpml);
    }
    //}
}

void angle_callback(const std_msgs::Float64 &theta)
{
    float angle = theta.data;
    //nh.loginfo("Angle: %f",val);

    int thetlim = 10;
    float rpmr=0,rpml=0;

    //if(distance<=50)
    //{
    if (angle < -thetlim)
    {
        rpmr=0;
        rpml=1.5;
        turn_right(rpmr,rpml);
        nh.loginfo("turning right...");
    }

    else if (angle > thetlim)
    {
        rpmr=1.5;
        rpml=0;
        turn_left(rpmr,rpml);
        nh.loginfo("turning left...");
    }
    else
    {
        rpmr=1.5;
        rpml=1.5;
        straight(rpmr,rpml);
    }
    //}
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
    //nh.subscribe(sub_distance);
}

void loop()
{
    nh.spinOnce();
}