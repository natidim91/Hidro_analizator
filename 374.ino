#include "arduino_secrets.h"

/*
 * This file is part of 374_noit_Hidroanalizator.
 *
 * Copyright (C) 2025 Kristiyan_V.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2         
#define DHTTYPE DHT11    
#define SOIL_PIN A0      

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
    Serial.begin(9600);
    dht.begin();
    
    lcd.init();
    lcd.backlight();
    
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();
}

void loop() {
    static unsigned long lastSerialPrint = 0;
    unsigned long currentMillis = millis();

    
    int soilValue = analogRead(SOIL_PIN);
    float soilMoisturePercent = map(soilValue, 1023, 300, 0, 100);
    if (soilMoisturePercent < 0) soilMoisturePercent = 0;
    if (soilMoisturePercent > 100) soilMoisturePercent = 100;

    
    String soilStatus;
    if (soilMoisturePercent < 30) {
        soilStatus = "Dry";
    } else if (soilMoisturePercent < 60) {
        soilStatus = "Moistured";
    } else {
        soilStatus = "Wet";
    }

    
    if (currentMillis - lastSerialPrint >= 1000) {
        lastSerialPrint = currentMillis;
        Serial.print("Soil moisture %: ");
        Serial.print(soilMoisturePercent);
        Serial.println("%");
        Serial.print("Soil moisture: ");
        Serial.println(soilStatus);
    }

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil moisture:");
    lcd.setCursor(0, 1);
    lcd.print(soilMoisturePercent);
    lcd.print("% ");
    lcd.print(soilStatus);

    delay(2000);  
}
