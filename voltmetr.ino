/* Входное напряжение подается на аналоговый вход ардуино через делитель напряжения
  выполненый на резисторах номиналом 10кОм и 1 кОм. Напряжение снимается с резистора 1 кОм,
  так как максимальное напряжение на аналоговые входы ардуино - не более 5 вольт.
  Коэффициент делителя напряжения таким образом 11. При сборке делителя на резисторах других номиналов
  необходимо будет задавать соответствующий коэффициент.
*/

# define Kd 11 // Коэффициент делителя напряжения (R2/(R2+R1))
# define LOW_VOLTAGE 23 // Нижний предел измерения напряжения
# define HIGH_VOLTAGE 27 // Верхний предел измерения напряжения
# define YourPin 9
# define VoltPin A0



float voltage; //напряжение
int state = 1; // переменная текущего состояния
int start_wait = 30; //seconds
int mid_wait = 60; //seconds
int final_wait = 120; // minutes
long current_time = 0;// переменная таймера
int period = 1000;// миллисек //как часто выводить показания в сериал




// Функция временной задержки
void waiting (int p) {
  unsigned long t1;
  unsigned long t2;
  t1 = millis();
  t2 = millis();
  while ((t2 - t1) < (1000 * p))
  {
    t2 = millis();
    voltage = (analogRead(VoltPin) * (5.0 / 1023.0)) * Kd;
    ShowV();
    //    if (something)break; /  можно поставить условие в скобки чтобы выйти из задержки
  }
}

void getV() {
  voltage = (analogRead(VoltPin) * (5.0 / 1023.0)) * Kd;
}

void ShowV(){
  if (millis()-current_time>period){
    Serial.print("Напряжение - ");
    Serial.println(voltage);
    current_time=millis();
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(YourPin, OUTPUT);
  digitalWrite(YourPin, LOW);
  waiting(start_wait);

}

void loop() {
  if (state == 1) {
    while (1) {
      getV();
      ShowV();
      if (voltage < LOW_VOLTAGE) {
        state = 2;
        waiting(mid_wait);
        break;
      }
    }
  }

  if (state == 2) {
    getV();
    ShowV();
    if (voltage < LOW_VOLTAGE) {
      digitalWrite(YourPin, HIGH);
      state = 3;
    }

    if (voltage >= LOW_VOLTAGE) {
      state = 1;
    }
  }

  if (state == 3) {
    getV();
    ShowV();
    if (voltage >= HIGH_VOLTAGE) {
      waiting(60 * final_wait);
      digitalWrite(YourPin, LOW);
      state = 1;
    }
  }

}
