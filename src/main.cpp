/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Teacher                                                   */
/*    Created:      12/20/2023, 3:40:31 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;



// A global instance of competition
competition Competition;



// define your global instances of motors and other devices here

motor Cata = motor(PORT3);
motor Lift = motor(PORT5);

motor MotorLF = motor(PORT20, ratio18_1, true); // reversed 
motor MotorLB = motor(PORT19, ratio18_1, true); // reversed
motor MotorRF = motor(PORT11, ratio18_1, false); // forward direction
motor MotorRB = motor(PORT12, ratio18_1, false); // forward direction

motor_group LeftMotors = motor_group(MotorLF, MotorLB);
motor_group RightMotors = motor_group(MotorRF, MotorRB);


brain Brain;
digital_in CataStop = digital_in(Brain.ThreeWirePort.C);
controller Controller1 = controller(primary);
bool CataPultOn = false;

int ShowMeInfo(){
  while (true) {
    Brain.Screen.setCursor(4,1);
    Brain.Screen.print("Digital In C");
    Brain.Screen.setCursor(5,1);
    Brain.Screen.print(CataStop.value());

    wait(25, msec);
  } 
  return 0;
}


void CatapultStop()
{
  Cata.setBrake(hold);
  Cata.spin(forward, 6, vex::volt);
  waitUntil(CataStop.value()==0);
  Cata.stop(hold);
  CataPultOn = false;

};

void CatapultStart()
{
  Cata.spin(forward, 8, vex::volt);
  CataPultOn = true;
};

void Catapult()
{
  if (!CataPultOn)
  {
    CatapultStart();
  }
  else
  {
    CatapultStop();
  };
}
 


void liftUp()
{ 
  Lift.spin(forward, 5, vex::volt);
  waitUntil(!Controller1.ButtonUp.pressing());
  Lift.stop(hold);
}

void liftDown()
{
  Lift.spin(reverse, 5, vex::volt);
  waitUntil(!Controller1.ButtonDown.pressing());
  Lift.stop(hold);
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  vex::task MyTask(ShowMeInfo);

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {


  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
    CatapultStop();
  Controller1.ButtonL2.pressed(Catapult);
  Controller1.ButtonUp.pressed(liftUp);
  Controller1.ButtonDown.pressed(liftDown);
 
  // User control code here, inside the loop
  while (1) {
    RightMotors.setVelocity((Controller1.Axis3.position() - Controller1.Axis1.position()), percent);
    LeftMotors.setVelocity((Controller1.Axis1.position() + Controller1.Axis3.position()), percent);
    RightMotors.spin(forward);
    LeftMotors.spin(forward);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
