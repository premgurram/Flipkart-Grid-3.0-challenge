//make sure you install RF24 in arduino to get this compiled

/*construction tips
require 3 motors
one communication module
bottom corner is ground and adjacent one is voltage source
communication connections
7 wires are required  and capactor if not working
refer the video from udemy 1300 in the udemy course
*/
/*protocols for reference
0 for stop
1 for run forward
2 for run backward
3 for turn right
4 for turn left
5 for over turn
6 for lift up
7 for lift down
8 for finish its job permanent stop
*/

// modify your values only in this section 
//VALUE SECTION BEGIN
int CHANNEL = 100;
int ROBOT = 1; // robot number ranges from 1 to 5
int DELAY = 50; // stop delay for the computer to recognise that the robot is at rest.
// assign port values to the following 6 
int LIFT_MOTOR_IN_1;
int LIFT_MOTOR_IN_2;
int LEFT_WHEEL_IN_1;
int LEFT_WHEEL_IN_2;
int RIGHT_WHEEL_IN_1;
int RIGHT_WHEEL_IN_2;

//VALUE SECTION END

//Dont touch anything below this line unless you need to change directions 
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#define MISO 12
#define IRQ 2
#define SCK 13
#define MOSI 11
#define CSN 10
#define CE 9
RF24 radio(CE, CSN);



const int pipes[1] = {ROBOT}; 
struct Data //we send 2 values which tells the bot when to stop after execution of the code
{
    int a;
    int delay;
} data;

void setup()
{
    radio.begin();
    radio.setChannel(CHANNEL);
    radio.setPALevel(RF24_PA_HIGH);  // adjust this value if necessary
    radio.setDataRate(RF24_250KBPS); // adjust this value if necessary
    radio.openReadingPipe(1, pipes[0]);
    radio.startListening();
}

void turn(int dir, int dly)
{
    dir = dir % 2; // change to the below line if the turning is going opposite to what we are getting
    //dir = (dir + 1) % 2; 
    digitalWrite(dir ? LEFT_WHEEL_IN_1 : LEFT_WHEEL_IN_2, HIGH);
    digitalWrite(dir ? LEFT_WHEEL_IN_2 : LEFT_WHEEL_IN_1, LOW);
    digitalWrite(dir ? RIGHT_WHEEL_IN_1 : RIGHT_WHEEL_IN_2, LOW);
    digitalWrite(dir ? RIGHT_WHEEL_IN_2 : RIGHT_WHEEL_IN_1, HIGH);
    delay(dly);
}
void turn_around(int dly)
{
    turn(1, dly);
    turn(1, dly);
}
void run(int clk, int len)
{   //clk is 1 for clockwise 
    clk = clk % 2; // change to the below line if it goes anti clockwise instead of clockwise
    //clk = (clk + 1) % 2;
    digitalWrite(LEFT_WHEEL_IN_2, clk ? LOW : HIGH);
    digitalWrite(LEFT_WHEEL_IN_1, clk ? HIGH : LOW);
    //analogWrite(ena, 255);

    digitalWrite(RIGHT_WHEEL_IN_2, clk ? LOW : HIGH);
    digitalWrite(RIGHT_WHEEL_IN_1, clk ? HIGH : LOW);
    //analogWrite(enb, 255);
    delay(len);
}
void stop(int dly)
{
    digitalWrite(LEFT_WHEEL_IN_1, LOW);
    digitalWrite(LEFT_WHEEL_IN_2, LOW);
    digitalWrite(RIGHT_WHEEL_IN_1, LOW);
    digitalWrite(RIGHT_WHEEL_IN_2, LOW);
    delay(dly);
}
void lift(int dir, int dly)
{
    dir = dir % 2; // change to the below line if the turning is going opposite to what we are getting
    //dir = (dir + 1) % 2;
    digitalWrite(dir ? LIFT_MOTOR_IN_1 : LIFT_MOTOR_IN_2, HIGH);
    digitalWrite(dir ? LIFT_MOTOR_IN_2 : LIFT_MOTOR_IN_1, LOW);
    delay(dly);
}
void finish()
{
    radio.stopListening();
    while(true)
    {
        stop(100);
    }
}
void loop()
{
    if (radio.available())
    {
        radio.read(&data, sizeof(Data));
        switch (data.a)
        {
        case 0:
            stop(data.delay);
            break;
        case 1:
            run(data.a, data.delay);
            break;
        case 2:
            run(data.a, data.delay);
            break;
        case 3:
            turn(data.a, data.delay);
            break;
        case 4:
            turn(data.a, data.delay);
            break;
        case 5:
            turn_around(data.delay);
            break;
        case 6:
            lift(data.a, data.delay);
            break;
        case 7:
            lift(data.a, data.delay);
            break;
        case 8:
            finish();
            break;
        }
        if (data.a != 1 && data.a != 2)
        {
            delay(DELAY);
        }
    }
}
