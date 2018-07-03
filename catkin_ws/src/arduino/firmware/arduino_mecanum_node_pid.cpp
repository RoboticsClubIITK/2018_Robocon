/*
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

float vd=0.75;

int fl=205;
int bl=190;
int fr=66;//66//65
int br=220;//255//250
int flMotorSpeed,blMotorSpeed,frMotorSpeed,brMotorSpeed;

ros::NodeHandle nh;

void angle_callback(const std_msgs::Float64 &theta)
{
    float angle = theta.data;
    //nh.loginfo("Angle: %f", val);

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
*/













#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <math.h>


#define FR 0.56
#define FL 2.05
#define BR 1.80
#define BL 1.90
/*
Works oascillating in this
float Kp=0.05;
float Ki=0.0;
float Kd=0.0;
int iter=0;

int maxRPM= 200;
int baseRPM=50;
*/


float Kp=0.07;
float Ki=0.01;
float Kd=0.0;
int iter=0;

int maxRPM= 200;
int baseRPM=70;

#define flSpeed  3
#define flDirn1  24
#define flDirn2  25
#define frSpeed  2
#define frDirn1  23
#define frDirn2  22
#define blSpeed  4
#define blDirn1  27
#define blDirn2  26
#define brSpeed  5
#define brDirn1  29
#define brDirn2  28

float angle;
float distance;
float rotCon=0;
int error=0;
int lastError=0;

ros::NodeHandle nh;

int limRPM(int rpm)
{
    if (rpm>maxRPM)
    {
        return maxRPM;
    }
    else if (rpm<0)
        return 0;
    return rpm;

}




void reset()
{
    digitalWrite(flDirn1, LOW);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, 0);
    analogWrite(frSpeed, 0);
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, LOW);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, 0);
    analogWrite(brSpeed, 0);
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

    char res1[8];
    dtostrf(limRPM((int)(FL*rpml)), 6, 2, res1);
    //nh.loginfo("FL");
    //nh.loginfo(res1);
    char res2[8];
    dtostrf(limRPM((int)(FR*rpmr)), 6, 2, res2);
    //nh.loginfo("FR");
    //nh.loginfo(res2);
    char res3[8];
    dtostrf(limRPM((int)(BL*rpml)), 6, 2, res3);
    //nh.loginfo("BL");
    //nh.loginfo(res3);
    char res4[8];
    dtostrf(limRPM((int)(BR*rpmr)), 6, 2, res4);
    //nh.loginfo("BR");
    //nh.loginfo(res4);
    //nh.loginfo("\n");
    //nh.loginfo("\n");


}

void move_sideway(float rpmfr,float rpmbr)
{
    //fr&bl motion: moves right
    //br&fl motion: moves left


    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, LOW);
    digitalWrite(frDirn2, HIGH);
    analogWrite(flSpeed, limRPM((int)(FL*rpmbr)));
    analogWrite(frSpeed, limRPM((int)(FR*rpmfr)));
    digitalWrite(blDirn1, LOW);
    digitalWrite(blDirn2, HIGH);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, limRPM((int)(BL*rpmfr)));
    analogWrite(brSpeed, limRPM((int)(BR*rpmbr)));

    char res1[8];
    dtostrf(limRPM((int)(FL*rpmbr)), 6, 2, res1);
    //nh.loginfo("FL");
    //nh.loginfo(res1);
    char res2[8];
    dtostrf(limRPM((int)(FR*rpmfr)), 6, 2, res2);
    //nh.loginfo("FR");
    //nh.loginfo(res2);
    char res3[8];
    dtostrf(limRPM((int)(BL*rpmfr)), 6, 2, res3);
    //nh.loginfo("BL");
    //nh.loginfo(res3);
    char res4[8];
    dtostrf(limRPM((int)(BR*rpmbr)), 6, 2, res4);
    //nh.loginfo("BR");
    //nh.loginfo(res4);
    //nh.loginfo("\n");
    //nh.loginfo("\n");


}


/*
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
    analogWrite(brSpeed, limRPM((int)(BR*rpmr)));
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
*/

/*void distance_callback(const std_msgs::Float64 &dist)
{
    distance = dist.data;
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

    //angle = theta.data;
    //nh.loginfo("Angle: %f",val);


void distance_callback(const std_msgs::Float64 &dist)
{
    //fr&bl motion: moves right
    //br&fl motion: moves left
    
    
    distance = dist.data;

    char res[8];
    dtostrf(distance, 6, 2, res);
    //nh.loginfo("Distance");
    //nh.loginfo(res);
    
    error=distance;

    float rpm = Kp * error + Kd * (error - lastError) + Ki*( error + lastError );
    lastError = error;


    float rpmr=baseRPM-rpm;
    float rpml=baseRPM+rpm;
    iter=iter+1;
    move(rpmr,rpml);
    char iterst[8];
    dtostrf(iter,6,2,iterst);
    //nh.loginfo(iterst);
}

void distance_rot_callback(const std_msgs::Float64 &dist)
{
    distance = dist.data;

    char res[8];
    dtostrf(distance, 6, 2, res);
    //nh.loginfo("Distance");
    //nh.loginfo(res);

    error=distance;

    float rpm = Kp * error + Kd * (error - lastError) + Ki*( error + lastError );
    lastError = error;


    float rpmr=baseRPM-rpm;
    float rpml=baseRPM+rpm;
    iter=iter+1;
    move_sideway(rpmr,rpml);
    char iterst[8];
    dtostrf(iter,6,2,iterst);
    //nh.loginfo(iterst);
}

void rotation_callback(const std_msgs::Int32 &rot)
{
    rotCon = rot.data;
}

//ros::Subscriber<std_msgs::Float64> sub_angle("/robocon2018/angle", &angle_callback);
ros::Subscriber<std_msgs::Float64> sub_distance("/robocon2018/distance", &distance_callback);
ros::Subscriber<std_msgs::Float64> sub_distance_rot("/robocon2018/distance", &distance_rot_callback);
ros::Subscriber<std_msgs::Int32> sub_rotcon("/robocon2018/rotation", &rotation_callback);

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
    
    char rott[8];
    nh.subscribe(sub_rotcon);
    dtostrf(rotCon,6,2,rott);
    nh.loginfo(rott);
    if(rotCon==0)
        nh.subscribe(sub_distance);
    else if (rotCon==1)
        nh.subscribe(sub_distance_rot);
}

void loop()
{
    nh.spinOnce();
}