int target = 0;

int perform_Control(){

  target = analogRead(0);

  
  //Serial.println(target / 30);

  return target / 30;
  
}
