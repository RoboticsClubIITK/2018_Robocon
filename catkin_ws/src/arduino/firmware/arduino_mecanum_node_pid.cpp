#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <math.h>

#define FR 0.38
#define FL 0.65
#define BR 0.50
#define BL 0.81

float Kp=1.0;
float Ki=0.0;
float Kd=1.0;

int maxRPM= 200;
int baseRPM=100;

#define flSpeed  2
#define flDirn1  23
#define flDirn2  22
#define frSpeed  3
#define frDirn1  25
#define frDirn2  24
#define blSpeed  4
#define blDirn1  27
#define blDirn2  26
#define brSpeed  5
#define brDirn1  29
#define brDirn2  28

float angle;
float distance;
int error=0;
int lastError=0;

ros::NodeHandle nh;

void limRPM(int rpm)
{
    if (rpm>maxRPM)
    {
        return maxRPM
    }
    return rpm;
}

void move(float rpmr,float rpml)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, limRPM((int)(FL*rpml)));
    analogWrite(frSpeed, limRPM((int)(FR*rpmr)));
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, limRPM((int)(BL*rpml)));
    analogWrite(brSpeed, limRPM((int)(BR*rpmr)));
}

void sideway_right(float rpmr,float rpml)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, HIGH);
    analogWrite(flSpeed, limRPM((int)(FL*rpml)));
    analogWrite(frSpeed, limRPM((int)(FR*rpmr)));
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, HIGH);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, limRPM((int)(BL*rpml)));
    analogWrite(brSpeed, limRPM(int)(BR*rpmr)));
}

void sideway_left(float rpmr,float rpml)
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, HIGH);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, limRPM((int)(FL*rpml)));
    analogWrite(frSpeed, limRPM((int)(FR*rpmr)));
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, HIGH);
    analogWrite(blSpeed, limRPM((int)(BL*rpml)));
    analogWrite(brSpeed, limRPM((int)(BR*rpmr)));
}

/*void distance_callback(const std_msgs::Float64 &dist)
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
}*/

//void angle_callback(const std_msgs::Float64 &theta)

    //float angle = theta.data;
    //nh.loginfo("Angle: %f",val);


void distance_callback(const std_msgs::Float64 &dist)
{
    float distance = dist.data;
    error=distance;

    float rpm = Kp * error + Kd * (error - lastError) + Ki*( error + lastError );
    lastError = error;

    float rpmr=baseRPM-rpm;
    float rpml=baseRPM+rpm;

    move(rpmr,rpml);
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
    //nh.subscribe(sub_angle);
    nh.subscribe(sub_distance);
    nh.subcribe(sub_rotcon);
}

void loop()
{
    nh.spinOnce();
}