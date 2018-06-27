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

void straight(int rpmfr,int rpmfl,int rpmbr,int rpmbl)
{
    digitalWrite(flDirn1, HIGH);
    digitalWrite(flDirn2, LOW);
    digitalWrite(frDirn1, HIGH);
    digitalWrite(frDirn2, LOW);
    analogWrite(flSpeed, rpmfl);
    analogWrite(frSpeed, rpmfr);
    digitalWrite(blDirn1, HIGH);
    digitalWrite(blDirn2, LOW);
    digitalWrite(brDirn1, HIGH);
    digitalWrite(brDirn2, LOW);
    analogWrite(blSpeed, rpmbl);
    analogWrite(brSpeed, rpmbr);
}

void setup()
{
    // put your setup code here, to run once:

    Serial.begin(4800);
    
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
}

void loop()
{
   //straight(38,65,50,81) ;
   
}
