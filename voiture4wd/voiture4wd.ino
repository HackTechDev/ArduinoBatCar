/*
   BatCar
   

 */

/*

   Matériels : 

   - Arduino Uno : http://www.gotronic.fr/art-carte-arduino-uno-12420.htm
   - Module Grove Base Shield SLD12148P : http://www.gotronic.fr/art-module-grove-base-shield-sld12148p-19068.htm
   - Controleur de moteur Pont H L298N : http://mchobby.be/wiki/index.php?title=Pont-H_L298N
   - Récepteur infrarouge Grove :http://www.gotronic.fr/art-module-recepteur-ir-grove-wls12136p-18952.htm
   - Afficheur LCD série Grove LCD23154P : http://www.gotronic.fr/art-afficheur-lcd-serie-grove-lcd23154p-19018.htm
   - Grove servo moteur : http://www.seeedstudio.com/wiki/Grove_-_Starter_Kit_Plus#15._Grove_.E2.80.93_Servo
   - Grove sound recorder : http://www.seeedstudio.com/wiki/index.php?title=Twig_-_Sound_Recorder_v0.92b
   - Grove bluetooth : http://www.seeedstudio.com/wiki/Grove_-_Serial_Bluetooth
   
Batteries : 

- Arduino Uno : Pile rectangulaire 9v
- Carte de pilotage de motur L298N : 6x1,5v 2500 mha

Configuration bluetooth :
- http://steamcyberpunk.net/2013/02/24/configuration-du-bluetooth/

 */

/*
   /!\ A voir : Grove vocal
 */
 
#include <IRSendRev.h>

#include <SerialLCD.h>
#include <SoftwareSerial.h>

#include <Servo.h>

Servo groveServo; //create a object
Servo groveServoTete;
Servo groveServoQueue;
Servo groveServoYeux;

int potentiometer = 0;
int shaft;

// Bluetooth
#define RxD 7
#define TxD 8
SoftwareSerial blueToothSerial(RxD,TxD);

// initialize the library
SerialLCD slcd(11,12);//this is a must, assign soft serial pins


//-- Moteur Gauche--
int ENA=5; //Connecté à Arduino pin 5(sortie pwm)
int IN1=2; //Connecté à Arduino pin 2
int IN2=3; //Connecté à Arduino pin 3

//-- Moteur Droit --
int IN3=4; //Connecté à Arduino pin 4
int IN4=7; //Connecté à Arduino pin 7
int ENB=6; //Connecté à Arduino pin 6(Sortie pwm)

unsigned char dta[20];

void arreterMoteurs() {
    Serial.println("arreterMoteurs");
    analogWrite(ENA,LOW);
    analogWrite(ENB,LOW);
}

void demarrerMoteurs(int puissance) {
    Serial.println("demarrerMoteurs");  
    analogWrite(ENA, puissance);
    analogWrite(ENB, puissance);
}

void reculer(int duree, int puissance) {
    Serial.println("reculer");

    digitalWrite(IN1,LOW); 
    digitalWrite(IN2,HIGH);

    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    demarrerMoteurs(puissance); 
    delay(duree);
    arreterMoteurs(); 

}

void avancer(int duree, int puissance) {
    Serial.println("avancer");

    digitalWrite(IN1,HIGH); 
    digitalWrite(IN2,LOW);

    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);

    demarrerMoteurs(puissance); 
    delay(duree);
    arreterMoteurs();
}

void tournerDroite(int duree, int puissance) {
    Serial.println("tournerDroite");

    digitalWrite(IN1,LOW); 
    digitalWrite(IN2,HIGH);

    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    demarrerMoteurs(puissance);  
    delay(duree);
    arreterMoteurs();
}

void tournerGauche(int duree, int puissance) {
    Serial.println("tournerGauche"); 
    digitalWrite(IN1,HIGH); 
    digitalWrite(IN2,LOW);

    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    demarrerMoteurs(puissance);  
    delay(duree);
    arreterMoteurs();
}

void setup() {

    Serial.begin(38400);

    IR.Init(A4);
    Serial.println("Configuration"); 

    // Audio
    pinMode(1, OUTPUT); 
    pinMode(2, OUTPUT); 
    digitalWrite(1, LOW );
    digitalWrite(2, LOW );

    // Servo moteur
    groveServo.attach(3); //the servo is attached to D3
    groveServo.write(50); // Avant

    groveServoTete.attach(4); //the servo is attached to D4
    groveServoTete.write(2); // Droit Devant

    groveServoQueue.attach(5); //the servo is attached to D5
    groveServoQueue.write(100); // Queue

    groveServoYeux.attach(6); //the servo is attached to D6
    groveServoYeux.write(100); // Yeux

    // set up
    slcd.begin();
    // Print a message to the LCD.
    slcd.print("Config BatCar");

    pinMode(ENA,OUTPUT);//Configurer les broches comme sortie
    pinMode(ENB,OUTPUT);
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);

    arreterMoteurs();


 pinMode(RxD, INPUT);
 pinMode(TxD, OUTPUT);
 setupBlueToothConnection(); 

}


void setupBlueToothConnection()
{
 blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
 blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
 blueToothSerial.print("\r\n+STNA=Batcar\r\n"); //set the bluetooth name as "SeeedBTSlave"
 blueToothSerial.print("\r\n+STPIN=6666\r\n");//Set SLAVE pincode"0000"
 blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
 blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
 delay(2000); // This delay is required.
 blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
 Serial.println("The slave bluetooth is inquirable!");
 delay(2000); // This delay is required.
 blueToothSerial.flush();
}   

void loop() {
 char recvChar;
 
    while(1) { 
     
          if (blueToothSerial.available()) {
             recvChar = blueToothSerial.read();
             switch (recvChar) {
                 case 'b' : {
                     blueToothSerial.print("b");
                     slcd.setCursor(0, 1);
                     slcd.print("Bluetooth");
                     Serial.println("Bluetooth");
                     break;
                     }
             }
         
         }
      
      
        if(IR.IsDta()){

            int length= IR.Recv(dta);
            Serial.println(dta[9]);

            // Bouton 0    
            if(dta[9] == 207) {
                Serial.println("Fonction0");   
                slcd.setCursor(0, 1);
                slcd.print("Fonction0");
            }
            
            // Bouton 1    
            if(dta[9] == 247) {
                Serial.println("Tete Gauche");   
                slcd.setCursor(0, 1);
                slcd.print("Tete Gauche");

                groveServo.write(20); //Gauche

                delay(150);

            }


            // Bouton 2    
            if(dta[9] == 119) {
                Serial.println("Tete Avant");   
                slcd.setCursor(0, 1);
                slcd.print("Tete Avant");

                groveServo.write(50); // Avant

                delay(150);

            }

            // Bouton 3
            if(dta[9] == 183) {
                Serial.println("Tete Droite");    
                slcd.setCursor(0, 1);
                slcd.print("Tete Droite");

                groveServo.write(80);   
                delay(150);
            }



            // Bouton 4    
            if(dta[9] == 215) {
                Serial.println("Tete Gauche");   
                slcd.setCursor(0, 1);
                slcd.print("Tete Gauche");

                groveServo.write(0); //Gauche

                delay(150);

            }

            // Bouton 6
            if(dta[9] == 151) {
                Serial.println("Tete Droite");    
                slcd.setCursor(0, 1);
                slcd.print("Tete Droite");

                groveServo.write(90);   
                delay(150);
            }


            /*
               Tete
             */

            // Bouton 7 : Relever Tete       
            if(dta[9] == 231) {
                Serial.println("Relever Tete");   
                slcd.setCursor(0, 1);
                slcd.print("Relever Tete ");

                groveServoTete.write(2); 

                delay(150);

            }

            // Bouton 8 : Baisser Tete   
            if(dta[9] == 103) {
                Serial.println("Baisser Tete");    
                slcd.setCursor(0, 1);
                slcd.print("Baisser Tete");

                groveServoTete.write(45);   
                delay(150);
            }

            // Bouton 9 : Baisser Tete   
            if(dta[9] == 167) {
                Serial.println("Baisser Tete");    
                slcd.setCursor(0, 1);
                slcd.print("Baisser Tete");

                groveServoTete.write(90);   
                delay(150);
            }

            /*
               Queue
             */

            // Bouton EQ : Queue Horizontale       
            if(dta[9] == 79) {
                Serial.println("Queue Horizontale");   
                slcd.setCursor(0, 1);
                slcd.print("Queue Horizontale");

                groveServoQueue.write(100); 

                delay(150);

            }

            // Bouton ST : Queue Verticale 
            if(dta[9] == 143) {
                Serial.println("Queue Verticalee");    
                slcd.setCursor(0, 1);
                slcd.print("Queue Verticale");

                groveServoQueue.write(15);   
                delay(150);
            }

            /*
               Yeux
             */

            // Bouton > : Yeux Droit       
            if(dta[9] == 175) {
                Serial.println("Yeux Droit");   
                slcd.setCursor(0, 1);
                slcd.print("Yeux Droit");

                groveServoYeux.write(90); 

                delay(150);

            }

            // Bouton < : Yeux Gauche 
            if(dta[9] == 239) {
                Serial.println("Yeux Gauche");    
                slcd.setCursor(0, 1);
                slcd.print("Yeux Gauche");

                groveServoYeux.write(0);   
                delay(150);
            }

            /*
               Deplacement 
             */

            if(dta[9] == 127) {
                Serial.println("Avancer");   
                slcd.setCursor(0, 1);
                slcd.print("Avancer");
                avancer(1000, 255);     
            }

            if(dta[9] == 111) {
                Serial.println("Reculer");    
                slcd.setCursor(0, 1);
                slcd.print("Reculer");
                reculer(1000, 255);
            }

            if(dta[9] == 159) {
                Serial.println("Tourner Droite");
                slcd.setCursor(0, 1);
                slcd.print("Tourner Droite");    
                tournerDroite(1000, 255);
            }

            if(dta[9] == 223) {
                Serial.println("Tourner Gauche");
                slcd.setCursor(0, 1);
                slcd.print("Tourner Gauche");    
                tournerGauche(1000, 255);
            }
            Serial.println();
        }
    }
}

