#include "Temperature.h"
#include "Time.h"
#include "Control.h"

//PIN setup
int digiT_1 = 7;  //first digit temperature
int digiT_0 = 6;  //second digit temperature
int digiH_1 = 5;  //first digit hour
int digiH_0 = 4;  //secont digit hour

int A = 18; //bcd
int B = 19; //bcd
int C = 2; //bcd
int D = 3; //bcd

int relais_Pin = 16; //relais pin to switch the lamps

int blink_Temp = 1;
bool blink_Temp_ON = false;

int blink_Hour = 1;

int blink_Count_Temp = 0;
int blink_Count_Hour = 0; 

int interval = 3; //display ON time in ms

int current_Temp, target_Temp;
byte temp_bcd, hours_bcd;

// the setup routine runs once when you press reset:
void setup() {  
                
  // initialize the digital pin as an output.
  pinMode(digiT_1, OUTPUT);  
  pinMode(digiT_0, OUTPUT);  
  pinMode(digiH_1, OUTPUT);  
  pinMode(digiH_0, OUTPUT);  
  pinMode(A, OUTPUT);  
  pinMode(B, OUTPUT);  
  pinMode(C, OUTPUT);  
  pinMode(D, OUTPUT);

  //get current temperature
  current_Temp = get_Temp();
       
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {

  
  //get current temperature
  current_Temp = get_Temp();
 
  target_Temp = perform_Control();

  if(0 == target_Temp){
    
    //display actual temperature
    temp_bcd = decToBcd(current_Temp);
    //stop blink
    blink_Temp_ON = false;
    //switch OFF heating
    digitalWrite(relais_Pin, LOW);
  }
  else{
    //set real target temperature
    target_Temp += TEMPERATURE_MIN;

    Serial.println(target_Temp);

    temp_bcd = decToBcd(target_Temp);

    if(current_Temp < target_Temp){
      //blink_Temp_ON temp higher than current >> blink and switch ON heating
      blink_Temp_ON = true;
      digitalWrite(relais_Pin, HIGH);
      
    }
    else if(current_Temp > target_Temp){
      //reached target temp >> switch OFF heating, stop blink
      blink_Temp_ON = false;
      digitalWrite(relais_Pin, LOW);
      
    }
  }

  
  hours_bcd = decToBcd(get_Elapsed_Time());
  
  //switch off
  digitalWrite(digiH_0, LOW);
  //write bcd for temperature digit 1
  digitalWrite(D,bitRead(temp_bcd, 7) || blink_Temp);
  digitalWrite(C,bitRead(temp_bcd, 6) || blink_Temp);
  digitalWrite(B,bitRead(temp_bcd, 5) || blink_Temp);
  digitalWrite(A,bitRead(temp_bcd, 4) || blink_Temp);
  //switch on
  digitalWrite(digiT_1, HIGH);   
  delay(interval);
  //switch off               
  digitalWrite(digiT_1, LOW);
  
  //write bcd for temperature digit 0
  digitalWrite(D,bitRead(temp_bcd, 3) || blink_Temp);
  digitalWrite(C,bitRead(temp_bcd, 2) || blink_Temp);
  digitalWrite(B,bitRead(temp_bcd, 1) || blink_Temp);
  digitalWrite(A,bitRead(temp_bcd, 0) || blink_Temp);
  //switch on
  digitalWrite(digiT_0, HIGH);   
  delay(interval);     
  //swotch off
  digitalWrite(digiT_0, LOW);
  
  //write bcd for hour digit 1
  digitalWrite(D,bitRead(hours_bcd, 7) || blink_Hour);
  digitalWrite(C,bitRead(hours_bcd, 6) || blink_Hour);
  digitalWrite(B,bitRead(hours_bcd, 5) || blink_Hour);
  digitalWrite(A,bitRead(hours_bcd, 4) || blink_Hour);
  //switch on, only if hours greater 10 (leading zero not displayed)
  if(10 <= hours_bcd){
    digitalWrite(digiH_1, HIGH);   
    delay(interval);      
    //switch off         
    digitalWrite(digiH_1, LOW);
  }

  //write bcd for hour digit 0
  digitalWrite(D,bitRead(hours_bcd, 3) || blink_Hour);
  digitalWrite(C,bitRead(hours_bcd, 2) || blink_Hour);
  digitalWrite(B,bitRead(hours_bcd, 1) || blink_Hour);
  digitalWrite(A,bitRead(hours_bcd, 0) || blink_Hour);
  //switch on
  digitalWrite(digiH_0, HIGH);   
  delay(interval);     
  
  //blink for temperature
  if(50 > blink_Count_Temp){
    blink_Count_Temp += 1;
  }
  else{
    blink_Count_Temp = 0;
    if(blink_Temp_ON){
     blink_Temp = !blink_Temp;
    }
    else{
      blink_Temp = 0;
    }
  }

  //blink for hour
  if(200 > blink_Count_Hour){
    blink_Count_Hour += 1;
  }
  else{
    blink_Count_Hour = 0;
    //check if timer delivers hours; if not blink is ON
    if(get_IsMinute()){
     blink_Hour = !blink_Hour;
    }
    else{
      blink_Hour = 0;
    }
  }
}

byte decToBcd(byte val)
{
  return (((val/10)*16)  + (val%10) );
}


