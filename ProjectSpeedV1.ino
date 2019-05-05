#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //LCD-screen setup

const int button1 = 4;          //Setup buttons and button lights
const int button1Light = 2;
const int button2 = 7;
const int button2Light = 5;
const int button3 = 8;
const int button3Light = 6;
const int button4 = 12;
const int button4Light = 9;

int i = 0;                      //Variables and boolean needed for the speedgame
int score = 0;
int scoreCheck = 0;
int highScore = 0;
int number = 0;
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int valCheck = 0;           
int timer = 1000;
int timeValue = 0;
bool val = false;
bool falseVal1 = false;
bool falseVal2 = false;
bool falseVal3 = false;
bool falseVal4 = false;
int falseValCheck = 0;

int buttonState1 = 0;         //Variables for memoryGame
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonPress1 = 0;
int buttonPress2 = 0;
int buttonPress3 = 0;
int buttonPress4 = 0;
long randNum = 0;
int maxlvl = 20;
int lvl = 0;
bool memTerminate = false;
int a = 0;
int j = 0;

const int switchPin1 = 4; //Variables for reactionGame
int switchStart = 4;
int ledPinP1 = 2 ;
int ledPin = 2;
int TulosP1 = 0;
int NapinAsentoP1 = 0;
int lastNapinAsentoP1 = 0;
int randomPin;
int pins[] = {2};
boolean PeliOn = false;
boolean StartNappiState = LOW;
boolean P1Valmis = false;
boolean TervetuloViesti = false;
long randomTime;
long startTime;
long endTimeP1;
float LopullinenAikaP1;


bool mainMenu = true;           //Booleans needed for the Menu
bool menuOnce = false;
bool gameMenu = false;
bool game1 = true;
bool game2 = false;
bool game3 = false;

bool speedGame = false;         //Game choosing booleans
bool reactionGame = false;
bool memoryGame = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(button1, INPUT);            //Setup inputs and outputs for buttons and their lights.
  pinMode(button1Light, OUTPUT);
  pinMode(button2, INPUT);
  pinMode(button2Light, OUTPUT);
  pinMode(button3, INPUT);
  pinMode(button3Light, OUTPUT);
  pinMode(button4, INPUT);
  pinMode(button4Light, OUTPUT);

  pinMode(switchPin1, INPUT);
  pinMode(ledPinP1, OUTPUT);
  randomSeed(millis);

  lcd.begin(16,2);                    //Set how many characters and rows
  lcd.clear();                        
  delay(1000);
  lcd.print("Welcome!");              //A nice little welcome message when opening the game.
  delay(1000);
}
void Menu()                           //Menu itself
{
  val1 = digitalRead(button1);        //Setup variable readers for buttons in menu
  val2 = digitalRead(button2);
  val3 = digitalRead(button3);
  val4 = digitalRead(button4);

  while(!menuOnce)                    //Have menu appear only once instead of constantly coming over and over
  {
    lcd.clear();                      //Print the menu text and set the lights for buttons accordingly
    lcd.print("1. game");
    lcd.setCursor(0, 1);
    lcd.print("2. highscore 3.R");
    digitalWrite(button1Light, LOW);
    digitalWrite(button2Light, HIGH);
    digitalWrite(button3Light, HIGH);
    digitalWrite(button4Light, HIGH);
    menuOnce = true;
  }

  while(gameMenu == true)               //Keep the game menu on untill said otherwise.
  {
    GameMenu();
  }
  if(val4 == 1)                         //Check if you press 1st button and turn on the game menu if you do.
  {
    gameMenu = true;
    delay(500);
  }
  if(val3 == 1)
  {
    ReadHiscore();
    lcd.clear();
    lcd.print("Hiscore: ");
    lcd.print(highScore);
    delay(3000);
    menuOnce = false;
  }
  if(val2 == 1)
  {
    ResetHighScore();
    lcd.clear();
    lcd.print("resetting");
    delay(1000);
    menuOnce = false;
  }
}

void ResetHighScore()
{
  highScore = 0;
  WriteHiscore();
}
void GameMenu()
{
  val1 = digitalRead(button1);          //Setup variable readers for buttons in gameMenu
  val2 = digitalRead(button2);
  val3 = digitalRead(button3);
  val4 = digitalRead(button4);
  
  digitalWrite(button3Light, HIGH);     //Turn on the lights needed for the menu.
  digitalWrite(button1Light, HIGH);
  
  if(val4 == 1)                         //Check which button has bee pressed and turn on the menu needed for that one
  {
    game1 = true;
    game2 = false;
    game3 = false;
    delay(500);
  }
  if(val3 == 1)
  {
    game1 = false;
    game2 = true;
    game3 = false;
    delay(500);
  }
  if(val2 == 1)
  {
    game1 = false;
    game2 = false;
    game3 = true;
    delay(500);
  }

  if(val1 == 1)                         //4th button will start the game
  {
    gameMenu = false;                   //Reset the menu and start the game
    mainMenu = false;
    digitalWrite(button1Light, LOW);    // Reset the button lights
    digitalWrite(button2Light, LOW);
    digitalWrite(button3Light, LOW);
    digitalWrite(button4Light, LOW);
    delay(500);
  }
  if (game1 == true)                    //Print the text of needed for the game in LCD-screen and turn on the corresponding game boolean
  {
    lcd.clear();
    lcd.print("Speed Game");
    game1 = false;
    speedGame = true;
    reactionGame = false;
    memoryGame = false;
  }
  if (game2 == true)
  {
    lcd.clear();
    lcd.print("Reaction Game");
    game2 = false;
    speedGame = false;
    reactionGame = true;
    memoryGame = false;
  }
  if (game3 == true)
  {
    lcd.clear();
    lcd.print("Memory Game");
    game3 = false;
    speedGame = false;
    reactionGame = false;
    memoryGame = true;
  }
}

void ButtonsOn (int button)                   //For the game Turn on the button light
{
  if (button == 0)
  {
    digitalWrite(button1Light, HIGH);
  }
  if (button == 1)
  {
    digitalWrite(button2Light, HIGH);
  }
  if (button == 2)
  {
    digitalWrite(button3Light, HIGH);
  }
  if (button == 3)
  {
    digitalWrite(button4Light, HIGH);
  }
}

void ButtonsOff (int button)                  //For the game turn off the buttons light
{
  if (button == 0)
  {
    digitalWrite(button1Light, LOW);
  }
  if (button == 1)
  {
    digitalWrite(button2Light, LOW);
  }
  if (button == 2)
  {
    digitalWrite(button3Light, LOW);
  }
  if (button == 3)
  {
    digitalWrite(button4Light, LOW);
  }
}
void CheckButtonState(int button)           //Check the state of the button that's needed to be pressed
{

  if (button == 0)
  {
    val = digitalRead(button1);             //Sets up the variable reader for the corresponding button
    
    if (val == true)                        //checks if the button has been pressed, if it has been sets up valCheck to 1
    {
      valCheck = 1;
    }
  }
  if (button == 1)
  {
    val = digitalRead(button2);
    
    if (val == true)
    {
      valCheck = 1;
    }
  }
  if (button == 2)
  {
    val = digitalRead(button3);
    
    if (val == true)
    {
      valCheck = 1;
    }
  }
  if (button == 3)
  {
    val = digitalRead(button4);
    
    if (val == true)
    {
      valCheck = 1;
    }
  }
}
void CheckFalseButtonState(int button)           //Check the state of the button that's needed to be pressed
{

  if (button == 0)
  {
    falseVal1 = digitalRead(button2);            
    falseVal2 = digitalRead(button3);
    falseVal3 = digitalRead(button4);

    if (falseVal1 == true)                        
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    if(falseVal2 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    if(falseVal3 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    /*if (val2 == 1)                        
    {
      falseValCheck = true;
    }else if(val3 == 1) 
    {
      falseValCheck = true;
    }else if(val4 == 1)
    {
      falseValCheck = true;
    }*/
  }
  if (button == 1)
  {
    falseVal1 = digitalRead(button1);            
    falseVal2 = digitalRead(button3);
    falseVal3 = digitalRead(button4);

    if (falseVal1 == true)                        
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    if(falseVal2 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    
    if(falseVal3 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    /*if (val1 == 1)                        
    {
      falseValCheck = true;
    }else if(val3 == 1) 
    {
      falseValCheck = true;
    }else if(val4 == 1)
    {
      falseValCheck = true;
    }*/
  }
  if (button == 2)
  {
    falseVal1 = digitalRead(button1);            
    falseVal2 = digitalRead(button2);
    falseVal3 = digitalRead(button4);

    if (falseVal1 == true)                        
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    if(falseVal2 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    if(falseVal3 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    /*if (val1 == 1)                        
    {
      falseValCheck = true;
    }else if(val2 == 1) 
    {
      falseValCheck = true;
    }else if(val4 == 1)
    {
      falseValCheck = true;
    }*/
  }
  if (button == 3)
  {
    falseVal1 = digitalRead(button1);            
    falseVal2 = digitalRead(button2);
    falseVal3 = digitalRead(button3);

    if (falseVal1 == true)                        
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    if(falseVal2 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    if(falseVal3 == true)
    {
      Serial.print("Check Check Check");
      falseValCheck = 1;
    }
    /*if (val1 == 1)                        
    {
      falseValCheck = true;
    }else if(val2 == 1) 
    {
      falseValCheck = true;
    }else if(val3 == 1)
    {
      falseValCheck = true;
    }*/
  }
}

void ReadHiscore() {
  highScore = (EEPROM.read(0) << 8) + EEPROM.read(1);

  // EEPROM initial value is FFFF
  if(highScore == 0xffff)
    highScore = 0;
}

void WriteHiscore() {
  EEPROM.write(0, highScore >> 8);
  EEPROM.write(1, highScore & 0xff);
}

void ScoreCounter()                         //Counts the score and prints to the screen current score
{
  ReadHiscore();
  score++;
  scoreCheck++;
  lcd.clear();
  lcd.print("Score: ");
  lcd.print(score);
}
void SpeedGame()
{
  timeValue = 1000;
  timer = timeValue;
  scoreCheck = 0;
  delay(1000);                                    //Wait 1 second before starting the game
  lcd.clear();                                    //Print to the LCD-screen the initial score setup (Score funktion will update it further)
  lcd.print("Score: ");
  lcd.print(score);
  while(i != 9)                                   //Start the actual game
  {
    number = random(4);                           //Sets up a random number value between 0-3 for Number variable.
    ButtonsOn(number);                            //Turns on the light for that button
    while (timer > 0)    //Turns on the timer
    { 
      CheckButtonState(number);                   //Checks the if that button has been pressed
      CheckFalseButtonState(number);
      timer--;                                    //Timer calculator
      if (valCheck == 1 || falseValCheck == 1)         //Checks if the button has been pressed, and if yes sets timer to 0
      {
        timer = 0;
      }
      //Serial.print(timer);
      //Serial.print("    ");
      Serial.println(timer);                       //For an unknown reason must have print otherwise won't work for somereason
    }
    if (valCheck == 1)                             //If timer is 0 (Will be set to 0 if button has been pressed) and button has been pressed
    {
       ScoreCounter();                             //Sets up scores accordingly to everywhere.
       ButtonsOff(number);                         //Turns off the light from the pressed button
       if (scoreCheck == 4)
       {
        if (timeValue <= 500)
        {
          timeValue = timeValue*0.7;
          scoreCheck = 0;
        }else if(timeValue <= 100)
        {
          timeValue = 100;
        }else
        {
          timeValue = timeValue - 100;
          scoreCheck = 0;
        }
       }
       timer = timeValue;                          //Resets the timer
       valCheck = 0;                               //Resets the button press
       delay(200);
       //PlayGame();
      /* Serial.print("Timer: ");
       Serial.print(timer);
       Serial.print(" score: ");
       Serial.print(score);
       Serial.print(" val: ");
       Serial.print(val);
       Serial.print(" valCheck: ");
       Serial.println(valCheck);*/
    }else                                          //If you haven't pressed the button and timer has run out
    {
      if(score >= highScore)
      {
        highScore = score;
        WriteHiscore();
      }
      ReadHiscore();
      lcd.clear();                                 //Writes the final scores to the LCD-screen and leaves them for 5 seconds
      lcd.print("Final: ");
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("Best: ");
      lcd.print(highScore);
      delay(5000);
      falseValCheck = 0;
      i = 9;                                       //Gets out of the the timer and score and gets out of the game.
      timer = 1000;
      score = 0;
      
    }
  }
  speedGame = false;                              //Gets back to the main menu and resets the gamestate to be possible to play again.
  game1 = true;
  mainMenu = true;
  menuOnce = false;
  i = 0;
}
   /* ScoreCounter();
    ButtonsOff(number);
    timer = 500;
    valCheck = 0;
    delay(500);
    Serial.print("Timer: ");
    Serial.print(timer);
    Serial.print(" score: ");
    Serial.print(score);
    Serial.print(" val: ");
    Serial.print(val);
    Serial.print(" valCheck: ");
    Serial.println(valCheck);*/
void MemoryGame()
{
  int nArray[maxlvl];
  a = 0;
  j = 0;

  for(a = 0; a < maxlvl; a++){
    nArray[a] = 0;
    randNum = random(1,5);
    nArray[a] = randNum;
    randNum = 0;
  }

 /* for(i = 0; i < maxlvl; i++)
  {
    Serial.print(nArray[i]);
  }
  Serial.println();*/
  
  for(lvl = 1; lvl <= maxlvl; lvl++)
  {
    for(a = 0; a < lvl; a++)
    {
  
      if(nArray[a] == 1)
      {
        digitalWrite(button1Light, HIGH);
        delay(500);
      }

      else if(nArray[a] == 2)
      {
        digitalWrite(button2Light, HIGH);
        delay(500);
      }

      else if(nArray[a] == 3)
      {
        digitalWrite(button3Light, HIGH);
        delay(500);
      }

      else if(nArray[a] == 4)
      {
        digitalWrite(button4Light, HIGH);
        delay(500);
      }

      digitalWrite(button1Light, LOW);
      digitalWrite(button2Light, LOW);
      digitalWrite(button3Light, LOW);
      digitalWrite(button4Light, LOW);
    
      delay(100);
    }

    for(j = 0; j < lvl; j++)
    {

      buttonPress1 = 0;
      buttonPress2 = 0;
      buttonPress3 = 0;
      buttonPress4 = 0;
    
      if(nArray[j] == 1)
      {
        while(buttonPress1 == 0 && buttonPress2 == 0 && buttonPress3 == 0 && buttonPress4 == 0)
        {
        
          buttonState1 = digitalRead(button1);
          buttonState2 = digitalRead(button2);
          buttonState3 = digitalRead(button3);
          buttonState4 = digitalRead(button4);
        
          if(buttonState1 == 1)
          {
          buttonPress1 = 1;
          }
          else if(buttonState2 == 1)
          {
          buttonPress2 = 1;
          }
          else if(buttonState3 == 1)
          {
          buttonPress3 = 1;
          }
          else if(buttonState4 == 1)
          {
          buttonPress4 = 1;
          }
          delay(20);
        }
        if(buttonPress1 == 0)
        {
          lvl = 20;
          j = 20;
          buttonPress1 = 0;
          buttonPress2 = 0;
          buttonPress3 = 0;
          buttonPress4 = 0;
          
          memoryGameOver();
        }
      }
      if(nArray[j] == 2)
      {
        while(buttonPress1 == 0 && buttonPress2 == 0 && buttonPress3 == 0 && buttonPress4 == 0)
        {
        
          buttonState1 = digitalRead(button1);
          buttonState2 = digitalRead(button2);
          buttonState3 = digitalRead(button3);
          buttonState4 = digitalRead(button4);
        
          if(buttonState1 == 1)
          {
          buttonPress1 = 1;
          }
          else if(buttonState2 == 1)
          {
          buttonPress2 = 1;
          }
          else if(buttonState3 == 1)
          {
          buttonPress3 = 1;
          }
          else if(buttonState4 == 1)
          {
          buttonPress4 = 1;
          }
          delay(20);
        }
        if(buttonPress2 == 0)
        {
          lvl = 20;
          j = 20;
          buttonPress1 = 0;
          buttonPress2 = 0;
          buttonPress3 = 0;
          buttonPress4 = 0;
          
          memoryGameOver();
        }
      }
      if(nArray[j] == 3)
      {
        while(buttonPress1 == 0 && buttonPress2 == 0 && buttonPress3 == 0 && buttonPress4 == 0)
        {
        
          buttonState1 = digitalRead(button1);
          buttonState2 = digitalRead(button2);
          buttonState3 = digitalRead(button3);
          buttonState4 = digitalRead(button4);
        
          if(buttonState1 == 1)
          {
          buttonPress1 = 1;
          }
          else if(buttonState2 == 1)
          {
          buttonPress2 = 1;
          }
          else if(buttonState3 == 1)
          {
          buttonPress3 = 1;
          }
          else if(buttonState4 == 1)
          {
          buttonPress4 = 1;
          }
          delay(20);
        }
        if(buttonPress3 == 0)
        {
          lvl = 20;
          j = 20;
          buttonPress1 = 0;
          buttonPress2 = 0;
          buttonPress3 = 0;
          buttonPress4 = 0;
          
          memoryGameOver();
        }
      }
      if(nArray[j] == 4)
      {
        while(buttonPress1 == 0 && buttonPress2 == 0 && buttonPress3 == 0 && buttonPress4 == 0)
        {
        
          buttonState1 = digitalRead(button1);
          buttonState2 = digitalRead(button2);
          buttonState3 = digitalRead(button3);
          buttonState4 = digitalRead(button4);
        
          if(buttonState1 == 1)
          {
          buttonPress1 = 1;
          }
          else if(buttonState2 == 1)
          {
          buttonPress2 = 1;
          }
          else if(buttonState3 == 1)
          {
          buttonPress3 = 1;
          }
          else if(buttonState4 == 1)
          {
          buttonPress4 = 1;
          }
          delay(20);
        }
        if(buttonPress4 == 0)
        {
          lvl = 20;
          j = 20;
          buttonPress1 = 0;
          buttonPress2 = 0;
          buttonPress3 = 0;
          buttonPress4 = 0;

          memoryGameOver();
        }
      }
      while(buttonState1 == 1 || buttonState2 == 1 || buttonState3 == 1 || buttonState4 == 1)
      {
        buttonState1 = digitalRead(button1);
        buttonState2 = digitalRead(button2);
        buttonState3 = digitalRead(button3);
        buttonState4 = digitalRead(button4);
        delay(10);
      }
    }
  }
}
void memoryGameOver()
{
  delay(200);
  buttonState1 = 0;
  while(buttonState1 == 0)
  {
    buttonState1 = digitalRead(button1);
    digitalWrite(button1Light, HIGH);
    delay(100);
    digitalWrite(button1Light, LOW);
    delay(100);
  }
  /*lcd.clear();
  lcd.print("points: ");
  lcd.print(lvl);
  delay(3000);*/
  memoryGame = false;
  speedGame = false;
  reactionGame = false;
  game1 = true;
  mainMenu = true;
  menuOnce = false;
  //memTerminate = true;
  delay(500);
  loop();
}

void reactionRandom(){
  Serial.println(" ");
  lcd.begin(16,2);                   
  lcd.clear();
  delay(50);
  lcd.print("Valmistaudu!!!!");
  randomTime = random(4,10);
  randomTime = randomTime*1000;
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(randomTime);
  ReactionGame();
}

void ReactionGame(){
  PeliOn = true;  //pelin aloitus 
  startTime = millis();
  randomPin = pins[0]; // futureproofin takia jos haluu lisätä lisää pelaajia 
  digitalWrite(randomPin, HIGH);

  if(digitalRead(pins[0]) == HIGH){ 
  while(pins[0] == HIGH || P1Valmis == false){

      NapinAsentoP1 = digitalRead(switchPin1); // kysytään nappulaa jolla peliä pelataan
      if (NapinAsentoP1 == HIGH && P1Valmis == false) {
        endTimeP1 = millis(); //paljo pelin ajaksi tulu
        P1Valmis = true;     // peli valmis
      }
    }
  }

  reactionGameOver();
}

void reactionGameOver(){
  digitalWrite(ledPin, LOW);
  LopullinenAikaP1 = (endTimeP1 - startTime);
  lcd.clear(); 
  lcd.print("P1 Aika:");
  lcd.print(LopullinenAikaP1/1000);
  delay(2500);
  lcd.clear();
  digitalWrite(ledPinP1, LOW);
  delay(randomTime/2);

  delay(1000);
  reactionGame = false;
  game1 = true;
  mainMenu = true;
  menuOnce = false;
  loop();
 
  NapinAsentoP1 = 0;
  lastNapinAsentoP1 = 0;
  PeliOn = false;
  StartNappiState = LOW;
  P1Valmis = false;
  randomTime = 0;
  startTime = 0;
  endTimeP1 = 0;
  LopullinenAikaP1 = 0;
  TervetuloViesti = false;
}

void loop() {
  // put your main code here, to run repeatedly:

  if(mainMenu == true)
  {
      Menu();
  }
  if(speedGame == true)
  {
      SpeedGame();
  }
  if(memoryGame == true)
  {
      delay(1000);
      MemoryGame();
  }
  if(reactionGame == true){
 if (TervetuloViesti == false){
    Serial.println(" "); //tää kohta piti lisätä koska ilman koodi ei toiminut lopullisessa koodissa serial printit poistuu ja muutetaan tulostus lcd näytölle
    lcd.clear();
    lcd.print("Paina 4 aloittaaksesi");
    TervetuloViesti = true;
  }
  StartNappiState = digitalRead(switchStart);
  if (StartNappiState == HIGH && PeliOn == false){
    reactionRandom();
   }
  }
}
