// Sets LEDs to increasing intensity cycling through all colour combinations


// Defines

//#define DEBUG 1

#define offsetof(st, m) __builtin_offsetof(st, m)
//#define offsetof(st, m) ((size_t)(&((st *)0)->m))


// Constants

// Pins
const int led1 = 3;
const int led2 = 4;
const int led3 = 5;
const int led4 = 6;
const int led5 = 7;

const int red = 9;
const int green = 10;
const int blue = 11;

// Pin array
const int noleds = 5;

const int ledpins[noleds]= {
  led1, led2, led3, led4, led5
};

// Timing
const int quanta = 1000; // ms per led
const int noframes = 4; // number of frames to hold pattern for

// Intensities
const int intensities[] = {
  0, 1, 4, 25, 100
};

const int nointensity = (sizeof(intensities) / sizeof(int));

const int intensityvalslong[] = {
  0, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 2, 2, 1, 1, 0
};

const int intensityvalsshort[] = {
  0, 1, 2, 3, 4, 3, 2, 1
};

const int nointensityvalslong = (sizeof(intensityvalslong) / sizeof(int));
const int nointensityvalsshort = (sizeof(intensityvalsshort) / sizeof(int));


// Variables

typedef struct
{
  int r;
  int g;
  int b;
} leddef;

leddef ledstruct[noleds];

leddef *leds[noleds];


// Prototypes

void setLed(int led, int r, int g, int b);


// Functions

void setup()
{                
  int i;

#ifdef DEBUG
  Serial.begin(9600); 
  
  Serial.println("---=== Spectrum ===---");
  
  // Dump led pins
  Serial.print("ledpins["); Serial.print(noleds); Serial.print("] = {");
  for(i=0; i<noleds; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(ledpins[i]);
  }
  Serial.println("}");
  
  // Dump RGB pins
  Serial.print("red = "); Serial.println(red);
  Serial.print("green = "); Serial.println(green);
  Serial.print("blue = "); Serial.println(blue);
  
  // Print timing details
  Serial.print("LED quanta = "); Serial.print(quanta); Serial.println(" microseconds");
  Serial.print("LED frame = "); Serial.print(quanta * noleds); Serial.println(" microseconds");
  Serial.print("pattern frames = "); Serial.println(noframes);
  Serial.print("LED pattern length = "); Serial.print(quanta * noleds * noframes); Serial.println(" microseconds");

  // Dump intensities
  Serial.print("intensities["); Serial.print(nointensity); Serial.print("] = {");
  for(i=0; i<nointensity; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(intensities[i]);
     Serial.print("%");
  }
  Serial.println("}");

  Serial.print("intensityvalslong["); Serial.print(nointensityvalslong); Serial.print("] = {");
  for(i=0; i<nointensityvalslong; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(intensityvalslong[i]);
  }
  Serial.println("}");

  Serial.print("intensityvalsshort["); Serial.print(nointensityvalsshort); Serial.print("] = {");
  for(i=0; i<nointensityvalsshort; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(intensityvalsshort[i]);
  }
  Serial.println("}");
#endif

  // Set the LED pins as outputs, initialise the LED structs and pointers.
  for(i=0; i<noleds; i++){
    pinMode(ledpins[i], OUTPUT);  
    digitalWrite(ledpins[i], HIGH);

    leds[i] = &ledstruct[i];
    ledstruct[i].r = 0;
    ledstruct[i].g = 0;
    ledstruct[i].b = 0;
  }  

  // Set the colour pins as outputs
  pinMode(red, OUTPUT);     
  pinMode(green, OUTPUT);     
  pinMode(blue, OUTPUT);       

  // Initialise the colour pins
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}


// Main loop

void loop()
{
  int stage;
  int p1;
  int p2;
  int p3;
  int l1;
  int l2;
  int l3;
  byte *statebase;
  int i;
  int l;
  leddef *tmpled;

  for(stage=0; stage<4; stage++){
#ifdef DEBUG
    Serial.print("stage="); Serial.print(stage); Serial.println();
#endif

    switch(stage){
    case 0:
      p1 = offsetof(leddef, b);
      p2 = offsetof(leddef, g);
      p3 = offsetof(leddef, r);
      break;
    case 1:
      p1 = offsetof(leddef, g);
      p2 = offsetof(leddef, r);
      p3 = offsetof(leddef, b);
      break;
    case 2:
      p1 = offsetof(leddef, r);
      p2 = offsetof(leddef, b);
      p3 = offsetof(leddef, g);
      break;
    }

#ifdef DEBUG
    Serial.print("p1="); Serial.print(p1); Serial.print(", ");
    Serial.print("p2="); Serial.print(p2); Serial.print(", ");
    Serial.print("p3="); Serial.print(p3); Serial.println();
#endif

    for(l1=0; l1<nointensityvalsshort; l1++){
#ifdef DEBUG
      Serial.print("l1="); Serial.print(l1); Serial.println();
#endif
      for(l2=0; l2<nointensityvalsshort; l2++){
        for(l3=0; l3<nointensityvalslong; l3++){
          statebase = (byte *) leds[noleds - 1];
          *((int *)(statebase + p1)) = intensities[intensityvalsshort[l1]];
          *((int *)(statebase + p2)) = intensities[intensityvalsshort[l2]];
          *((int *)(statebase + p3)) = intensities[intensityvalslong[l3]];

          for(i=0; i<noframes; i++){
            for(l=0; l<noleds; l++){
              setLed(l, leds[l]->r, leds[l]->g, leds[l]->b);
            }
          }
          
          // Move
          tmpled = leds[0];
          for (i = 0; i < noleds - 1; i++) {
            leds[i] = leds[i+1];
          }
          leds[noleds - 1] = tmpled;
        }
      }
    }
  }
}


void setLed(int led, int r, int g, int b)
{
  int i;
  int rc;
  int gc;
  int bc;

  // Initialise the rgb pins
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);

  // Switch on the required led
  digitalWrite(ledpins[led], LOW);

  rc=0;
  gc=0;
  bc=0;  
  for(i=0; i<100; i++){
    rc+=r;
    if(rc>=100){
      digitalWrite(red, HIGH);
      rc-=100;
    }
    else{
      digitalWrite(red, LOW);
    }

    gc+=g;
    if(gc>=100){
      digitalWrite(green, HIGH);
      gc-=100;
    }
    else{
      digitalWrite(green, LOW);
    }

    bc+=b;
    if(bc>=100){
      digitalWrite(blue, HIGH);
      bc-=100;
    }
    else{
      digitalWrite(blue, LOW);
    }

    // Wait
    delayMicroseconds(quanta / 100);
  }

  // Switch off the led
  digitalWrite(ledpins[led], HIGH);  
}

