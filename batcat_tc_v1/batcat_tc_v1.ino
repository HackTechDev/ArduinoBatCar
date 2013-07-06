/*
   Télécommande pour BatCar
   

 */

/*

   Matériels : 

   - Arduino Uno : http://www.gotronic.fr/art-carte-arduino-uno-12420.htm
   - Module Grove Base Shield SLD12148P : http://www.gotronic.fr/art-module-grove-base-shield-sld12148p-19068.htm
 
   - Grove servo moteur : http://www.seeedstudio.com/wiki/Grove_-_Starter_Kit_Plus#15._Grove_.E2.80.93_Servo
   - Grove bluetooth : http://www.seeedstudio.com/wiki/Grove_-_Serial_Bluetooth
   
Batteries : 

- Arduino Uno : Pile rectangulaire 9v

Configuration bluetooth :
- http://steamcyberpunk.net/2013/02/24/configuration-du-bluetooth/

 */

#include <SoftwareSerial.h>
#include <Servo.h>
#include <IRSendRev.h>

// ServoMoteurs

Servo groveServoCou; //create a object
Servo groveServoTete;
Servo groveServoQueue;
Servo groveServoYeux;

// Infrarouge

//the first parameter(15): the data that needs to be sent;
//the next 2 parameter(70,70): the logic high and low duration of "Start";
//the next 2 parameter(20,60): the logic "short" and "long"duration in the communication
//                             that to say:  if "0", the high duration is 20ms and  low is 20 ms; while logic "1",
//                              the high duration is 20 ms and low is 60 ms;
//the next 2 parameter(10): number of data you will sent;
//the next parameter(1, 2, 3, 4,5,6,7,8,9,10): data you will sent ;

unsigned char d[] = {15, 70, 70, 20, 60, 10, 1, 2, 3, 4,5,6,7,8,9,10};

// Bluetooth
#define RxD 7
#define TxD 8
SoftwareSerial blueToothSerial(RxD,TxD);


void setup() {

    Serial.begin(38400);

    Serial.println("Configuration"); 

    // Servo moteur

    groveServoCou.attach(2); //the servo is attached to D2
    groveServoCou.write(50); // Cou


    groveServoTete.attach(4); //the servo is attached to D4
    groveServoTete.write(2); // Droit Devant

    groveServoYeux.attach(6); //the servo is attached to D6
    groveServoYeux.write(100); // Yeux

    groveServoQueue.attach(5); //the servo is attached to D5
    groveServoQueue.write(100); // Queue

    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    setupBlueToothConnection(); 

}


void setupBlueToothConnection() {
 blueToothSerial.begin(38400); 
 blueToothSerial.print("\r\n+STWMOD=0\r\n"); 
 blueToothSerial.print("\r\n+STNA=Batcar\r\n");
 blueToothSerial.print("\r\n+STPIN=6666\r\n");
 blueToothSerial.print("\r\n+STOAUT=1\r\n"); 
 blueToothSerial.print("\r\n+STAUTO=0\r\n"); 
 delay(2000);
 blueToothSerial.print("\r\n+INQ=1\r\n"); 
 Serial.println("The slave bluetooth is inquirable!");
 delay(2000); 
 blueToothSerial.flush();
}   

void loop() {
 char recvChar;
 
    while(1) { 
     
          if (blueToothSerial.available()) {
             recvChar = blueToothSerial.read();
             switch (recvChar) {

                 case 'z' : {
                     blueToothSerial.println("Avancer Droit");
                     // 4
                     unsigned char d[] = {15, 70, 70, 20, 60, 10, 1, 2, 3, 4,5,6,7,8,9,10};
                     IR.Send(d, 38);                    
                     Serial.println("Avancer Droit");                                          
                     break;
                     }

                 case 's' : {
                     blueToothSerial.println("Reculer Droit");                    
                     // 5
                     unsigned char d[] = {15, 70, 70, 20, 60, 10, 1, 2, 3, 5,5,6,7,8,9,10};
                     IR.Send(d, 38);                    
                     Serial.println("Reculer Droit");
                     break;
                     }

                 case 'q' : {
                     blueToothSerial.println("Tourner Gauche");
                      // 6
                     unsigned char d[] = {15, 70, 70, 20, 60, 10, 1, 2, 3, 6,5,6,7,8,9,10};
                     IR.Send(d, 38);                                        
                     Serial.println("Tourner Gauche");
                     break;
                     }

                 case 'd' : {
                     blueToothSerial.println("Tourner Droite");
                      // 7
                     unsigned char d[] = {15, 70, 70, 20, 60, 10, 1, 2, 3, 7,5,6,7,8,9,10};
                     IR.Send(d, 38);                                        
                     Serial.println("Tourner Droite");
                     break;
                     }

               case 'b' : {
                     blueToothSerial.println("Bluetooth");    
                     // 8
                     unsigned char d[] = {15, 70, 70, 20, 60, 10, 1, 2, 3, 8,5,6,7,8,9,10};
                     IR.Send(d, 38);                    
                     
                     Serial.println("Bluetooth");
                     break;
                     }
                     
// Cou

                 case '1' : {
                     blueToothSerial.println("Cou Gauche");  
                     groveServoCou.write(10);                  
                     Serial.println("Cou Gauche");
                     break;
                     }

                 case '2' : {
                     blueToothSerial.println("Cou Milieu");
                     groveServoCou.write(50);                    
                     Serial.println("Cou Milieu");
                     break;
                     }

               case '3' : {
                     blueToothSerial.println("Cou Droit");
                     groveServoCou.write(90);                    
                     Serial.println("Cou Droit");
                     break;
                     }

// Tete

                 case '4' : {
                     blueToothSerial.println("Tete Devant");  
                     groveServoTete.write(2);                  
                     Serial.println("Tete Devant");
                     break;
                     }

                 case '5' : {
                     blueToothSerial.println("Tete Baisser");
                     groveServoTete.write(50);                    
                     Serial.println("Tete Baisser");
                     break;
                     }

               case '6' : {
                     blueToothSerial.println("Tete Sol");
                     groveServoTete.write(90);                    
                     Serial.println("Tete Sol");
                     break;
                     }


// Yeux

                 case '7' : {
                     blueToothSerial.println("Yeux Gauche");  
                     groveServoYeux.write(10);                  
                     Serial.println("Yeux Gauche");
                     break;
                     }

                 case '8' : {
                     blueToothSerial.println("Yeux Devant");
                     groveServoYeux.write(100);                    
                     Serial.println("Yeux Devant");
                     break;
                     }

               case '9' : {
                     blueToothSerial.println("Yeux Droit");
                     groveServoYeux.write(90);                    
                     Serial.println("Yeux Droit");
                     break;
                     }
                     
// Queue

                 case 'y' : {
                     blueToothSerial.println("Queue Devant");  
                     groveServoQueue.write(2);                  
                     Serial.println("Queue Devant");
                     break;
                     }

                 case 'h' : {
                     blueToothSerial.println("Queue Baisser");
                     groveServoQueue.write(50);                    
                     Serial.println("Queue Baisser");
                     break;
                     }

               case 'n' : {
                     blueToothSerial.println("Queue Sol");
                     groveServoQueue.write(90);                    
                     Serial.println("Queue Sol");
                     break;
                     }                     
             }
          
         }
      
    }  

}

