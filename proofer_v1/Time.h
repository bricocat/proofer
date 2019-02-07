int elapsed_Minutes = 0;
bool isMinute = true;
unsigned long INTERVAL_HOUR = 60000;
unsigned long INTERVAL_HOUR_LAST = 0;

int get_Elapsed_Time(){
  
  if(millis() - INTERVAL_HOUR_LAST > INTERVAL_HOUR){

    INTERVAL_HOUR_LAST = millis();
    elapsed_Minutes += 1;

    if(60 <= elapsed_Minutes){
      isMinute = false;  
    }
  }
  
  if(isMinute){
    return elapsed_Minutes;
  }
  else{
    return elapsed_Minutes / 60;
  }
}

bool get_IsMinute(){

  return isMinute;
}
