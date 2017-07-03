#define WATER_BOMB_PIN 8  
#define WATCH_DOG 13
//pin que controla la bomba de agua
#define SEC_TO_MILISEC(x) ((x)*1000) 
#define STOP_BOMB 1
#define RUN_BOMB 2


class waterBomb
{
  public:
  waterBomb(unsigned int pin_);
  on(){digitalWrite(pin,HIGH);};
  off(){digitalWrite(pin,LOW);};
  private:
  unsigned int pin;
  
};

waterBomb::waterBomb(unsigned int pin_)
{
  pin=pin_;
  pinMode(pin,OUTPUT);
  off();
}

class myTimer
{
  public:
  myTimer(unsigned int seconds=0);
  bool timeOver(void);
  void setNewTime(unsigned long seconds_);
  void showInfo();
  
  unsigned long seconds;
  unsigned long startTime;
  void resetTimer(void);
    
};
myTimer timer(1);
waterBomb bomb(WATER_BOMB_PIN);
String readData;
void setup() {
  Serial.begin(9600);
  pinMode(WATCH_DOG,OUTPUT);
  

}

String serialData(void)
{
  String data;
  if(!Serial.available())
  {
    String _empty;
    return _empty;
  }
   while(Serial.available())
  {
    delay(5);
    if(Serial.available()>0)
    {
      char c= Serial.read();
      data+=c; 
    }
  }
  return data;
}
int mode=STOP_BOMB;
myTimer blinkTimer(1);

//********************************START LOOP*****************************************
void loop() 
{
  int dataSended;
  //****************ETAPA DE RECEPCION DE INFORMACION DEL PUERTO SERIE
  readData=serialData();
  if(readData.length()!=0)
  {
    
    dataSended=readData.toFloat();
  }
  else
  {
    dataSended=-1;
  }
//**************** FIN ETAPA DE RECEPCION DE INFORMACION DEL PUERTO SERIE
  
//******************ETAPA DE ANALIZIS DE DATO RECIVIDO  
  if(dataSended==200)
  {
    mode=STOP_BOMB;
//    Serial.println("Se recivio comando para apagar el sistema");
  }
  else if(dataSended>0)
  {
    mode=RUN_BOMB;
    timer.setNewTime(dataSended*60);
//    Serial.print("Se registro un Tiempo de: ");
//    Serial.println(dataSended*60);
    timer.showInfo();
    
  }
//******************FIN ETAPA DE ANALIZIS DE DATO RECIVIDO
  
//****************ETAPA FSM**************  
  switch (mode)
  {
    case STOP_BOMB:
    
    bomb.off();
    break;
    case RUN_BOMB:
    if(!timer.timeOver())
    {
      
      bomb.on();
    }
    else
    {
     // Serial.println("TimeOut");
      mode =STOP_BOMB;
    }
    break;
  }
//**********************FIN FSM

//********WACH DOG SECTION********//
if(blinkTimer.timeOver())         //
{                                 //
 // timer.showInfo();               //
  if(digitalRead(WATCH_DOG))      //
  {                               //
    digitalWrite(WATCH_DOG,LOW);  //
  }                               //
  else                            //
  {                               //
    digitalWrite(WATCH_DOG,HIGH); //
  }                               //
//********************************//  
}

  

}

//****************************************************************************END LOOP

myTimer::myTimer(unsigned int seconds)
{
  setNewTime(seconds);
}

//timeOver
//devuelve true si ya paso el tiempo seteado,
//caso contrario devuelve false
//
bool myTimer::timeOver(void)
{
  if((millis())>startTime)
  {
    resetTimer();
    return true;
  }
  else
  {
    return false;
  }
}

void myTimer::resetTimer(void)
{
  unsigned long temp=seconds+millis();
 
  startTime=temp;
  //Serial.print("se llamo a rest timer con: ");
  //Serial.println(startTime);
}

void  myTimer::setNewTime(unsigned long seconds_)
{
  unsigned long temp=1000*seconds_;
  //Serial.println(temp);
  seconds=temp;
 
  //Serial.print("s seteo un timer cada: ");
  //Serial.print(seconds_);
  //Serial.print(" se registro un tirmpo de: ");
  //Serial.println(seconds/1000);
  resetTimer();

}

void myTimer::showInfo()
{
  //Serial.println(startTime);
  unsigned long dif=startTime-millis();
  //Serial.print("Remaining time (seconds):");
  //Serial.println(dif/1000);
  //Serial.println(startTime);
  //Serial.println(millis());
  //Serial.println(seconds/1000);
}

