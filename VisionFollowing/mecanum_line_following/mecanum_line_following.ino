char ch;
int distance;
float angle;
int iter = 1;

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

void setup() {
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

void loop() {
  static char buffer[32];
  static size_t pos;
  
  if(Serial.available())
  {
    iter++;
    char c = Serial.read();
    int rpm;
    if(c == '\n' || c == '\t') 
    {
      buffer[pos] = '\0';
      float val = atof(buffer);
      
      //Serial.print("Iteration: ");
      //Serial.println(iter);

      pos = 0;
      //int rpm=int(angle*255.0/90.0)%255;
      //int rpm=int(angle/3)%255;
      
      if(c == '\t')
      {
        int dellim=10;
        rpm=100;
        if(val < -dellim)
        {
          turn_right(rpm);
          Serial.println("turning right...");
        }
      
        else if(val > dellim)
        {
          Serial.println("turning left...");
          turn_left(rpm);
        }
        else
        {
          straight();
        }
      }
      
      else if(c == '\n')
      {
        int thetlim=10;
        rpm=100;
        if(val < -thetlim)
        {
          sideway_right(rpm);
          Serial.println("sideway right...");
        }
      
        else if(val > thetlim)
        {
          Serial.println("sideway left...");
          sideway_left(rpm);
        }
        else
        {
          straight();
        }
      }
      
      
      Serial.print(val);
      Serial.print("    ");
      Serial.println(rpm);
    }
    
    else if(pos < sizeof buffer - 1) 
    {
      buffer[pos++] = c;
    }
  }
}
