 /*Secuenciador Con Arduino 26/05/2022
  * 
  * 
  * Autor: Negro Gonzalo 
  */
 #define PINSTART 2
 #define ENDPIN 10 
 #define BUTTON 12
 #define PINPOT A1
 #define TIMETOCHK 50
 #define MINTIME 150
 #define MAXTIME 1500
 #define TIMEHEARTBEAT 200 
 #define LED 13
 typedef enum{ //enumeracion para los modos de la MEF 
        FOWARD,
        BACKWARD,
        CENTER,
        SIDE,
        FULL
    }_eMode;
_eMode Mode;    
typedef enum{//enumeracion para la detección de flancos 
    BUTTON_DOWN,    //0
    BUTTON_UP,      //1
    BUTTON_FALLING, //2
    BUTTON_RISING   //3
}_eButtonState;

_eButtonState myButton;
typedef struct{
    uint8_t estado;
    int32_t timeDown;
    int32_t timeDiff;
}_sTeclas;
_sTeclas ourButton;
 unsigned long lastTime,hbTime,secuenceTime; 
 unsigned int setTime; 
 uint8_t actualLed;
//prototipos de funciones 
/*
 * Funcion startMef, no params, inicializa la maquina de estados para el pulsador 
 */
void startMef();

void actuallizaMef();
void toggleLed(int led);
void resetLeds();
void getTime();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for(int i=PINSTART;i<=ENDPIN;i++){ //defino los 8 pines como salidas digitales 
    pinMode(i,OUTPUT);
  }
  pinMode(LED,OUTPUT);
  pinMode(PINPOT,INPUT);//potenciometro como entrada
  pinMode(BUTTON,INPUT);//boton como entrada 
  actualLed=PINSTART+1;
  Serial.println("Nuevo programa iniciado");
}

void loop() {
  // put your main code here, to run repeatedly:
    setTime=500;
    if(millis()-hbTime>TIMEHEARTBEAT){
      toggleLed(LED);
      hbTime=millis();
      }
    if(millis()-lastTime > TIMETOCHK){
       actuallizaMef(); 
       lastTime=millis();  
       
      }    
    switch(Mode){
      case FOWARD:
        if(millis()-secuenceTime > setTime){
           if(actualLed < ENDPIN){  
              digitalWrite(actualLed,HIGH);
              actualLed++;
              digitalWrite(actualLed,LOW);
             }else{
              digitalWrite(actualLed,HIGH);
              actualLed=PINSTART;
              digitalWrite(actualLed,LOW);   
              }
          secuenceTime=millis();
          }
        //getTime();//llamada a funcion que lee el potenciometro      
      break;
      case BACKWARD:
          if(millis()-secuenceTime > setTime){
            if(actualLed > PINSTART){  
              digitalWrite(actualLed,HIGH);
              actualLed--;
              digitalWrite(actualLed,LOW);
             }else{
              digitalWrite(actualLed,HIGH);
              actualLed=ENDPIN;
              digitalWrite(actualLed,LOW);   
              }
          secuenceTime=millis();
          }
        //getTime();//llamada a funcion que lee el potenciometro  
      break;
      case CENTER:
         if(millis()-secuenceTime > setTime){
          
          secuenceTime=millis();
          }
        //getTime();//llamada a funcion que lee el potenciometro  
      break;
      case SIDE:
          if(millis()-secuenceTime > setTime){
          
          secuenceTime=millis();
          }
        //getTime();//llamada a funcion que lee el potenciometro  
      break;
      case FULL:
          if(millis()-secuenceTime > setTime){
                for(int j=PINSTART;j<=ENDPIN;j++){
                    toggleLed(j);
                  }
          secuenceTime=millis();
          }
      break;
      }
}
void startMef(){
   ourButton.estado=BUTTON_UP;
}
void actuallizaMef(){
   switch (ourButton.estado)
    {
    case BUTTON_DOWN:
        if(!digitalRead(BUTTON))
           ourButton.estado=BUTTON_RISING;
    
    break;
    case BUTTON_UP:
        if(digitalRead(BUTTON))
            ourButton.estado=BUTTON_FALLING;
    
    break;
    case BUTTON_FALLING:
        if(digitalRead(BUTTON))
        {
            ourButton.timeDown=millis();
            ourButton.estado=BUTTON_DOWN;
            //Flanco de bajada
        }
        else
            ourButton.estado=BUTTON_UP;    

    break;
    case BUTTON_RISING:
        if(!digitalRead(BUTTON)){

            ourButton.estado=BUTTON_UP;
            //Flanco de Subida
            ourButton.timeDiff=millis()-ourButton.timeDown;
            if(ourButton.timeDiff >= 200){//si el boton estuvo presionado entre 100 y 1000ms (cambio los modos)
                
                if(Mode==BACKWARD){
                  Mode=FOWARD;    
                  resetLeds();
                  Serial.println("change Mode FOWARD");   
                   }else{
                    if(Mode==FOWARD){
                      Mode=FULL;
                        resetLeds();
                        Serial.println("change Mode FULL"); 
                      }else{
                        Mode=BACKWARD;
                        resetLeds();
                        Serial.println("change Mode BACKWARD"); 
                        } 
                    }
              ourButton.timeDiff=0;
            }
        }else{
            ourButton.estado=BUTTON_DOWN;
         }
    break;
    
    default:
        startMef();
        break;
    
    }
  
}
void resetLeds(){
  for(int k=PINSTART;k<=ENDPIN;k++){
      digitalWrite(k,HIGH);
    }
  }
void toggleLed(int led){
  if(digitalRead(led)==HIGH){
    digitalWrite(led,LOW);
    }else{
      digitalWrite(led,HIGH);
      }
  }
void getTime(){//funcion que lee el potenciometro y obtiene el tiempo de demora. 
  
  }
