
/*
PIN Configuration
Buttons
2 -> UP Button
3 -> DOWN Button
4 -> Enter Button
5 -> Start/Stop Button

Stepper Motor Driver
10 -> IN1             
11 -> IN2           
12 -> IN3           
13 -> IN4

LCD Display
14 -> RS
15 -> Enable
16 -> D4
17 -> D5
18 -> D6
19 -> D7
*/

 // Include the Libraries
 #include <LiquidCrystal.h>
 #include <Stepper.h>
 
 // initialize the LCD 
 LiquidCrystal lcd (14, 15, 16, 17, 18, 19);

 //initialize the stepper (assume 200 steps motor)
 const int stepsPerRevolution = 200;
 int speedLevel = 0;
 int rpm = 0;
 #define MAX_SPEED_LEVEL  23
 #define MIN_SPEED_LEVEL  0
  
 Stepper stepper(stepsPerRevolution, 10,11,12,13);


 // Initialize the PINs variables
 //        Name           PIN 
 const int upButton      = 2;
 const int downButton    = 3;
 const int enterButton   = 4;
 const int startButton   = 5;
 
 int upState     = LOW;
 int downState   = LOW;
 int enterState  = LOW;
 int startState  = LOW;

// local functions declarations
void speedSelection();
void runStepper();

// Function Setup 
void setup() {
  // put your setup code here, to run once:

  // Set the PIN Modes - INPUT OR OUTPUT
  pinMode(upButton,      INPUT);
  pinMode(downButton,    INPUT);
  pinMode(enterButton,   INPUT);
  pinMode(startButton,   INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);  

}

void loop() {
  // put your main code here, to run repeatedly:

  // clear screen for the next loop:
  lcd.clear();
    
  // Initialize the Button State 
  upState     = LOW;
  downState   = LOW;
  enterState  = LOW;
  startState  = LOW;

  // Start Screen
  // Loop till START is pressed
  while(true)
  {
     // Start Message Display
     lcd.setCursor(2, 0);
     lcd.print("Press START");

     startState = digitalRead(startButton);

     // START is pressed 
     if (startState == HIGH)
     {
        while (startState == HIGH)
              startState = digitalRead(startButton);
        break;
     }    
  }
  // The START is pressed
  // Continue to SPEED Selection
  
  // Call the routine to select the speed
  speedSelection();

  // Confirm to Start or Reselect Speed
  while(true)
  {    
     lcd.setCursor(0, 0);
     lcd.print("START to Run");

     lcd.setCursor(0, 1);
     lcd.print("ENTER to Select");
  
     enterState = digitalRead(enterButton);
     startState = digitalRead(startButton);

     // If Start Button is pressed
     // Call the Stepper Routine to start stepper
     if (startState == HIGH)
     {
        while (startState == HIGH)
              startState = digitalRead(startButton);
              
        runStepper();
        break;   
     }

     // If Enter Button then reslect Speed 
     if (enterState == HIGH)
     {
        while (enterState == HIGH)
              enterState = digitalRead(enterButton);

        speedSelection();
     }      
  }
}


// Stepper Operations Routine
void runStepper()
{
   delay(200);
   
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("START to Reset");

   lcd.setCursor(0, 1);
   lcd.print("Speed: ");
   lcd.print(rpm,DEC);
   lcd.print(" RPM");
   
   stepper.step(1);
   stepper.setSpeed(rpm);

   delay(100);
     
   while(true)
   {    
     startState = digitalRead(startButton);
          
     if (startState == HIGH)
     {
        while (startState == HIGH)
              startState = digitalRead(startButton);
              
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Stepper is stopping");
        delay (100);
        stepper.setSpeed(50);
        delay (300);
        stepper.setSpeed(0);
        delay (500);
        break;
     } 
   }
}


// Speed Selection Routine
void speedSelection()
{
  // clear screen for the next loop:
  delay (150);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Select Speed");
  bool newSpeedSelected;
  
  while(true)
  {   
     newSpeedSelected = false;
           
     upState   = digitalRead(upButton);
     downState = digitalRead(downButton);

     if (upState == HIGH)
     {
        while (upState == HIGH)
              upState = digitalRead(upButton);

        // Increase the speed level till we reach MAX
        if (speedLevel <= MAX_SPEED_LEVEL)
        {
           speedLevel++;
           newSpeedSelected = true;
        }   
     }
     else if (downState == HIGH)
     {
        while (downState == HIGH)
              downState = digitalRead(downButton);

        // Decrease the speed level till we reach MIN
        if (speedLevel > MIN_SPEED_LEVEL)
        {
           speedLevel--;
           newSpeedSelected = true;
        }          
     }

     if (newSpeedSelected)
     {
        lcd.setCursor(0, 0);
        lcd.print("ENTER to Confirm");
       
        lcd.setCursor(0, 1);
        lcd.print("Speed :         ");
         
        rpm = speedLevel*100;
        lcd.setCursor(11, 1);
        lcd.print(rpm,DEC);
     }
          
     enterState = digitalRead(enterButton);

     // Enter is pressed 
     if (enterState == HIGH)
     {
        while (enterState == HIGH)
              enterState = digitalRead(enterButton);
        break;
     }              
  }

  lcd.clear();
}
     

