/*
   BatCar
   Déplacement du véhicule   

 */

/*

   Matériels : 

   - Arduino Uno : http://www.gotronic.fr/art-carte-arduino-uno-12420.htm
   - Module Grove Base Shield SLD12148P : http://www.gotronic.fr/art-module-grove-base-shield-sld12148p-19068.htm
   - Controleur de moteur Pont H L298N : http://mchobby.be/wiki/index.php?title=Pont-H_L298N
   - Récepteur infrarouge Grove :http://www.gotronic.fr/art-module-recepteur-ir-grove-wls12136p-18952.htm
  
Batteries : 

- Carte de pilotage de motor L298N : 6x1,5v 2500 mha (Pile Auchan)


 */

#include <IRSendRev.h>

#include <SerialLCD.h>
#include <SoftwareSerial.h>

// initialize the library
SerialLCD slcd(11,12);//this is a must, assign soft serial pins

// Moteur Droit

int IN1=2; //Connecté à Arduino pin 2
int IN2=3; //Connecté à Arduino pin 3
int ENA=5; //Connecté à Arduino pin 5(sortie pwm)

// Moteur Gauche
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

    // Moteur Droit
    digitalWrite(IN1,LOW); 
    digitalWrite(IN2,HIGH);

    // Moteur Gauche
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);

    demarrerMoteurs(puissance);  
    delay(duree);
    arreterMoteurs();
}

void tournerGauche(int duree, int puissance) {
    Serial.println("tournerGauche"); 

    // Moteur Droit
    digitalWrite(IN1,HIGH); 
    digitalWrite(IN2,LOW);

    // Moteur Gauche
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

    
    // set up
    slcd.begin();
    // Print a message to the LCD.
    slcd.print("Config BatCar");

    pinMode(ENA,OUTPUT);
    pinMode(ENB,OUTPUT);
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);

    arreterMoteurs();  
    


 
}


void loop() {
 char recvChar;
 
    while(1) { 
     


      
        if(IR.IsDta()){

            int length= IR.Recv(dta);
            Serial.println(dta[9]);


            /*
               Deplacement 
             */

            if(dta[9] == 127 || dta[9] == 4) {
                Serial.println("Avancer");   
                slcd.setCursor(0, 1);
                slcd.print("Avancer");
                avancer(1000, 255);     
            }

            if(dta[9] == 111 || dta[9] == 5) {
                Serial.println("Reculer");    
                slcd.setCursor(0, 1);
                slcd.print("Reculer");
                reculer(1000, 255);
            }

            if(dta[9] == 159  || dta[9] == 7) {
                Serial.println("Tourner Droite");
                slcd.setCursor(0, 1);
                slcd.print("Tourner Droite");    
                tournerDroite(1000, 255);
            }

            if(dta[9] == 223  || dta[9] == 6) {
                Serial.println("Tourner Gauche");
                slcd.setCursor(0, 1);
                slcd.print("Tourner Gauche");    
                tournerGauche(1000, 255);
            }

        }
    }
}

