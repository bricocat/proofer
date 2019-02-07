int temp = 99;
unsigned long INTERVAL_TEMP = 2000;
unsigned long INTERVAL_TEMP_LAST = 0;

int TEMPERATURE_MIN = 19;


// resistor value of voltage divider in ohm
float resistorfixed = 2210;
float rtd_factor = 7.5; //change in ohm per °C as an average
// From the data sheet the value of the resistance of the sensor @ 25 degrees is 1000 +/- 10 ohmsStart with calculating the measured resistance.
float R25 = 1000;

 
int read_Temp() {  

  float adc_temp = analogRead(1);
  
  float Rkty = (resistorfixed * adc_temp)/(1023 - adc_temp);
  //Serial.println(Rkty);

  //linear calculation, for this case mor accurat, we have small temperature range ***********/
  float temp_lin = 25 + ((Rkty - R25) / rtd_factor);

  //interpolation calculation ****************************************************************/
  //NOT USED, FOR THE MOMENT
  //we are also given alpha and beta
  float alpha = 7.9/1000 ;
  float beta = 1.676/10000 ; 

  //Now we need to calculate the temperature factor (KayTee)
  float KayTee = Rkty/R25 ;
  
  //We now have all the information to calculate the actual temperature (AcT)
  float AcT = 25 + ((sqrt((alpha*alpha)-(4*beta)+(4*beta*KayTee)) - alpha)/(2*beta)) ;
  
  /*******************************************************************************************/

  //Serial.println(temp_lin);

  return round(temp_lin);
}
 
int get_Temp(){
  
  if(millis() - INTERVAL_TEMP_LAST > INTERVAL_TEMP){

    temp = read_Temp();
    INTERVAL_TEMP_LAST = millis();
  }
  
  return temp;  
}
