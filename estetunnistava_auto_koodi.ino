#include <Servo.h>
#include <NewPing.h>

#define SERVO_PIN 3
#define ULTRA_AANI_ANTURI_TRIG 11
#define ULTRA_AANI_ANTURI_ECHO 12
#define MAKS_NORMAALI_MOOTTORI_NOPEUS 150
#define MAKS_MOOTTORI_SAATO_NOPEUS 200
#define TARKISTETTAVA_ETAISYYS 23

// Oikea moottori
const int oikeaMoottoriEnable = 5;
const int oikeaMoottoriPin1 = 7;
const int oikeaMoottoriPin2 = 8;

// Vasen moottori
const int vasenMoottoriEnable = 6;
const int vasenMoottoriPin1 = 9;
const int vasenMoottoriPin2 = 10;

NewPing ultraAaniAnturi(ULTRA_AANI_ANTURI_TRIG, ULTRA_AANI_ANTURI_ECHO, 400);
Servo minunServo;

void setup()
{
  pinMode(oikeaMoottoriEnable, OUTPUT);
  pinMode(oikeaMoottoriPin1, OUTPUT);
  pinMode(oikeaMoottoriPin2, OUTPUT);

  pinMode(vasenMoottoriEnable, OUTPUT);
  pinMode(vasenMoottoriPin1, OUTPUT);
  pinMode(vasenMoottoriPin2, OUTPUT);

  minunServo.attach(SERVO_PIN);
  minunServo.write(90);
  pyoritaMoottoria(0, 0);
}

void loop()
{

  int etaisyys = ultraAaniAnturi.ping_cm();

  // Jos etäisyys on 30 cm sisällä, säädä moottorin suuntaa
  if (etaisyys > 0 && etaisyys < TARKISTETTAVA_ETAISYYS)
  {
    // Pysäytä moottorit
    pyoritaMoottoria(0, 0);
    delay(500);  

    // Käännä moottoreita taaksepäin
    pyoritaMoottoria(-MAKS_MOOTTORI_SAATO_NOPEUS, -MAKS_MOOTTORI_SAATO_NOPEUS);        
    delay(300);

    // Pysäytä moottorit
    pyoritaMoottoria(0, 0);
    delay(500);

    // Käännä servoa vasemmalle    
    minunServo.write(180);
    delay(500);

    // Lue vasemman puolen etäisyys ultraäänianturilla
    int vasenEtaisyys = ultraAaniAnturi.ping_cm();    

    // Käännä servoa oikealle
    minunServo.write(0);    
    delay(500);    

    // Lue oikean puolen etäisyys ultraäänianturilla   
    int oikeaEtaisyys = ultraAaniAnturi.ping_cm();

    // Palauta servo keskelle
    minunServo.write(90); 
    delay(500);        

    // Vertaa vasemman ja oikean puolen etäisyyksiä ja päätä kumpaan suuntaan kääntyä
    if (vasenEtaisyys == 0 )
    {
      pyoritaMoottoria(MAKS_MOOTTORI_SAATO_NOPEUS, -MAKS_MOOTTORI_SAATO_NOPEUS);
      delay(600);
    }
    else if (oikeaEtaisyys == 0 )
    {
      pyoritaMoottoria(-MAKS_MOOTTORI_SAATO_NOPEUS, MAKS_MOOTTORI_SAATO_NOPEUS);
      delay(600);
    }
    else if (vasenEtaisyys >= oikeaEtaisyys)
    {
      pyoritaMoottoria(MAKS_MOOTTORI_SAATO_NOPEUS, -MAKS_MOOTTORI_SAATO_NOPEUS);
      delay(600);
    }
    else
    {
      pyoritaMoottoria(-MAKS_MOOTTORI_SAATO_NOPEUS, MAKS_MOOTTORI_SAATO_NOPEUS);
      delay(600);      
    }
    // Pysäytä moottorit ennen uuden silmukan alkua
    pyoritaMoottoria(0, 0);    
    delay(600);     
  }
  // Jos ei ole esteitä, aja eteenpäin normaalilla nopeudella
  else
  {
    pyoritaMoottoria(MAKS_NORMAALI_MOOTTORI_NOPEUS, MAKS_NORMAALI_MOOTTORI_NOPEUS);
  }
}

// Aliohjelma moottorin pyörittämiseen
void pyoritaMoottoria(int oikeaMoottoriNopeus, int vasenMoottoriNopeus)
{
  // Aseta oikean moottorin pyörimissuunta
  if (oikeaMoottoriNopeus < 0)
  {
    digitalWrite(oikeaMoottoriPin1, LOW);
    digitalWrite(oikeaMoottoriPin2, HIGH);    
  }
  else if (oikeaMoottoriNopeus >= 0)
  {
    digitalWrite(oikeaMoottoriPin1, HIGH);
    digitalWrite(oikeaMoottoriPin2, LOW);      
  }

  // Aseta vasemman moottorin pyörimissuunta
  if (vasenMoottoriNopeus < 0)
  {
    digitalWrite(vasenMoottoriPin1, LOW);
    digitalWrite(vasenMoottoriPin2, HIGH);    
  }
  else if (vasenMoottoriNopeus >= 0)
  {
    digitalWrite(vasenMoottoriPin1, HIGH);
    digitalWrite(vasenMoottoriPin2, LOW);      
  }

  // Aseta oikean ja vasemman moottorin nopeus käyttäen PWM:ää (Pulssinleveysmodulaatio)
  analogWrite(oikeaMoottoriEnable, abs(oikeaMoottoriNopeus));
  analogWrite(vasenMoottoriEnable, abs(vasenMoottoriNopeus));    
}
