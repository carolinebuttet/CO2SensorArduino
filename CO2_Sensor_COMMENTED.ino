// WHAT THIS SCRIPT DOES: 
// Read the values of a sensor; open and close relays accordingly. 
// Display the value on a LCD screen
// Change threshold with a potentiometer

// This script is meant to be used with Seed Studio's Relay Shield for Arduino.
// https://www.distrelec.ch/fr/shield-relais-seeed-studio-103030009/p/30069883
// https://wiki.seeedstudio.com/Relay_Shield_v3/
// Digital 4 – controls RELAY4’s COM4 pin (located in J4)
// Digital 5 – controls RELAY3’s COM3 pin (located in J3)
// Digital 6 – controls RELAY2’s COM2 pin (located in J2)
// Digital 7 – controls RELAY1’s COM1 pin (located in J1)

//import libraries
# include <Wire.h> 
# include <Adafruit_GFX.h> // library for LCD screen display
# include <Adafruit_SH1106.h> // driver for LCD screen

# define OLED_RESET -1

Adafruit_SH1106 display(OLED_RESET);  

int relay1 = 5;    // Digital Arduino Pin used to control the motor
int relay2 =  4;   // Digital Arduino Pin used to control the motor

int fcount=0;

int CO2Threshold = 560; // The initial threshold of CO2
bool isrelay1Open = false; // the logic to determine if the window is open or closed
bool isrelay2Open = false;


void setup()  { // Executed only once
  
  Serial.begin(9600); // Start the serial, what allows us to write values to our computer
  pinMode(relay1, OUTPUT); // Set pin 5 to output mode (to send 5V when the window needs to be open)
  digitalWrite(relay1,LOW); // Set this pin to 0V for now
  pinMode(relay2, OUTPUT); 
  digitalWrite(relay2,LOW);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); // Set up the LCD display. Each display will have a different library/setup.
  display.clearDisplay(); // Clear the LCD display
  display.setTextColor(WHITE);// Set the text color to white
  display.setTextSize(1);  // Set the text size to 1
}
 
void loop()  { // This code will be executed infinitely
  
  delay(200); // Wait 200ms between each check. You can change this value. 
  fcount++;
  int thresholdFromPotentiometer = analogRead(A1); // Get the current value of the potentiometer (0 - 1024)
  CO2Threshold = thresholdFromPotentiometer; // Set the CO2 threshold to the current value of the potentiometer. Comment this line if you do not want the value to change with the potentiometer.
  int sensorValue = analogRead(A0); //  Get the current value of the CO2 sensor (0 - 1024)
  int mappedThreshold = map(thresholdFromPotentiometer, 0,1024,0,2000); // Remap the value of the potentiomenter from 0 to 2000
  int mappedC02Value = map(sensorValue, 0, 1024, 0, 2000); // Remar the value of the CO2 sensot form 0 to 2000
 
  display.clearDisplay(); // Clear the LCD display 
  display.setCursor(0,0); // Place the cursos on the LCD screen to write text
  display.print("C02 = "); // Write "CO2" on the screen
  display.print(mappedC02Value); // Write the CO2 value
  display.println(" ppm"); // Write "ppm"
  display.print("Seuil = "); // Same for the 'seuil' (threshold)
  display.print(mappedThreshold);
  display.println(" ppm");
  display.display();

  Serial.println(sensorValue); // Print the value in the serial monitor (it will show when you open the serial in your computer)
  
  if(mappedC02Value >  mappedThreshold ){ // if the CO2 value is bigger than the threshold
    // if the motor is already opening
    if(!isrelay1Open && isrelay2Open){
      return; // don't do anything
      }
    else{ // if it's not opening: 
      // go forward!
      Serial.println("Open window!");
      digitalWrite(relay2,HIGH); // open relay 1
      digitalWrite(relay1,LOW); // close relay 2
      isrelay1Open = false; // update the values accordingly
      isrelay2Open = true;
      
      display.clearDisplay(); // Write the text
      display.setCursor(0,0);
      display.println("Ouvrir la fenetre...");
      display.print("C02 = ");
      display.print(mappedC02Value);
      display.println(" ppm");
      display.print("Seuil = ");
      display.print(mappedThreshold);
      display.println(" ppm");
      display.display();
      
      delay(5000); // wait 5000 ms
     }
  }
  
  else if(mappedC02Value < mappedThreshold ){ // if the CO2 value is smaller than the threshold
    // close it!
    // if the motor is already closing
    if(isrelay1Open && !isrelay2Open ){
      return; // don't do anything
    }
    else{ // if the motot is not closing, close the window
      Serial.println("Close window!"); // Write over serial (this is just for debug)
      
      display.clearDisplay(); // Clear the LCD display and write the text
      display.setCursor(0,0);
      display.println("Fermer la fenetre...");
      display.print("C02 = ");
      display.print(mappedC02Value);
      display.println(" ppm");
      display.print("Seuil = ");
      display.print(mappedThreshold);
      display.println(" ppm");
      display.display();
      
      digitalWrite(relay2,LOW); // close relay 2
      digitalWrite(relay1,HIGH); // open relay 1
      isrelay1Open = true; // update the values accordingly
      isrelay2Open = false;
      delay(5000); // wait 5000 ms
    }
   }
}


// A function to remap numbers
long mapf(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
