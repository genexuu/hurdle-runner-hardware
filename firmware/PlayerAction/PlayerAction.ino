 #include "OneMsTaskTimer.h"
OneMsTaskTimer_t timerTask = {250, playerActionTimerISR, 0, 0};

enum playerActions {START, GameStart, WaitingforAction, MoveForward, MoveBack, Jump};
playerActions state;

typedef struct xy_struct{
  int x;
  int y;
} XY;

XY HeroLocation;
XY prevHeroLocation;
XY obstacle;
XY prevobstacle;
XY obstacle2;
XY prevobstacle2;

int timerFlag = 0;
int forwardFlag = 0;
int backwardFlag = 0;
int jumpFlag = 0;
int leftThreshold = 250;
int rightThreshold = 800;
int VRX = A8;
int selectPin = P4_7;
int sensorValue = 0;
int cnt;


void setupPlayerActions() {
  state = START;
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(selectPin, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(selectPin), jumpISR, FALLING);
  OneMsTaskTimer::add(&timerTask);
  OneMsTaskTimer::start();

}

void playerAction(){
  //State Transition
  switch(state){
    case START:
        Serial.println("Transition: Start to Game Start");
        state = GameStart;
        break;
    case GameStart:
        Serial.println("Transition: Start to Waiting for Action");
        state = WaitingforAction;
        break;
    case WaitingforAction:
        if(forwardFlag == 1){
          Serial.println("Transition: Game Start to Move Forward");
          forwardFlag = 0;
          state = MoveForward;
        }
        else if (backwardFlag == 1){
          Serial.println("Transition: Game Start to Move back");
          backwardFlag = 0;
          state = MoveBack;
        }
        else if (jumpFlag == 1){
          Serial.println("Transition: Game Start to Jump");
          jumpFlag = 0;
          state = Jump;
        }
        break;
     case MoveForward:
        Serial.println("Transition: Move Forward to WaitingforAction");
        state = WaitingforAction;
        break;
     case MoveBack:
        Serial.println("Transition: Move Back to WaitingforAction");
        state = WaitingforAction;
        break;
     case Jump:
        if(cnt >= 2){
        Serial.println("Transition: Jump to WaitingforAction");
        state = WaitingforAction;
        }
        break;
  }

   //State Actions
      switch(state){
        case START:          
           break;
        case GameStart:
           HeroLocation.x = 0;
           HeroLocation.y = 1;
           break;
        case WaitingforAction:
          HeroLocation.y = 1;
          cnt = 0;
           break;
        case MoveForward:
           (HeroLocation.x)++;
           break;
        case MoveBack:
           (HeroLocation.x)--;
           break;
        case Jump:      
           HeroLocation.y = 0;
           cnt++;
           break;
      }
}

void playerActionTimerISR(){
  timerFlag = 1;
}

void loopPlayerActions() {

 while( timerFlag == 0 ) { delay(10); }
  timerFlag = 0;
  //Serial.println("click");
  sensorValue = analogRead(VRX);
  if(sensorValue > rightThreshold){
    forwardjoystick();
  }
  if(sensorValue < leftThreshold){
    backforwardjoystick();
  }
  playerAction();
  printLocation();
  delay(10);
}

void jumpISR(){
  //Serial.println("Jump");
  jumpFlag = 1;
}

void forwardjoystick(){
  //Serial.println("Forward");
  forwardFlag = 1;
}

void backforwardjoystick(){
  //Serial.println("Backforward");
  backwardFlag = 1;
}

void printLocation(){
  Serial.print("Hero Position: ");
  Serial.print(HeroLocation.x);
  Serial.print(" ");
  Serial.println(HeroLocation.y);
}
