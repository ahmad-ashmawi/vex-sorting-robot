#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START IQ MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS


// Robot configuration code.
inertial BrainInertial = inertial();


// generating and setting random seed
void initializeRandomSeed(){
  wait(100,msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  // Combine these values into a single integer
  int seed = int(
    xAxis + yAxis + zAxis
  );
  // Set the seed
  srand(seed); 
}



void vexcodeInit() {

  // Initializing random seed.
  initializeRandomSeed(); 
}

#pragma endregion VEXcode Generated Robot Configuration

//----------------------------------------------------------------------------
//
//    Module:       main.cpp
//    Author:       Martin Montgomery, Anshia Yaqoob, Ahmad Ashmawi, & Jaime McGale
//    Created:      November 19, 2025
//    Description:  Robot Project - MTE 100 & MTE 121 
//
//----------------------------------------------------------------------------


#include "iq_cpp.h" // Include the IQ Library
using namespace vex; // Allows for easier use of the VEX Library

// Configuration of Sensors
void configureAllSensors() {
  BrainInertial.calibrate();
  wait(2, seconds);
  BrainInertial.setHeading(0, degrees);
  BrainInertial.setRotation(0, degrees);
  MotorLeft.setPosition(0, turns);
  MotorRight.setPosition(0, turns);
  ClawMotor.setPosition(0, turns); // set positions of motors
  LiftMotor.setPosition(0, turns);
  Brain.Screen.clearScreen();
  Brain.Screen.setFont(mono15);
  Optical4.setLight(ledState::on);
}


// ------------------- FUNCTION PROTOTYPES -------------------

void move_lift(bool  move);
void claw(bool grab);
void claw_reset();
void use_claw();
void let_go();
int getCategoryInput();
bool findStripe (int colorNum);
void rotate(int angle);
void driveToBox ();
void moveBackwards(double dist, int motorPower, double Angle);
void driveStraight(double Angle, double motorPower);
double difference(double arr[], int size);





// ------------------- FUNCTION DEFINITIONS -------------------


// ______________________ Claw & lift

void move_lift(bool  move)
{
  LiftMotor.setVelocity(10, percent);
  if (move)
  {
    //moves lift up
    LiftMotor.spin(forward);
    while (LiftMotor.position(turns) < 0.44){}
    LiftMotor.stop();
  }
  else if(!move)
  {
    // moves lift down
    LiftMotor.spin(reverse);
    while(LiftMotor.position(turns) > 0.05) {}
    LiftMotor.stop();
  }
}

void claw(bool grab){
  ClawMotor.setVelocity(10, percent);

  if (grab)
  {
    //opens claw
    ClawMotor.spin(forward);
    while (ClawMotor.position(degrees) < 20){}
    ClawMotor.stop();
  }
  else if(!grab)
  {
    //Closes claw
    ClawMotor.spin(reverse);
    // should not stop so as to grip on to the animal
  }
}
void claw_reset(){
  // resets the claw to close position and turns it off
  ClawMotor.spin(reverse);
  while (ClawMotor.position(degrees) > 0.5){}
  ClawMotor.stop();
}

void use_claw(){
  claw(1);  // open claw
  wait(5, seconds);  // wait for user to put object
  claw(0); //  grap object
  wait(0.5, seconds); // gives time to pick the object up
  move_lift(1);  // moves it up
}
void let_go(){
  move_lift(0);  // bring it down
  wait(1, seconds);
  claw(1); // let go of object
  move_lift(1); // brings the claw up
  claw_reset(); // resets the claw to starting position
}

// gets the user input
int getCategoryInput(){
  int tapCount = 0;
  int idleTime = 0;
  int totalIdle = 0;
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Tap 1-3 times.");
  Brain.Screen.newLine();
  Brain.Screen.print("4+ taps = invalid");

  while (true)
  {
    tapCount = 0;
    idleTime = 0;

    while (idleTime < 15 && totalIdle < 80)
    {
      if (TouchLED1.pressing())
      {
        while (TouchLED1.pressing())
        {
          wait(0.1, seconds);
        }

        tapCount++;
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Taps: %d", tapCount);

        // changes touchLED color
        if(tapCount==1){
          TouchLED1.setColor(colorType::blue);
        } else if(tapCount==2){
          TouchLED1.setColor(colorType::yellow);
        } else if(tapCount==3){
          TouchLED1.setColor(colorType::green);
        } else{
          TouchLED1.setColor(colorType::black);
        }

        idleTime = 0;  // reset short inactivity timer
        wait(0.5, seconds);
      }
      else
      {
        idleTime++;
        totalIdle++;
        wait(0.1, seconds);
      }

      // Check for shutdown here inside the loop
      if (totalIdle >= 60)
      {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Bye bye!");
        wait(2, seconds);
        Brain.programStop();  // fully stop program
      }
    }

    // Validate tap count
    if (tapCount >= 1 && tapCount <= 3) {
      return tapCount;
    }
    else {
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("Invalid input!");
      wait(2, seconds);
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("Tap 1-3 times to choose a box");
      Brain.Screen.newLine();
      Brain.Screen.print("4+ taps = invalid");
    }
  }
}

bool findStripe (int colorNum){
  Brain.Timer.reset();

  const int MotorPower = 30;
  MotorLeft.setVelocity(MotorPower,percent);
  MotorRight.setVelocity(MotorPower,percent);

  MotorLeft.spin(forward);
  MotorRight.spin(forward);

  vex::color targetColor;

  // sets the target variable to corresponding color
  if (colorNum ==1)
    targetColor = blue;
  else if (colorNum ==2)
      targetColor = yellow;
  else if (colorNum ==3)
    targetColor = green;
  else
  {
    MotorLeft.stop();
    MotorRight.stop();
    return 0;
  }

  //drives until it detects color
  while (Optical4.color()!=targetColor){

    if (Brain.Timer.time(seconds) >= 15){
      MotorLeft.stop();
      MotorRight.stop();

        return 0; //if no colour is found after 15 seconds
    }
  }
  MotorLeft.stop();
  MotorRight.stop();

    return 1;
}

void rotate(int angle){
  // the max motor speed
  const int maxMotorPower = 20;
  float intendedAngle = angle+BrainInertial.rotation(degrees);
  MotorLeft.setVelocity(maxMotorPower,percent);
  MotorRight.setVelocity(maxMotorPower,percent);
  if (angle<0){
    //rotates clockwise
    MotorLeft.spin(forward);
    MotorRight.spin(reverse);
  }else{
    //rotates counterClockwise
    MotorLeft.spin(reverse);
    MotorRight.spin(forward);
  }

  // USING pid control which I was taught during the beginning of the lecture
  // on Monday November 10th by Mike
  // values where tested followed https://www.youtube.com/watch?v=IB1Ir4oCP5k
  // to learn how to tune a PID
  double kp = 1.25; // 1.25
  double ki = 0.000000025; // 0.00015
  double kd = 0.25;

  double errorSum = 0;
  double errorLast = 0; // to calculate change in speed
  // tolerance in degrees
  double tolerance = 1;
  while(fabs(BrainInertial.rotation(degrees)-intendedAngle)>tolerance){
    // how far it is off angularly
    double error = intendedAngle-BrainInertial.rotation(degrees);
    errorSum += error; // integral of error sum
    double motorCorrection = kp*error + ki*errorSum + kd*(error-errorLast);

    // caps the motor speed
    if (motorCorrection>maxMotorPower){
      motorCorrection = maxMotorPower;
    }
    if(motorCorrection<((-1.0)*maxMotorPower)) {
      motorCorrection = (-1.0)*maxMotorPower;
    }
    MotorLeft.setVelocity(-motorCorrection,percent);
    MotorRight.setVelocity(motorCorrection,percent);
    errorLast = error;
    //Brain.Screen.printAt(10, 50, "%f", BrainInertial.rotation(degrees));
    wait(5,msec);
  }
  MotorLeft.stop(brake);
  MotorRight.stop(brake);
  MotorLeft.setPosition(0,turns);
  MotorRight.setPosition(0,turns);
  wait(0.1,seconds);
}
// This function just returns the average of the array
double average(double arr[], int size){
  double sum = 0;
  for(int i = 0; i<size; i++){
    sum += arr[i];
  }
  return(sum/size);
}

void driveToBox (){
  const int motorPower = 20;
  const double Angle = BrainInertial.rotation(degrees);
  MotorRight.spin(forward, motorPower,  percent);
  MotorLeft.spin(forward, motorPower,  percent);
  const int size = 10;
  double arr[size] = {30,30,30,30,30,30,30,30,30,30};
  // Initializer array to being bigger than 20

  // keeps on checking if the average distance sensor output is
  while (average(arr,size)>20){
    arr[size-1] = Distance9.objectDistance(mm);
    for(int i = 0; i<(size-1); i++){
      arr[i] = arr[i+1];
    }
    driveStraight(Angle, motorPower);
    wait(10,msec);
  }
  MotorLeft.stop();
  MotorRight.stop();
}
// Is a function that can be called in a loop
// Allows the robot to drive straight
void driveStraight(double Angle, double motorPower){
  double correction = Angle-BrainInertial.rotation(degrees);

  const int absCorrection = 5;
  if(correction>absCorrection){
    correction =absCorrection;
  }
  if(correction<-absCorrection){
    correction = -absCorrection;
  }
  MotorLeft.setVelocity(motorPower-correction,percent);
  MotorRight.setVelocity(motorPower+correction,percent);
}

void moveBackwards(double dist, int motorPower, double Angle){
  //Drives the robot straight backwards being used to return to start

  MotorLeft.setPosition(0,turns);
  MotorRight.setPosition(0,turns);

  MotorLeft.setVelocity(motorPower,percent);
  MotorRight.setVelocity(motorPower,percent);
  MotorLeft.spin(reverse);
  MotorRight.spin(reverse);

  const double tolerance = 1;
  while(fabs(dist-((MotorRight.position(turns)+MotorLeft.position(turns))/2)*200)>tolerance){
    double correction = Angle-BrainInertial.rotation(degrees);
    double error = (dist-((MotorRight.position(turns)
    +MotorLeft.position(turns))/2)*200);
    //Brain.Screen.printAt(10, 50, "%f",error);
    // limits the motor power
    if (error>motorPower){
      error = motorPower;
    }else if(error<((-1.0)*motorPower)) {
      error = (-1.0)*motorPower;
    }
    const int absCorrection = 5;
    if(correction>absCorrection){
      correction =absCorrection;
    } else if(correction<-absCorrection){
      correction = -absCorrection;
    }
    // motor power is going to be set at most 5% more than motor power
    // inputted. This is due to correction having its own cap of 5% separate
    // from motorPower.
    MotorLeft.setVelocity(error-correction,percent);
    MotorRight.setVelocity(error+correction,percent);
  }
  MotorLeft.stop(brake);
  MotorRight.stop(brake);
}





// ------------------- MAIN PROGRAM -------------------

int main(){
  vexcodeInit();
  configureAllSensors();

  while(true){
    MotorLeft.setPosition(0,turns);
    MotorRight.setPosition(0,turns);
    double xPos = 0;
    double yPos = 0;
    int category = getCategoryInput();
    use_claw();
    if(findStripe(category)!=0){
      xPos = (MotorRight.position(turns)+MotorLeft.position(turns))*200/2;
      rotate(90);
      driveToBox();
      let_go();

      TouchLED1.setColor(colorType::black);
      TouchLED1.off();

      yPos = (MotorRight.position(turns)+MotorLeft.position(turns))*200/2;
      moveBackwards(-yPos, 30, BrainInertial.rotation(degrees));
      rotate(-90);
      MotorLeft.setPosition(0,turns);
      MotorRight.setPosition(0,turns);
    }
    // if it goes through if statement should add 0
    xPos += (MotorRight.position(turns)+MotorLeft.position(turns))*200/2;
    TouchLED1.setColor(colorType::black);
    TouchLED1.off();
    moveBackwards((-xPos), 30, 0);
    move_lift(0); // brings the claw back down
  }
}

