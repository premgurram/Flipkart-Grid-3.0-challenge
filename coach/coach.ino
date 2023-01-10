/*
    we use serial to note the status of the game.
*/
int CHANNEL = 100;
int DELAY; // if applicable we use this

#include <SPI.h>
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
const int pipes[4] = {1, 2, 3, 4};

struct Data
{
    int a;
    int delay;
} data;

struct Instruction
{
    int robot;
    int call;
    int delay;
} packet;

int ROBOT = 1;
void setup()
{
    Serial.begin(9600);
    delay(100);
    radio.begin();
    radio.setChannel(CHANNEL);       //set communication channel
    radio.setPALevel(RF24_PA_MAX);   //set power amplifier level to max
    radio.setDataRate(RF24_250KBPS); //set data rate
    radio.openWritingPipe(ROBOT);    //initialize the radio object with the pipe address
    radio.stopListening();           //sets module as a transmitter
    delay(100);                      //wait a little
}
void loop()
{
    while (Serial.available() > 0)
    {
        packet.robot = Serial.parseInt();

        // do it again:

        packet.call = Serial.parseInt();

        // do it again:

        packet.delay = Serial.parseInt();

        // look for the newline. That's the end of your sentence:

        if (Serial.read() == '\n')
        {
            if (packet.robot == 0)
            {
                Serial.println("Relay completed");
                while (true)
                {
                    delay(100);
                }
            }

            if (ROBOT != packet.robot)
            {
                next_robot();
            }

            data.a = packet.call;
            data.delay = packet.delay;
            radio.write(&data, sizeof(Data)); //send the data
            //delay(DELAY);                     // adjust the delay accordingly
            Serial.print(data.a);
            Serial.print("-");
            switch (data.a)
            {
            case 0:
                Serial.println("Robot stops");
                break;
            case 1:
                Serial.println("Robot moves forward");
                break;
            case 2:
                Serial.println("Robot moves backward");
                break;
            case 3:
                Serial.println("Robot turns to right");
                break;
            case 4:
                Serial.println("Robot turns to left");
                break;
            case 5:
                Serial.println("Robot returns");
                break;
            case 6:
                Serial.println("Robot Dropping the object");
                break;
            case 7:
                Serial.println("Robot coming back to original state after dropping");
                break;
            case 8:
                Serial.println("Robot finished its race");
                break;
            }
        }
    }
}
void next_robot()
{
    radio.openWritingPipe(++ROBOT);
    radio.stopListening();
    Serial.println("Next Robot starts its race");
    delay(100);
}
