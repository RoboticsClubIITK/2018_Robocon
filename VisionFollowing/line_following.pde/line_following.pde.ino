char ch;
int distance;
float angle;
int iter = 1;

int lSpeed = 3;
int lDirn1 = 4;
int lDirn2 = 7;
int rSpeed = 11;
int rDirn1 = 12;
int rDirn2 = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(4800);

  pinMode(lDirn1, OUTPUT);
  pinMode(lDirn2, OUTPUT);
  pinMode(lSpeed, OUTPUT);
  pinMode(rDirn1, OUTPUT);
  pinMode(rDirn2, OUTPUT);
  pinMode(rSpeed, OUTPUT);

//  turn_right_updated();
}

void straight()
{
  digitalWrite(lDirn1, HIGH);
  digitalWrite(lDirn2, LOW);
  digitalWrite(rDirn1, HIGH);
  digitalWrite(rDirn2, LOW);
  analogWrite(lSpeed, 100);
  analogWrite(rSpeed, 100);
}



void turn_left(int rpm)
{
  digitalWrite(lDirn1, LOW);
  digitalWrite(lDirn2, LOW);
  digitalWrite(rDirn1, HIGH);
  digitalWrite(rDirn2, LOW);
  analogWrite(lSpeed, rpm);
  analogWrite(rSpeed, rpm);
}

void turn_right(int rpm)
{
  digitalWrite(lDirn1, HIGH);
  digitalWrite(lDirn2, LOW);
  digitalWrite(rDirn1, LOW);
  digitalWrite(rDirn2, LOW);
  analogWrite(lSpeed, rpm);
  analogWrite(rSpeed, rpm);
}

void loop() {
  static char buffer[32];
  static size_t pos;
  
  if(Serial.available())
  {
    iter++;
    char c = Serial.read();
    
    if(c == '\n') 
    {
      buffer[pos] = '\0';
      float angle = atof(buffer);
      
      //Serial.print("Iteration: ");
      //Serial.println(iter);

      pos = 0;
      //int rpm=int(angle*255.0/90.0)%255;
      //int rpm=int(angle/3)%255;
      int rpm=100;
      if(angle < -10)
      {
        turn_right(rpm);
        Serial.println("turning right...");
      }
      else if(angle > 10)
      {
        Serial.println("turning left...");
        turn_left(rpm);
      }
      else
      {
        straight();
      }
      Serial.print(angle);
      Serial.print("    ");
      Serial.println(rpm);
    }
    else if(pos < sizeof buffer - 1) 
    {
      buffer[pos++] = c;
    }
  }
}
