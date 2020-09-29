const int buttonRed    = 7;
const int buttonGreen  = 6;
const int buttonYellow = 5;
const int buttonBlue   = 4;

const int interrupt1   = 3;
const int interrupt2   = 2;

const int ledRed    = 13;
const int ledGreen  = 12; 
const int ledYellow = 11; 
const int ledBlue   = 10;

const int scoreMin      = 2;
const int delayMin      = 900;
const int delayMax      = 2000;
const int delayDecrease = 500;

volatile  int score         = 0;
volatile  int scoreLimit    = 2;
volatile  int delayBetween  = 2000;
volatile bool endGame       = false;
volatile  int activeLed     = 0;
volatile  int activeBtn     = 0;
volatile bool alreadyPushed = false;

void setup() 
{
  Serial.begin(9600);
  
  randomSeed(analogRead(A0));
  
  pinMode(ledRed,    OUTPUT);
  pinMode(ledGreen,  OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledBlue,   OUTPUT);
  
  pinMode(buttonRed,    INPUT);
  pinMode(buttonGreen,  INPUT);
  pinMode(buttonYellow, INPUT);
  pinMode(buttonBlue,   INPUT);
  
  attachInterrupt(digitalPinToInterrupt(interrupt1), checkPushedButton, RISING);
  attachInterrupt(digitalPinToInterrupt(interrupt2), checkPushedButton, RISING);  
}
  
void loop()
{
  if (!alreadyPushed)
    showFail();
  
  changeLeds(LOW);
  
  delay(400);

  alreadyPushed = false;
  activeBtn     = 0;
  
  int randomNumber = random(10, 14);
  
  for (int current = ledRed; current >= ledBlue; current--)
  {
    if (randomNumber == current)
    {
      digitalWrite(current, HIGH);
      activeLed = current;
      break;
    }
  }  

  delay(delayBetween);  
}

void checkPushedButton()
{
  if (digitalRead(buttonRed) == HIGH)
    activeBtn = ledRed;
  else if (digitalRead(buttonGreen) == HIGH)
    activeBtn = ledGreen;
  else if (digitalRead(buttonYellow) == HIGH)
    activeBtn = ledYellow;
  else if (digitalRead(buttonBlue) == HIGH)
    activeBtn = ledBlue;
      
  processScore(activeBtn);
    
  debugLed();
    
  alreadyPushed = true;
}  

void processScore(int led)
{
  if (!alreadyPushed)
  {
    if (activeBtn == activeLed)
      score++;
    else 
      showFail();
  }
  
  if (score == scoreLimit)
    showRoundResult();
}  

void showRoundResult()
{
  changeLeds(HIGH);

  score = 0;
  
  scoreLimit++;
    
  if ((delayBetween -= delayDecrease) <= delayMin)
    showEndGame(); 
  else
    delay(1000);
}

void showFail()
{
  score = 0;
  
  changeLeds(HIGH);
  
  delay(100);
  
  changeLeds(LOW);
}  

void showEndGame()
{
  scoreLimit   = scoreMin;
  delayBetween = delayMax;
  
  for (int j = 0; j < 5; j++)
  {
    for (int current = ledRed; current >= ledBlue; current--)
    {
      digitalWrite(current, HIGH);
      delay(200);
      changeLeds(LOW);
    }
  }
}

void changeLeds(byte state)
{
  digitalWrite(ledRed,    state);
  digitalWrite(ledGreen,  state);
  digitalWrite(ledYellow, state);
  digitalWrite(ledBlue,   state);
}

void debugLed()
{
  if (ledRed == activeLed)
    Serial.print("Led: Red    || ");
  else if (ledGreen == activeLed)
    Serial.print("Led: Green  || "); 
  else if (ledYellow == activeLed)
    Serial.print("Led: Yellow || ");
  else if (ledBlue == activeLed)
    Serial.print("Led: Blue   || ");
    
  if (digitalRead(activeLed) == HIGH)
    Serial.print("LedState: HIGH");
  else
    Serial.print("LedState: LOW ");
    
  Serial.print(" || Score: "); 
  Serial.print(score);
  Serial.print("/");
  Serial.println(scoreLimit);
}  
