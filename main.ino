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

// 3s avant de passer à la 2e vitesse
#define timeScdSpeed 3000

// 3s avant de passer à la 3e vitesse
#define timeThdSpeed 3000

int main(){
  
  init(); // on initialise les trucs d'arduino pour que pinmode tt ça fonctionne
  
  // pinMode du moteur:
  pinMode(motordriver, OUTPUT);
  pinMode(positive, OUTPUT);
  pinMode(negative, OUTPUT);
  
  digitalWrite(motordriver, HIGH);
  
  // pinMode des boutons:
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  
  // Variables pour le maintien de la pression
  int lastState = 0; // 0 = null / 1 = up / 2 = down
  unsigned long startedTime = 0;
  int currentSpeed = 0;
  
  while (true){
    // On récupère l'état des boutons et on regarde si appuyé:
    if(digitalRead(up) && digitalRead(down)){ // Si les 2 boutons sont appuyés

      // On coupe le moteur
      motorSpeed(0,0);
      changeState(0, &lastState, &startedTime, &currentSpeed);

    } else if (digitalRead(up)){ // Si seulement le bouton pour monter est appuyé
	  
      if(lastState != 1) changeState(1, &lastState, &startedTime, &currentSpeed);

      // On gère le maintien
      maintien(&currentSpeed, startedTime);

      // On envoie le courant à la vitesse adéquate
      motorSpeed(currentSpeed, 0);

    } else if (digitalRead(down) == 1) { // Si seulement le bouton descendre est appuyé

      if(lastState != 2) changeState(2, &lastState, &startedTime, &currentSpeed);

      // On gère le maintien
      maintien(&currentSpeed, startedTime);

      // On envoie le courant à la vitesse adéquate en "inversant" le courant
      motorSpeed(0, currentSpeed);

    } else { // Si on appuie pas, on coupe le moteur ET on dit qu'on appuie pas
      motorSpeed(0, 0);
      changeState(0, &lastState, &startedTime, &currentSpeed);
    } 
    delay(10);
  }
}  

void maintien(int* currentSpeed, int startedTime){
  if(millis() - startedTime >= timeScdSpeed && *currentSpeed == firstSpeed){
       *currentSpeed = secondSpeed; // on dit qu'on est à la 2nde vitesse
    } else if(millis() - startedTime >= (timeThdSpeed+timeScdSpeed) && *currentSpeed == secondSpeed){
       *currentSpeed = thirdSpeed; // on dit qu'on est à la 3e vitesse
    }
}

void motorSpeed(int plus, int moins){
  analogWrite(positive, plus);
  analogWrite(negative, moins);
}  

void changeState(int state, int* lastState, unsigned long* startedTime, int* currentSpeed){
  *lastState = state;
  *startedTime = millis();
  *currentSpeed = firstSpeed;
}  
