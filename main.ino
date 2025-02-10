/*

   ▄▄▄▄▄    ▄ ▄   ▄███▄   █▄▄▄▄ █ ▄▄  ▄███▄   
  █     ▀▄ █   █  █▀   ▀  █  ▄▀ █   █ █▀   ▀  
▄  ▀▀▀▀▄  █ ▄   █ ██▄▄    █▀▀▌  █▀▀▀  ██▄▄    
 ▀▄▄▄▄▀   █  █  █ █▄   ▄▀ █  █  █     █▄   ▄▀ 
           █ █ █  ▀███▀     █    █    ▀███▀   
            ▀ ▀            ▀      ▀           
		  https://github.com/swerpe
         
*/


#define motordriver 13
#define positive 11
#define negative 9

#define up 2
#define down 3

#define firstSpeed 80
#define secondSpeed 160
#define thirdSpeed 250

// Variables pour le maintien de la pression
unsigned long startedTime = 0;
int currentSpeed = 0;
int lastState = 0; // 0 = null / 1 = up / 2 = down
int timeScdSpeed = 3000; // 3s avant de passer à la 2e vitesse
int timeThdSpeed = 3000; // 3s avant de passer à la 3e vitesse

// Variable pour les petites pressions
unsigned long lastPressionUP = 0;
unsigned long lastPressionDOWN = 0;

void setup(){
  
  // pinMode du moteur:
  pinMode(motordriver, OUTPUT);
  pinMode(positive, OUTPUT);
  pinMode(negative, OUTPUT);
  
  digitalWrite(motordriver, HIGH);
  
  // pinMode des boutons:
  pinMode(up, INPUT);
  pinMode(down, INPUT);

}

void loop(){
  // On récupère l'état des boutons et on regarde si appuyé:
  if(digitalRead(up) && digitalRead(down)){ // Si les 2 boutons sont appuyés
    
    // On coupe le moteur
    motorSpeed(0,0);
    changeState(0);
    
  } else if (digitalRead(up)){ // Si seulement le bouton pour monter est appuyé
    
    if(lastState != 1) changeState(1);
    
    // On gère le maintien
    maintien();
    
    // On envoie le courant à la vitesse adéquate
	motorSpeed(currentSpeed, 0);
    
  } else if (digitalRead(down) == 1) { // Si seulement le bouton descendre est appuyé
    
    if(lastState != 2) changeState(2);
    
    // On gère le maintien
    maintien();
    
    // On envoie le courant à la vitesse adéquate en "inversant" le courant
	motorSpeed(0, currentSpeed);
    
  } else { // Si on appuie pas, on coupe le moteur ET on dit qu'on appuie pas
    motorSpeed(0, 0);
    changeState(0);
  } 
  delay(10);
}  

void maintien(){
  if(millis() - startedTime >= timeScdSpeed && currentSpeed == firstSpeed){
       currentSpeed = secondSpeed; // on dit qu'on est à la 2nde vitesse
    } else if(millis() - startedTime >= (timeThdSpeed+timeScdSpeed) && currentSpeed == secondSpeed){
       currentSpeed = thirdSpeed; // on dit qu'on est à la 3e vitesse
    }
}

void motorSpeed(int plus, int moins){
  analogWrite(positive, plus);
  analogWrite(negative, moins);
}  

void changeState(int state){
  lastState = state;
  startedTime = millis();
  currentSpeed = firstSpeed;
}  
