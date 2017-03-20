#include "STM32F4xx.h"
#include "LCD/lcd_driver.h"
#include "SWT.h"
#include "CommonFunctions.h"
#include "LED.h"
#include <string.h>
#include "menu.h"
#include <stdlib.h>
#include "ADC.h"
#include "StateMenu.h"
#include <stdio.h>
#include <string.h>



int MULTIMETER_MODE = MODE_VOLTAGE;


#define SCROLL_RATE 400

const float maximInputVoltage = 3.0; // The input voltage which represents a maximum reading.
float maximumValue = 0.0;
float minimumValue = 10.0;


void scrollText(char message[], int messageLength){

	char lastCharacter = message[messageLength];
	for(int i=messageLength; i > 0; i--){
		message[i] = message[i-1];
	}
	message[0] = lastCharacter;
}

MenuIds openVoltageMenu(){
	
	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[3] = {0,1,7};
	int size = 3;
	int buttonPressed = printAndWait("Select options", "1.Manual Range    2.Auotmatic Range ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
			break;
		case 1:
			selectedMenu = MENU_ID_VOLTAGE_AUTO_RANGE;
			break;
		case 7:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualVoltage() {

	MULTIMETER_MODE = MODE_VOLTAGE;
	int buttonArray[5] = {0,1,2,3,7};
	int size = 5;
	
	selectMode(0xF);
	
	int buttonPressed = printAndWait("Voltage Manual", " 1.10V  2.1V  3.100mV 4.10mV ", buttonArray, size);
	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;
	
	switch(buttonPressed) {
		case 0: 
			//manual range of 10 V
			selectedSettings.selectedRange = RANGE_ID_RANGE_10;
			break;
		case 1:
			//manual range of 1 V
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 2:
			//manual range of 100 mV
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 3:
			//manual range of 10 mV
			selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 7:
			selectedSettings.nextMenu = MENU_ID_VOLTAGE;
			break;
	}
	
	return selectedSettings;
}

MenuIds openCurrentMenu(){
	
	MULTIMETER_MODE = MODE_CURRENT;
	
	int buttonArray[3] = {0,1,7};
	int size = 3;
	
	int buttonPressed = printAndWait("Select options", "1.Manual Range    2.Auotmatic Range ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_CURRENT_MANUAL_RANGE;
			break;
		
		case 1:
			selectedMenu = MENU_ID_CURRENT_AUTO_RANGE;
			break;
		
		case 7:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualCurrent() {
	
	MULTIMETER_MODE = MODE_CURRENT;
	int buttonArray[4] = {0,1,2,7};
	int size = 4;
	
	selectMode(0xF);
	
	int buttonPressed = printAndWait("Current Manual", "1.1A  2.100mA 3.10mA ", buttonArray, size);
	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_1;
	
	switch(buttonPressed) {

		case 0:
			//manual range of 1 A
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 1:
			//manual range of 100 mA
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 2:
			//manual range of 10 mA
			selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 7:
			selectedSettings.nextMenu = MENU_ID_CURRENT;
			break;
	}
	
	return selectedSettings;
	
}

MenuIds openResistanceMenu(){
	
	MULTIMETER_MODE = MODE_RESISTANCE;
	
	int buttonArray[3] = {0,1,7};
	int size = 3;
	int buttonPressed = printAndWait("Select options", " 1.Manual Range    2.Auotmatic Range ", buttonArray, size);
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
	
	switch(buttonPressed) {
		case 0: 
			selectedMenu = MENU_ID_RESISTANCE_MANUAL_RANGE;
			break;
		
		case 1:
			selectedMenu = MENU_ID_RESISTANCE_AUTO_RANGE;
			break;
		
		case 7:
			selectedMenu = MENU_ID_OPEN;
			break;
	}
	
	return selectedMenu;
}

RangeMenuSettings openManualResistance() {
	
	MULTIMETER_MODE = MODE_RESISTANCE;
	int buttonArray[5] = {0,1,2,3,7};
	int size = 5;
	
	selectMode(0xF);
	
	int buttonPressed = printAndWait("Resistance Manual", "1.1MOhm 2.100kOhm 3.10kOhm 4.1kOhm ", buttonArray, size);
	RangeMenuSettings selectedSettings;
	selectedSettings.nextMenu = MENU_ID_MEASUREMENT;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;
	
	switch(buttonPressed) {
		case 0: 
			//manual range of 1MOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_10;
			break;
		case 1:
			//manual range of 100kOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_1;
			break;
		case 2:
			//manual range of 10kOhm
			selectedSettings.selectedRange = RANGE_ID_RANGE_100m;
			break;
		case 3:
			//manual range of 1kOhm
		  selectedSettings.selectedRange = RANGE_ID_RANGE_10m;
			break; 
		case 7:
			selectedSettings.nextMenu = MENU_ID_RESISTANCE;
			break;
	}
	
	return selectedSettings;
	
}

MenuIds openMenu(){

	int buttonArray[4] = {0,1,2,7};
	int size = 4;
	int buttonPressed = printAndWait("Select Function", " 1.Voltage   2.Current   3.Resistance ", buttonArray, size);
	
	MenuIds selectedMenu = MENU_ID_VOLTAGE; 
	switch (buttonPressed){
		case 0:
			selectedMenu = MENU_ID_VOLTAGE;
			break;
		
		case 1:
			selectedMenu = MENU_ID_CURRENT;
			break;
				
		case 2:
			selectedMenu =  MENU_ID_RESISTANCE;
			break;
		
		case 7:
			selectedMenu = MENU_ID_MEASUREMENT; //GO TO measuerment menu
			break;
	}
	return selectedMenu;
}


MenuIds openMaxMenu(){
	
	int button = 7;
	int size = 1;
		
	char stringValue[16];
	sprintf(stringValue, "%.4f", maximumValue);
	
	char stringMeasure[16];
	
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
		switch(MULTIMETER_MODE) {
			case MODE_VOLTAGE: 
				strncpy(stringMeasure, "Maximum Voltage", 15); 
				break;
			
			case MODE_CURRENT:
				strncpy(stringMeasure, "Maximum Current", 15); 
				break;
			
			case MODE_RESISTANCE:
				strncpy(stringMeasure, "Maximum Resist.", 15); 
				break;
		}
		
	int buttonPressed = printAndWait(stringMeasure, stringValue, &button, size);
	
	return selectedMenu;
}

MenuIds openMinMenu(){
	
	int button = 7;
	int size = 1;
		
	char stringValue[16];
	sprintf(stringValue, "%.4f", minimumValue);
	
	char stringMeasure[16];
	
	MenuIds selectedMenu = MENU_ID_MEASUREMENT; 
		switch(MULTIMETER_MODE) {
			case MODE_VOLTAGE: 
				strncpy(stringMeasure, "Minimum Voltage", 15); 
				break;
			
			case MODE_CURRENT:
				strncpy(stringMeasure, "Minimum Current", 15); 
				break;
			
			case MODE_RESISTANCE:
				strncpy(stringMeasure, "Minimum Resist.", 15); 
				break;
		}
	
	int buttonPressed = printAndWait(stringMeasure, stringValue, &button, size);
	
	return selectedMenu;
}

int printAndWait(char firstLineString[], char* secondLineString, int buttons[], int size) {
		
	lcd_write_string(firstLineString, 0, 0);

	int messageLength = strlen(secondLineString)-1;
	char* message = malloc(messageLength+1 * sizeof(char));
	strcpy(message, secondLineString);

	int buttonPressed = -1;
	
	while(buttonPressed == -1){
		//If message wont fit on the screen then scroll it
		if(messageLength > 16){
			scrollText(message, messageLength);
		}

		lcd_write_string(message, 1, 0);
		buttonPressed = DelayForButton(SCROLL_RATE, buttons, size);	
	}

	free(message);
	return buttonPressed; 
}

void menu(){
	MenuIds selectedMenuID = MENU_ID_OPEN;
	RangeMenuSettings selectedSettings;
	selectedSettings.selectedRange = RANGE_ID_RANGE_10;

	int autoRange = 0; 
	while(1){
		
		waitForRelease();
		lcd_clear_display();
		LED_On(7);

		switch (selectedMenuID) {
			
			case MENU_ID_MEASUREMENT:
				LED_Out(96);
				selectedMenuID = measurementMenu(autoRange, selectedSettings.selectedRange);
				break; 
			
			case MENU_ID_OPEN:
				LED_Out(7);
				selectedMenuID = openMenu();
				break;
			
			case MENU_ID_VOLTAGE:  //go to Voltage menu
				LED_Out(3);
				selectedMenuID = openVoltageMenu();
				break;
			
			case MENU_ID_VOLTAGE_MANUAL_RANGE:
				//manual range for Current
				//go to select range menu
				LED_Out(15);
				selectedSettings = openManualVoltage();	
				selectedMenuID = selectedSettings.nextMenu;
			  autoRange = 0; 
				break;
			
			case MENU_ID_VOLTAGE_AUTO_RANGE:
				//auto range for Current
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
			
			case MENU_ID_CURRENT:
				//got to Current menu
				LED_Out(3);
				selectedMenuID = openCurrentMenu();
				break; 
			
			case MENU_ID_CURRENT_MANUAL_RANGE:
				//manual range for CURRENT
				//go to select range menu
			  LED_Out(7);
				selectedSettings = openManualCurrent();
				selectedMenuID = selectedSettings.nextMenu;
				autoRange = 0; 
				break;
			
			case MENU_ID_CURRENT_AUTO_RANGE:
				//auto range for CURRENT
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
			
			case MENU_ID_RESISTANCE:
				//got to resistance menu
			  LED_Out(3);
				selectedMenuID = openResistanceMenu();
				break;
			
			case MENU_ID_RESISTANCE_MANUAL_RANGE:
				//manual range for resistance
				//go to select range menu
				LED_Out(15);
				selectedSettings = openManualResistance();
				selectedMenuID = selectedSettings.nextMenu;
				autoRange = 0; 
				break;
			
			case MENU_ID_RESISTANCE_AUTO_RANGE:
				//auto range for resistance
				//auto ranging stuff 
				LED_Out(0);
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
			
			case MENU_ID_MIN:
				LED_Out(0);
				selectedMenuID = openMinMenu();
				break;
			
			case MENU_ID_MAX:
				LED_Out(0);
				selectedMenuID = openMaxMenu();
				break;
			
			default:
				selectedMenuID = MENU_ID_MEASUREMENT;
				autoRange = 1; 
				break;
		}	
	}	
}

RangeIds autoRanging(RangeIds currentRange) {
	
	int value = read_ADC1();
		
	if (value >= SAMPLES_DEPTH && currentRange != RANGE_ID_RANGE_10) {
		currentRange--;
	} else if (value <= 10 && currentRange != RANGE_ID_RANGE_10m) {
		currentRange++;
	}
	return currentRange;
}


MenuIds measurementMenu(int isAutoRangeOn, RangeIds range) {
	
	int buttonArray[3] = {5,6,7};	// What button to listen for.
	int size = 3;					//Number of buttons to listen for.
	MenuIds selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE; 
	
	char* measurement = "";
	
	int buttonPressed = -1;
	
	maximumValue = 0.0;
  minimumValue = 10.0;
	
	unsigned int value; // Raw value measured from ADC (0-4096)
	float actualValue; // Actual value being represented by the input (eg 100 mA)
	
	while( buttonPressed !=  7){

		//TODO take the variable init out of the loop

		
		char* units; // The Units of the current value to be displayed.
		char* rangeString;// The range (and units) of this measurement.
		char modeString = 'M';// Indicated which mode the multimeter is in (Auto-range or manual).
		
		unsigned int rangeMode ;	//range Control signal.
		unsigned int typeMode ; //Measurement type control signal (Eg. Are we measuring current?)
		
		buttonPressed = DelayForButton(300, buttonArray, size);
		
		if (buttonPressed == 6) { 
			return MENU_ID_MAX; //go to the maximum menu
		} else if (buttonPressed == 5) {
			return MENU_ID_MIN; //go to the minimum menu
		} else {
		
			if (isAutoRangeOn == 1) {
				range = autoRanging(range);
				modeString = 'A';
			}
			
			state currentState = stateLookUp[MULTIMETER_MODE][range];
			measurement = currentState.measurementString;
			typeMode = currentState.typeMode;
			rangeMode = currentState.rangeMode;
			rangeString = currentState.rangeString;
			units = currentState.unitString;
			
			selectMode( typeMode | (rangeMode<<2));
			value = read_ADC1();
			actualValue = retSignedValue(value, currentState.scalingFactor);
			
			if (actualValue > maximumValue) {
				maximumValue = actualValue;
			}
			
			if (actualValue < minimumValue) {
				minimumValue = actualValue;
			}
			
			display_Measure(measurement, modeString, rangeString, units, actualValue);	
			
			//Set which menu to return to if the menu button is pressed.
			switch(MULTIMETER_MODE){
				
				default:
				case MODE_VOLTAGE:
					selectedMenu = MENU_ID_VOLTAGE_MANUAL_RANGE;
					break;
				
				case MODE_CURRENT:
					selectedMenu = MENU_ID_CURRENT_MANUAL_RANGE;
					break;
				
				case MODE_RESISTANCE:
					selectedMenu = MENU_ID_RESISTANCE_MANUAL_RANGE;
					break;
			}		
		}
	}
	return selectedMenu;
}

const float ADC_VREF = 3.0;

//Scales the adc reading to produce a meaningful measurement value.
float retSignedValue(int readValue, float rangeValue) { 
	float retValue;
	float maxUsableDivisions = (4096.0f/ADC_VREF) * maximInputVoltage;
	
	if ( MULTIMETER_MODE == MODE_VOLTAGE ) {
			retValue = (readValue * (ADC_VREF/4096.0f) * rangeValue/maximInputVoltage) - (rangeValue/2.0f);
		//	retValue = (readValue * (ADC_VREF/4096.0f));//NOTE we thing the vfref for the ad is actually 3 v not 3.3 so we have changes this.
	} else {
		retValue = (readValue * (ADC_VREF/4096.0f) * rangeValue/maximInputVoltage);
	}
	
	return retValue;
}
