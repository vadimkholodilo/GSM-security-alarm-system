/**
 * This file/code is part of GSM-security-alarm-system project.
 *
* https://github.com/vadimkholodilo/GSM-security-alarm-system
 *
 * Copyright (c) 2017 Vadim Kholodilo
 *
 * Licensed under the MIT license.
 */
#include <SoftwareSerial.h>
#include <util/delay.h>
#include <arduino.h>
SoftwareSerial GSM(12, 11);
// Функции для работы с GSM модемом
void GSMInit() { // Функция инициализации модема
	GSM.begin(9600);
	pinMode(5, OUTPUT);
	digitalWrite(5, LOW);
	_delay_ms(2000); // Даем модему время загрузиться
	GSM.println("ATE0"); // Выключаем эхо
	_delay_ms(100);
	GSM.println("AT+CLIP=1");
	_delay_ms(100);
	while (!GSM.find("0,0")) {
		GSM.println("AT+COPS?");
		_delay_ms(100);
	}
}
void GSMSendSMS(String text, String phone) {//Функция отправки SMS сообщения
	GSM.println("AT+CMGF=1"); // Включаем текстовый режим отправки сообщений
	_delay_ms(100);
	GSM.println("AT+CSCS=\"GSM\""); // кодировка СМС "GSM"
	_delay_ms(100);
	GSM.println("AT+CMGS=\"" + phone + "\""); // Указываем номер получателя
	_delay_ms(500);
	GSM.print(text); // вводим текст
	_delay_ms(500);
	GSM.print((char)26); // Чтоб отправить СМС надо послать модему символ с кодом 0x26
	delay(200);

}
void GSMOff() { // Функция отключения модема
	GSM.println("AT+CPWROFF");
}
void GSMRing(String phone) {
	GSMInit();
	GSM.println("ATD" + phone + ";");
	_delay_ms(2000);



}
int call() {
	if (GSM.find("RING")) return 1;
	else return 0;
}