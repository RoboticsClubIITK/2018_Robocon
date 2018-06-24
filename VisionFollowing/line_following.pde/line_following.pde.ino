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

void turn_left()
{
  digitalWrite(lDirn1, LOW);
  digitalWrite(lDirn2, HIGH);
  digitalWrite(rDirn1, HIGH);
  digitalWrite(rDirn2, LOW);
  analogWrite(lSpeed, 50);
  analogWrite(rSpeed, 50);
}

void turn_right()
{
  digitalWrite(lDirn1, HIGH);
  digitalWrite(lDirn2, LOW);
  digitalWrite(rDirn1, LOW);
  digitalWrite(rDirn2, HIGH);
  analogWrite(lSpeed, 50);
  analogWrite(rSpeed, 50);
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
      Serial.println(angle);
      Serial.print("Iteration: ");
      Serial.println(iter);
//      Serial.flush();
      pos = 0;

      if(angle < -8)
      {
        turn_right_updated();
        Serial.println("turning right...");
      }
      else if(angle > 8)
      {
        Serial.println("turning left...");
        turn_left_updated();
      }
      else
      {
        straight();
      }
    }
    else if(pos < sizeof buffer - 1) 
    {
      buffer[pos++] = c;
    }
  }
}
