#include <Servo.h>                      // Include servo library

Servo servoLeft;
Servo servoRight;

int RightAlignedLevel = 0;
int LeftAlignedLevel = 0;
long aligntime = 0;

long pathEndTimer = 0; // Used to detect long end and short end


void setup()
{
  servoLeft.attach(13);
  servoRight.attach(12);
  
  pinMode(10,OUTPUT);
  pinMode(11,INPUT);
  pinMode(A2,OUTPUT);
  
  pinMode(6,OUTPUT);
  pinMode(7,INPUT);
  pinMode(A1,OUTPUT);
  
  pinMode(2,OUTPUT);
  pinMode(3,INPUT);
  pinMode(A0,OUTPUT);
}

void loop()
{
    int irMiddle = irMiddleSensor(39750);
    turn(irMiddle);
}

void turn(int irMiddle)
{
    int irLeft = irLeftSensor(38000);
    int irRight = irRightSensor(38000);

    if (irMiddle != 0 && irRight != 0){
        delay(480);//480
        turningRightnoFront();

        servoRight.writeMicroseconds(1502);
        servoLeft.writeMicroseconds(1504);

        delay(400);
    } else if (irMiddle != 0){
        forward();
        align(irLeft, irRight);

    } else if (irMiddle == 0 && irRight != 0){
        turningRight();

    } else if (irMiddle == 0 && irLeft != 0){
        turningLeft();

    } else if (irMiddle == 0 && irRight == 0 && irLeft == 0){
        
        if (millis() - pathEndTimer < 1590) {
            flip();

        } else{
            stop();
        }
    }
}


void align(int irLeft, int irRight)
{

    int delayTimes[7] = {6, 12, 18, 27, 39, 51, 51};
    long frequency[7] = {40650, 41100, 41600, 42050, 42650, 43250, 44700};
    long barrierDelay[7] = {600, 560, 520, 480, 440, 400, 200};


    int ElapsedTime = 0;
    int irSensor = 0;
    int i = 0;
    bool nearCenter = true; //This is used to reflect after hard alignment in 2nd block of code

    // Using left sensor to align - by tilting a little right   
    while (irLeft == 0 && irSensor == 0 && i < 7){

        irSensor = irLeftSensor(frequency[i]);
        if (irSensor == 0){
            nearCenter = false; //Sets it to false after reaching the first alignment level


            // set elapsed time by accounting for aligned level
            // the higher the level, the more danger so barrierDelay is kept small (in above list)
            // the lower the level, we multiple barrierDelay time with current alignment level, so low level alignments dont get executed when close to the wall
            ElapsedTime = barrierDelay[i] * (LeftAlignedLevel - i + 1);

            if (ElapsedTime > 1100){ // Set max barrier time
                ElapsedTime = 1100;
            }


            if ( LeftAlignedLevel == i || (millis() - aligntime > ElapsedTime) ){
                turningRightCustom(delayTimes[i]);

                aligntime = millis();
                LeftAlignedLevel = i + 1;
            }
        }

        i++;
    }
    
    if (LeftAlignedLevel != 0 && nearCenter){

        irSensor = irLeftSensor(40000);  // At center of the road
        if (irSensor != 0){ //Reached center after alignment

            LeftAlignedLevel = 0;
        }
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////

    irSensor = 0;
    i = 0;
    nearCenter = true; //This is used to reflect after hard alignment in 2nd block of code

    // Using right sensor to align - by tilting a little left
    while (irRight == 0 && irSensor == 0 && i < 7){

        irSensor = irRightSensor(frequency[i]);
        if (irSensor == 0){
            nearCenter = false; //Sets it to false after reaching the first alignment level


            // set elapsed time by accounting for aligned level
            // the higher the level, the more danger so barrierDelay is kept small (in above list)
            // the lower the level, we multiple barrierDelay time with current alignment level, so low level alignments dont get executed when close to the wall
            ElapsedTime = barrierDelay[i] * (RightAlignedLevel - i + 1);

            if (ElapsedTime > 1100){ // Set max barrier time
                ElapsedTime = 1100;
            }


            if ( RightAlignedLevel == i || (millis() - aligntime > ElapsedTime) ){
                turningLeftCustom(delayTimes[i]);

                aligntime = millis();
                RightAlignedLevel = i + 1;
            }
        }

        i++;
    }

    if (RightAlignedLevel != 0 && nearCenter){
        irSensor = irRightSensor(40000);  // At center of the road
        if (irSensor != 0){ //Reached center after alignment

            RightAlignedLevel = 0;
        }
    }

}


void turningRightCustom(int delaytime)
{
    while (delaytime > 0){ 

        //Making sure that during alignment, if front wall is detected, we turn first
        int irMiddle = irMiddleSensor(39750);
        if (irMiddle == 0){
            turn(irMiddle);
        }

        servoRight.writeMicroseconds(1492);
        servoLeft.writeMicroseconds(1620);

        if (delaytime >= 3){
            delay(2);  //Middle Sensor takes 1 millisecond
            delaytime -= 3; 
        } else{
            delay(delaytime);
            break;
        }
    }
}

void turningLeftCustom(int delaytime)
{
    while (delaytime > 0){ 
        
        //Making sure that during alignment, if front wall is detected, we turn first
        int irMiddle = irMiddleSensor(39750);
        if (irMiddle == 0){
            turn(irMiddle);
        }

        servoRight.writeMicroseconds(1000);
        servoLeft.writeMicroseconds(1514);

        if (delaytime >= 3){
            delay(2);  //Middle Sensor takes 1 millisecond
            delaytime -= 3; 
        } else{
            delay(delaytime);
            break;
        }
    }
}



int irLeftSensor(long frequency)
{
    tone(10, frequency);
    delay(1);
    int irResult = digitalRead(11);
    noTone(10);
    return irResult;
}

int irMiddleSensor(long frequency)
{
    tone(6, frequency);
    delay(1);
    int irResult = digitalRead(7);
    noTone(6);
    return irResult;
}

int irRightSensor(long frequency)
{
    tone(2, frequency);
    delay(1);
    int irResult = digitalRead(3);
    noTone(2);
    return irResult;
}



void forward()
{
    servoRight.writeMicroseconds(1000);
    servoLeft.writeMicroseconds(1620);
}

void turningRightnoFront()
{
    servoRight.writeMicroseconds(1532);
    servoLeft.writeMicroseconds(2000);

    delay(720);

    pathEndTimer = millis();
}

void turningRight()
{
    servoRight.writeMicroseconds(1532);
    servoLeft.writeMicroseconds(2000);

    if (RecheckingMiddleSensor() == false){ //Prevent accidental turn
        return;
    }

    delay(720);

    pathEndTimer = millis();
}

void turningLeft()
{
    servoRight.writeMicroseconds(1000);
    servoLeft.writeMicroseconds(1474);

    if (RecheckingMiddleSensor() == false){ //Prevent accidental turn
        return;
    }
    
    delay(720);

    pathEndTimer = millis();
}

void flip()
{
    // flipping by turning left
    servoRight.writeMicroseconds(1000);
    servoLeft.writeMicroseconds(1478);

    if (RecheckingMiddleSensor() == false){ //Prevent accidental turn
        return;
    }
    
    delay(500 - 4);
    servoLeft.writeMicroseconds(1350);
    delay(725);


    pathEndTimer = millis();
}

void stop()
{	
    servoRight.writeMicroseconds(1502);
    servoLeft.writeMicroseconds(1504);

    if (RecheckingMiddleSensor() == false){ //Prevent accidental stop
        return;
    }
    delay(20);
    if (RecheckingMiddleSensor() == false){ //Double Check: Prevent accidental stop
        return;
    }

    while (true){ //Endless looping to halt the movement
        delay(100);
    }
}


bool RecheckingMiddleSensor(){
    delay(3);
    if (irMiddleSensor(39750) != 0){
        servoRight.writeMicroseconds(1000);
        servoLeft.writeMicroseconds(1620);
        return false;
    }
    return true;
}