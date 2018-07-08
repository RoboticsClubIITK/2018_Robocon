#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <math.h>

int flSpeed = 3;
int flDirn1 = 24;
int flDirn2 = 25;
int frSpeed = 2;
int frDirn1 = 23;
int frDirn2 = 22;
int blSpeed = 4;
int blDirn1 = 27;
int blDirn2 = 26;
int brSpeed = 5;
int brDirn1 = 29;
int brDirn2 = 28;

float vd=1;
float angle= 1.57;

int fl=205;
int bl=190;
int fr=66;//66//65
int br=220;//255//250
int flMotorSpeed,blMotorSpeed,frMotorSpeed,brMotorSpeed;

ros::NodeHandle nh;

void angle_callback(const std_msgs::Float64 &theta)
{
    float angle = theta.data;
    nh.loginfo("Angle: %f", val);

    flMotorSpeed = fl*vd*sin( angle + 0.785);
    blMotorSpeed = bl*vd*cos( angle + 0.785);
    frMotorSpeed = fr*vd*cos( angle + 0.785);
    brMotorSpeed = br*vd*sin( angle + 0.785);

    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    analogWrite(flSpeed, flMotorSpeed);

    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    analogWrite(blSpeed, blMotorSpeed);   
   
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(frSpeed, frMotorSpeed);
    
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(brSpeed, brMotorSpeed);
}

ros::Subscriber<std_msgs::Float64> sub_angle("/robocon2018/angle", &angle_callback);

void setup()
{  
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
}

void loop() 
{ 
    nh.spinOnce();
}