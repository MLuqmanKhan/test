// Starting of Program

int m1a = 9;

int m1b = 10;

int m2a = 11;

int m2b = 12;

char val;



void setup()
    {  
      pinMode(m1a, OUTPUT);  // Digital pin 10 set as output Pin
      pinMode(m1b, OUTPUT);  // Digital pin 11 set as output Pin
      pinMode(m2a, OUTPUT);  // Digital pin 12 set as output Pin
      pinMode(m2b, OUTPUT);  // Digital pin 13 set as output Pin
      Serial.begin(9600);
    }

void loop()
    {
      while (Serial.available() > 0){
      val = Serial.read();
      Serial.println(val);
    }

  
        
if( val == 'A') // Forward
    {
      forward();
      delay (1000);
      val='0';  
    }
else if( val == 'B') // Forward
   {
    forward();
   delay (2000);
  val='0';  
  }
else if( val == 'C') // Forward
   {
    forward();
   delay (3000);
  val='0';  
  }
  else if( val == 'D') // Forward
   {
    forward();
   delay (4000);
  val='0';  
  }
  
  else if(val == 'E') //Right
    {
      Right();
     delay (250);
     forward();
     delay (1000);
    val='0';
    }
    else if(val == 'F') //Right
    {
      Right();
     delay (250);
     forward();
     delay (2000);
    val='0';
    }
    else if(val == 'G') //Right
    {
      Right();
     delay (250);
     forward();
     delay (3000);
    val='0';
    }
    else if(val == 'H') //Right
    {
      Right();
     delay (250);
     forward();
     delay (4000);
    val='0';
    }
   
   
else if(val == 'I') //Left
    {
      Left();
      delay (250);
      forward();
     delay (1000);
      val='0';
    }
   
else if(val == 'J') //Left
    {
      Left();
      delay (250);
      forward();
     delay (2000);
      val='0';
    }
   
else if(val == 'K') //Left
    {
      Left();
      delay (250);
      forward();
     delay (3000);
      val='0';
    }
   
else if(val == 'L') //Left
    {
      Left();
      delay (250);
      forward();
     delay (4000);
      val='0';
    }

else if(val == 'M') // Backward
    {
      Backward();
   delay (1000);
  val='0';
    }
else if(val == 'N') // Backward
    {
      Backward();
   delay (2000);
  val='0';
    }
    else if(val == 'O') // Backward
    {
      Backward();
   delay (3000);
  val='0';
    }
    else if(val == 'P') // Backward
    {
      Backward();
   delay (4000);
  val='0';
    }
else if(val == 'S') //Stop
    {
      Stop();
      delay (1000);
      val='0';
    }
else
{
  End();
  }
}



//Functions


void forward(){
      digitalWrite(m1a, HIGH);
      digitalWrite(m1b, LOW);
      digitalWrite(m2a, HIGH);
      digitalWrite(m2b, LOW);
  }
void Backward(){
      digitalWrite(m1a, LOW);
      digitalWrite(m1b, HIGH);
      digitalWrite(m2a, LOW);
      digitalWrite(m2b, HIGH);
  }
void Right(){
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, HIGH);
    digitalWrite(m2b, LOW);
  }  
 
void Left(){
    digitalWrite(m1a, HIGH);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW);
  }
void Stop(){
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW);
  }  
void End(){
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW);
  }
