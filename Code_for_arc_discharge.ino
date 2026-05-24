// ================= PIN DEFINITIONS =================
#define STEP_PIN   7
#define DIR_PIN    6
#define EN_PIN     10

#define START_BTN    9
#define REVERSE_BTN  8
#define ESTOP_PIN    2
#define LIMIT_PIN    3

// ================= PARAMETERS ======================
#define STEP_DELAY_US     1000    // dealy after pulse 
#define REVERSE_STEPS     600    // auto retract after limit
#define REVERSE_JOG_STEPS 10000    // manual  reverse

#define FORWARD_DIR HIGH
#define REVERSE_DIR LOW

// ================= GLOBAL FLAGS ====================
volatile bool estopTriggered = false;
volatile bool limitTriggered = false;

bool motorRunning = false;

// ================= INTERRUPTS ======================
void estopISR() {
  estopTriggered = true;
}

void limitISR() {
  limitTriggered = true;
}

// ================= STEP FUNCTION ===================
void stepMotor() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(STEP_DELAY_US);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(STEP_DELAY_US);
}

// ================= SETUP ===========================
void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  pinMode(START_BTN, INPUT);
  pinMode(REVERSE_BTN, INPUT);
  pinMode(ESTOP_PIN, INPUT);
  pinMode(LIMIT_PIN, INPUT);

  digitalWrite(EN_PIN, HIGH);  // motor OFF

  attachInterrupt(digitalPinToInterrupt(ESTOP_PIN), estopISR, RISING);
  attachInterrupt(digitalPinToInterrupt(LIMIT_PIN), limitISR, RISING);
}

// ================= LOOP ============================
void loop() {

  // -------- EMERGENCY STOP --------
  if (estopTriggered) {
    digitalWrite(EN_PIN, HIGH);
    motorRunning = false;

    while (digitalRead(ESTOP_PIN) == HIGH);
    estopTriggered = false;
  }

  // -------- MANUAL REVERSE JOG (10 cm) --------
  if (!motorRunning && digitalRead(REVERSE_BTN) == HIGH) {
    delay(30); // debounce
    if (digitalRead(REVERSE_BTN) == HIGH) {

      digitalWrite(DIR_PIN, REVERSE_DIR);
      digitalWrite(EN_PIN, LOW);

      for (long i = 0; i < REVERSE_JOG_STEPS; i++) {
        if (estopTriggered) break;
        stepMotor();
      }

      digitalWrite(EN_PIN, HIGH);
    }

    while (digitalRead(REVERSE_BTN) == HIGH);
  }

  // -------- START BUTTON (AUTO FORWARD) --------
  if (!motorRunning && digitalRead(START_BTN) == HIGH) {
    delay(30);
    if (digitalRead(START_BTN) == HIGH) {

      limitTriggered = false;
      motorRunning = true;

      digitalWrite(DIR_PIN, FORWARD_DIR);
      digitalWrite(EN_PIN, LOW);
    }

    while (digitalRead(START_BTN) == HIGH);
  }

  // -------- FORWARD MOTION --------
  if (motorRunning && !limitTriggered) {
    stepMotor();
  }

  // -------- LIMIT HIT → AUTO REVERSE --------
  if (limitTriggered) {

    digitalWrite(EN_PIN, HIGH);
    motorRunning = false;
    delay(40);

    digitalWrite(DIR_PIN, REVERSE_DIR);
    digitalWrite(EN_PIN, LOW);

    for (int i = 0; i < REVERSE_STEPS; i++) {
      if (estopTriggered) break;
      stepMotor();
    }

    digitalWrite(EN_PIN, HIGH);

    while (digitalRead(LIMIT_PIN) == HIGH);
    limitTriggered = false;
  }
}


