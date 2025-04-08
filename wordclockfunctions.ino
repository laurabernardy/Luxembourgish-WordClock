
const String clockStringGerman =  "ETPASSAECHIVEIERELZENGZWANZEGHUNNFENNEFCNVIRSOPLWHALWERFENNEFODECHENGSGARZWOUDRAITUVEIERSECHSQAACHTSIEWENEELEFZWIELEFZENGNINGKENAUER";

/**
 * @brief control the four minute indicator LEDs
 *
 * @param minutes minutes to be displayed [0 ... 59]
 * @param color 24bit color value
 */
void drawMinuteIndicator(uint8_t minutes, uint32_t color){
  //separate LEDs for minutes in an additional row
  {
  switch (minutes%5)
    { 
      case 0:
        break;
          
      case 1:
        ledmatrix.setMinIndicator(0b1000, color);
        break;

      case 2:
        ledmatrix.setMinIndicator(0b1100, color);
        break;

      case 3:
        ledmatrix.setMinIndicator(0b1110, color);
        break;

      case 4:
        ledmatrix.setMinIndicator(0b1111, color);
        break;
    }
  }
}

/**
 * @brief Draw the given sentence to the word clock
 * 
 * @param message sentence to be displayed
 * @param color 24bit color value
 * @return int: 0 if successful, -1 if sentence not possible to display
 */
int showStringOnClock(String message, uint32_t color){
    int messageStart = 0;
    String word = "";
    int lastLetterClock = 0;
    int positionOfWord  = 0;
    int nextSpace = 0;
    int index = 0;

    // add space on the end of message for splitting
    message = message + " ";

    // empty the targetgrid
    ledmatrix.gridFlush();

    while(true){
      // extract next word from message
      word = split(message, ' ', index);
      index++;
      
      if(word.length() > 0){
        // find word in clock string
        positionOfWord = clockStringGerman.indexOf(word, lastLetterClock);
        
        if(positionOfWord >= 0){
          // word found on clock -> enable leds in targetgrid
          for(int i = 0; i < word.length(); i++){
            int x = (positionOfWord + i)%WIDTH;
            int y = (positionOfWord + i)/WIDTH;
            ledmatrix.gridAddPixel(x, y, color);
          }
          // remember end of the word on clock
          lastLetterClock = positionOfWord + word.length();
        }
        else{
          // word is not possible to show on clock
          logger.logString("word is not possible to show on clock: " + String(word));
          return -1;
        }
        //logger.logString(String(nextSpace) + " - " + String());
      }else{
        // end - no more word in message
        break;
      }
    }
    // return success
    return 0;
}

/**
 * @brief Converts the given time as sentence (String)
 * 
 * @param hours hours of the time value
 * @param minutes minutes of the time value
 * @return String time as sentence
 */
String timeToString(uint8_t hours,uint8_t minutes){
  Serial.println(hours);
  Serial.println(minutes);
   //ES IST
  String message = "ET ASS ";

  
  //show minutes
  if(minutes >= 5 && minutes < 10)
  {
    message += "FENNEF OP ";
  }
  else if(minutes >= 10 && minutes < 15)
  {
    message += "ZENG OP ";
  }
  else if(minutes >= 15 && minutes < 20)
  {
    message += "VEIEREL OP ";
  }
  else if(minutes >= 20 && minutes < 25)
  {
    message += "ZWANZEG OP "; 
  }
  else if(minutes >= 25 && minutes < 30)
  {
    message += "FENNEF VIR HALWER ";
  }
  else if(minutes >= 30 && minutes < 35)
  {
    message += "HALWER ";
  }
  else if(minutes >= 35 && minutes < 40)
  {
    message += "FENNEF OP HALWER ";
  }
  else if(minutes >= 40 && minutes < 45)
  {
    message += "ZWANZEG VIR ";
  }
  else if(minutes >= 45 && minutes < 50)
  {
    message += "VEIEREL VIR ";
  }
  else if(minutes >= 50 && minutes < 55)
  {
    message += "ZENG VIR ";
  }
  else if(minutes >= 55 && minutes < 60)
  {
    message += "FENNEF VIR ";
  }

  //convert hours to 12h format
  if(hours >= 12)
  {
      hours -= 12;
  }
  if(minutes >= 25)
  {
      hours++;
  }
  if(hours == 12)
  {
      hours = 0;
  }

    // show hours
  switch(hours)
  {
  case 0:
    message += "ZWIELEF ";
    break;
  case 1:
    message += "ENG ";
    //EIN(S)
    //if(minutes > 4){
    //  message += "S";
    //}
    //message += " ";
    break;
  case 2:
    message += "ZWOU ";
    break;
  case 3:
    message += "DRAI ";
    break;
  case 4:
    message += "VEIER ";
    break;
  case 5:
    message += "FENNEF ";
    break;
  case 6:
    message += "SECHS ";
    break;
  case 7:
    message += "SIEWEN ";
    break;
  case 8:
    message += "AACHT ";
    break;
  case 9:
    message += "NING ";
    break;
  case 10:
    message += "ZENG ";
    break;
  case 11:
    message += "EELEF ";
    break;
  }
  if(minutes < 5)
  {
    message += "AUER ";
  }

  Serial.println(message);
  logger.logString("time as String: " + String(message));

  return message;
}

