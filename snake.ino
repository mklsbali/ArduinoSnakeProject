#include <LiquidCrystal.h>

//Allocate LCD Control Pins
bool gameOver;
const int RS = 7;             //Set RS pin as Pin 7
const int Enable = 6;         //Set Enable as Pin 6
const int D4 = 5;              //Set D4 pin as Pin 5
const int D5 = 4;              //Set D5 pin as Pin 4
const int D6 = 3;              //Set D6 pin as Pin 3
const int D7 = 2;             //Set D7 pin as Pin 2

const int height = 2;                 //2 Rows
const int width = 16;                 //16 Columns

//Declare the LCD variable, with defined Pins

LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7);

int maxLength = 12;

// Set the time it takes the snake to move

long timeToMove = 1500;


int p = 23;
char incoming_value = 'r';

const int TAIL = 4;               //The Tail is Custom Character 0
const int HEAD = 1;               //The Head is Customer Character 1
const int CLEAR = 2;              //Custom Character 2 is used for Clear Space
const int APPLE = 3;


byte snakeHead[8] = {     //The BitMap for the head
  B10001,
  B01010,
  B11111,
  B10101,
  B11111,
  B10101,
  B10001,
  B01110
};

byte snakeTail[8] = {    //The BitMap for the tail
  B00100,
  B01110,
  B01110,
  B11011,
  B11011,
  B01110,
  B01110,
  B00100
};

byte noSnake[8] = {        //The BitMap for Clear Space
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte apple[8] = {          //The BitMap for Clear Space
  B00000,
  B00000,
  B00010,
  B00100,
  B01110,
  B11011,
  B11110,
  B01100
};

int lcdOutput [height][width];

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton dir;


void render()
{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      lcd.setCursor(j, i);
      lcd.write(lcdOutput[j][i]);
    }
  }
}
void start()
{
  lcd.begin(height, width);
  lcd.createChar(HEAD, snakeHead);
  lcd.createChar(TAIL, snakeTail);
  lcd.createChar(CLEAR, noSnake);
  lcd.createChar(APPLE, apple);

  lcd.setCursor(6, 0);
  lcd.print("Snake!");
  delay(500);

  lcd.setCursor(7, 1);
  lcd.print("By Miklos");
  delay(500);

  lcd.setCursor(2, 0);
  lcd.write(HEAD);
  delay(500);
  lcd.setCursor(1, 0);
  lcd.write(TAIL);
  delay(500);
  lcd.setCursor(0, 0);
  lcd.write(TAIL);
  delay(1000);

  digitalWrite(22, HIGH);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      lcdOutput[col][row] = CLEAR;
    }
  }
  render();

  gameOver = false;
  dir = STOP;
  x = width / 2;
  y = height / 2;
  fruitX = random(1,15);
  fruitY = random(0,1);
  score = 0;

}
void Draw()
{


  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (i == y && j == x)
        lcdOutput[j][i] = HEAD;
      else if (i == fruitY && j == fruitX)
        lcdOutput[j][i] = APPLE;
      else
      {
        bool print = false;
        for (int k = 0; k < nTail; k++)
        {
          if (tailX[k] == j && tailY[k] == i)
          {
            lcdOutput[j][i] = TAIL;

            print = true;
          }
        }
        if (!print)
          lcdOutput[j][i] = CLEAR;
      }
    }
  }
}
void Input()
{
  if (Serial1.available()) {
    // Read from Bluetooth and send to PC
    incoming_value = Serial1.read();
    Serial.write(incoming_value);
  }

  switch (incoming_value)
  {
    case 'l':
      dir = LEFT;
      break;
    case 'r':
      dir = RIGHT;
      break;
    case 'u':
      dir = UP;
      break;
    case 'd':
      dir = DOWN;
      break;
    case 'x':
      gameOver = true;
      break;
  }
}

void Logic()
{
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;
  for (int i = 1; i < nTail; i++)
  {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }

  switch (dir)
  {
    case LEFT:
      x--;
      break;
    case RIGHT:
      x++;
      break;
    case UP:
      y--;
      break;
    case DOWN:
      y++;
      break;
    default:
      break;
  }
  //if (x > width || x < 0 || y > height || y < 0)
  //  gameOver = true;
  if (x >= width) x = 0; else if (x < 0) x = width - 1;
  if (y >= height) y = 0; else if (y < 0) y = height - 1;

  for (int i = 0; i < nTail; i++)
    if (tailX[i] == x && tailY[i] == y)
      gameOver = true;
  if (nTail == maxLength)
    gameOver = true;
  if (x == fruitX && y == fruitY)
  {
    score += 10;
    fruitX = random(1,15);
    fruitY = random(0,1);
    nTail++;
	digitalWrite(p++, HIGH);
  }

}

unsigned long previousMillis = 0;
void setup() {

  //Set Pins as Output

  pinMode(RS, OUTPUT);
  pinMode(Enable, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  DDRA = 0b11111111;//DDRA as output
  DDRC = 0b11111111;//DDRC as output for marking the snake's current length
  Serial.begin(9600); // Interfata Serial 0, pentru PC
  Serial1.begin(9600); // Interfata Serial 1, pentru Bluetooth
  



}
void loop() {

  start();
  while (!gameOver)
  {
    render();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > timeToMove)
    {
      previousMillis = currentMillis;
      //delay(1);
      Draw();
     // delay(1);
      Input();
    //  delay(1);
      Logic();
     // delay(1);

    }

  }
  lcd.setCursor(0, 0);
  lcd.print("Game over");
  lcd.setCursor(0, 1);
  lcd.print("Score ");
  lcd.print(score);
}
