/**
 * This file/code is part of GSM-security-alarm-system project.
 *
* https://github.com/vadimkholodilo/GSM-security-alarm-system
 *
 * Copyright (c) 2017 Vadim Kholodilo
 *
 * Licensed under the MIT license.
 */
#pragma once
#include <util/delay.h>
#include <Arduino.h>
// Функция генирации сигнала серены
void alarm(const int PIN) {
	for (int i = 0; i < 255; i = i + 2) {
		analogWrite(PIN, i);
		_delay_ms(10);
	}
	for (int i = 255; i > 1; i = i - 2) {
		analogWrite(PIN, i);
		_delay_ms(5);
	}
	for (int i = 1; i <= 10; i++) {
		analogWrite(PIN, 200);
		_delay_ms(100);
		analogWrite(PIN, 25);
		_delay_ms(100);
	}
}