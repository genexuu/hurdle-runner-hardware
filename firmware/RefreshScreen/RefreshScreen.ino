#include <LiquidCrystal.h>


LiquidCrystal lcd(P6_7, P2_3, P2_6, P2_4, P5_6, P6_6);

OneMsTaskTimer_t timerTask2 = {250, RefreshScreenTimerISR, 0, 0};
enum RefreshScreens {START2, DrawScreen1, DrawScreen2, GameOver};

RefreshScreens state2;

int timerFlag2 = 0;
int score = 0;

byte hero[8] = {
  B00110,
  B00110,
  B00000,
  B01110,
  B00111,
  B00110,
  B00101,
};

byte OBSTACLE[8] = {
  B01110,
  B10001,
  B11011,
  B10001,
  B01110,
  B11011,
  B00100,
  B11011,
};

void setup() {
  state2 = START2;
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.createChar(0, hero);
  lcd.createChar(1, OBSTACLE);
  OneMsTaskTimer::add(&timerTask2);
  OneMsTaskTimer::start();

}

void refreshScreen(){
  switch(state2){
    case START2:
       Serial.println("Transition: Start to DrawScreen1");
       state2 = DrawScreen1;
       break;
    case DrawScreen1:
       obstacle.x = 15;
       obstacle.y = 1;
       obstacle2.x = 20;
       obstacle2.y = 1;
       Serial.println("Transition: DrawScreen1 to DrawScreen2");
       state2 = DrawScreen2;
    case DrawScreen2:
       drawHero();
       drawObstacle();
       drawObstacle2();
       if((HeroLocation.x == obstacle.x) && (HeroLocation.y == obstacle.y)){
           displayGameOver();
           Serial.println("Transition: DrawScreen2 to Game Over");
           state2 = GameOver;
       }
       else if((HeroLocation.x == obstacle2.x) && (HeroLocation.y == obstacle2.y)){
           displayGameOver();
           Serial.println("Transition: DrawScreen2 to Game Over");
           state2 = GameOver;
       }
       else{
        //score incresed with time (250ms = 1 score)
        score++;
       }
       break;
  }
}

void RefreshScreenTimerISR(){
  timerFlag2 = 1;
}

void loop() {
  // put your main code here, to run repeatedly: 
  while( timerFlag2 == 0 ) { delay(10); }
  timerFlag2 = 0;
  refreshScreen();
 
  
}



void drawHero(){
  //Erase the previous hero position by drawing a blank character in the old position
  lcd.setCursor(prevHeroLocation.x, prevHeroLocation.y);
  lcd.print(" ");
  
  //Draw the hero in the new position
  lcd.setCursor(HeroLocation.x, HeroLocation.y);
  lcd.write(byte(0));
  
  //Set the previous hero position to the current position
  prevHeroLocation = HeroLocation;
  
}

void drawObstacle(){
  // Erase the previous obstacle position by drawing a blank character in the old position
  lcd.setCursor(prevobstacle.x, prevobstacle.y);
  lcd.print(" ");
  
  //Draw the position in the new position
  lcd.setCursor(obstacle.x, obstacle.y);
  lcd.write(byte(1));
  
  //Set the previous obstacle position to the current obstacle position
  prevobstacle = obstacle;
  
  //Move the obstacle position to the left by one. 
  (obstacle.x)--;
  
  //Check if the obstacle is now < 0. If so, move the position all the way to the right side of the screen
  if(obstacle.x < 0){
    obstacle.x = 15;
  }
}

void drawObstacle2(){
  // Erase the previous obstacle position by drawing a blank character in the old position
  lcd.setCursor(prevobstacle2.x, prevobstacle2.y);
  lcd.print(" ");
  
  //Draw the position in the new position
  lcd.setCursor(obstacle2.x, obstacle2.y);
  lcd.write(byte(1));
  
  //Set the previous obstacle position to the current obstacle position
  prevobstacle2 = obstacle2;
  
  //Move the obstacle position to the left by one. 
  (obstacle2.x)--;
  
  //Check if the obstacle is now < 0. If so, move the position all the way to the right side of the screen
  if(obstacle2.x < 0){
    obstacle2.x = 20;
  }
}

void displayGameOver(){
  //Erase the entire LCD screen
  lcd.begin(16, 2);
  //Display Game Over
  lcd.print("Game Over!");
  
    // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  // print the score
  lcd.print(score);
}
