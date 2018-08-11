/**
 * This file/code is part of GSM-security-alarm-system project.
 *
* https://github.com/vadimkholodilo/GSM-security-alarm-system
 *
 * Copyright (c) 2017 Vadim Kholodilo
 *
 * Licensed under the MIT license.
 */
#include "message.h"
#include <util/delay.h>
#include "alarm.h"
//Переменные
const char NUMBER[13]; // Номер, на который будет звонить сегнализация и отправлять СМС
const int SPEAKER = 10; // пин для подключения динамика
const int MOVEMENTSENCER = 3; // Пин для подключения датчика движения
int notified = 0;
volatile boolean opened = 0; // Эта переменная будет изменена по внешнему прерыванию от геркона
volatile boolean movement = 0; // Эта переменная будет изменена по внешнему прерыванию от датчика движения
const int DOOR1 = 2; // Пин, на котором находится геркон, отвечающий за 1-ую дверь
const int DOORUSUALSTATE = LOW; // Обычно на всех пинах дверей должна быть единица
const int MOVEMENTSENCERUSUALSTATE = LOW;
const int RELAY = 4;
int door1State; // Сигнал на пине, отвечающем за первую дверь
int movementSencerState;
void setup() {
pinMode(DOOR1, INPUT);
digitalWrite(DOOR1, HIGH);
	pinMode(MOVEMENTSENCER, INPUT);
	digitalWrite(MOVEMENTSENCER, LOW);
	pinMode(RELAY, OUTPUT);
	GSMInit();
	while (call() == 0) {
		// Nothing goes here
	}
	sound(SPEAKER, 200);
attachInterrupt(0, gerconInterruptionHandler, CHANGE);
	attachInterrupt(1, movementSencerInterruptionHandler, CHANGE);
	ADCSRA = 0x00;
}
void loop() {
	if (opened) {
		digitalWrite(RELAY, HIGH);
		alarm(SPEAKER);
		if (notified == 0) {
			GSMRing(NUMBER);
			//GSMSendSMS("Dver bila otkrita", NUMBER);
notified = 1;
		}
	}
	else {
		digitalWrite(RELAY, LOW);
		notified = 0;
	}
	if (readVcc() < 3.3) {
		GSMSendSMS("Batareya razryajena", NUMBER);
	}

}
	//функции

float readVcc() { //Функция для получения напряжени	я питания
	const float typVbg = 1.179; // This constant should be colibrated
	byte i;
	float result = 0.0;
	float tmp = 0.0;

	for (i = 0; i < 5; i++) {
		// Read 1.1V reference against AVcc
		// set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
		ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		ADMUX = _BV(MUX3) | _BV(MUX2);
#else
		// works on an Arduino 168 or 328
		ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

		delay(3); // Wait for Vref to settle
		ADCSRA |= _BV(ADSC); // Start conversion
		while (bit_is_set(ADCSRA, ADSC)); // measuring

		uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
		uint8_t high = ADCH; // unlocks both

		tmp = (high << 8) | low;
		tmp = (typVbg * 1023.0) / tmp;
		result = result + tmp;
		delay(5);
	}

	result = result / 5;
	return result;
}
void gerconInterruptionHandler() { // Обработчик прерывания
	door1State = digitalRead(DOOR1);
	if (door1State != DOORUSUALSTATE) opened = 1;
	else opened = 0;
}
void movementSencerInterruptionHandler() {
	movementSencerState = digitalRead(MOVEMENTSENCER);
	if (MOVEMENTSENCERUSUALSTATE != movementSencerState) movement = 1;
	else movement = 0;
}
void sound(const int PIN, const int DURATION) { // Генирирует звук на указанном пине с указанной длительностью
	analogWrite(PIN, 150);
	_delay_ms(DURATION);
	analogWrite(PIN, 0);
}