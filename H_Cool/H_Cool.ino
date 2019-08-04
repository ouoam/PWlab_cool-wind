#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer, outsideThermometer;
#include <../fonts/FreeSans9pt7b.h>
#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSansBold18pt7b.h>

#define TFT_RST 8
#define TFT_RS	9
#define TFT_CS	10	// SS
#define TFT_SDI 11	// MOSI
#define TFT_CLK 13	// SCK
#define TFT_LED 0	// 0 if wired to +5V directly
#define TFT_BRIGHTNESS 220
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
int p = 0, d = 0, f = 1, se = 0, ti = 5;
int Up = A0, Down = A1, Left = A2, Right = A3, Enter = A4, Buzz = A5;
int UpSt = 0, DownSt = 0, LeftSt = 0, RightSt = 0, EnterSt = 0;
int UD = 0, LR = 0, con = 0, zz = 0;
int SA = 3, SB = 4, SC = 5;
int Min = 10, C = 0;
int bg = 0, De = 12, ww = 0;
float tempC = 0;
unsigned long Ti, Tb, Tc, Td, sttime, yy;
int state;
int NT = 0, TP = 0, Mi = 0, R1 = 0, R2 = 0, TeMin = 0, TeC = 0;
int Ta[5], a = 0, ss = 0, aa = 0, bb = 0, cc = 0, dd = 0, Sec = 60, ee = 0, ff = 0;
int Ready = 7, Out = 6;
void setup() {
	sensors.begin();
	tft.begin();
	tft.clear();
	pinMode(SC, OUTPUT);
	pinMode(SB, OUTPUT);
	pinMode(SA, OUTPUT);
	pinMode(De, OUTPUT);
	digitalWrite(SC, LOW);
	digitalWrite(SB, LOW);
	digitalWrite(SA, LOW);
	digitalWrite(De, LOW);
	pinMode(Ready, OUTPUT);
	pinMode(Out, OUTPUT);
	Serial.begin(9600);
	tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
	tft.setGFXFont(&FreeSansBold18pt7b);
	tft.drawGFXText(27, 120, "H cool", COLOR_CYAN);
	delay(2000);
	tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);


}
void loop() {
	UpSt = analogRead(Up);
	DownSt = analogRead(Down);
	LeftSt = analogRead(Left);
	RightSt = analogRead(Right);
	EnterSt = analogRead(Enter);
	if (p == 0) // Setting
	{
		if (UpSt > 1000)
		{
			UD++;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
		}
		if (DownSt > 1000)
		{
			UD--;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
		}
		if (UD < 0 || UD > 3)
		{
			UD = 0;
		}
		if (UD == 0) // time to run
		{
			tft.drawRectangle(3, 87, 174, 116, COLOR_WHITE);
			tft.drawRectangle(3, 47, 174, 76, COLOR_BLUE);
			tft.drawRectangle(3, 167, 174, 196, COLOR_WHITE);
			if (LeftSt > 1000)
			{
				tft.fillRectangle(80, 50, 120, 74, COLOR_WHITE);
				Min--;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
			}
			if (RightSt > 1000)
			{
				tft.fillRectangle(80, 50, 120, 74, COLOR_WHITE);
				Min++;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
			}
			if (Min < 1)
			{
				Min = 1;
			}
		}
		else if (UD == 1) // temp
		{
			tft.drawRectangle(3, 47, 174, 76, COLOR_WHITE);
			tft.drawRectangle(3, 87, 174, 116, COLOR_BLUE);
			tft.drawRectangle(100, 127, 135, 156, COLOR_WHITE);
			if (LeftSt > 1000)
			{
				tft.fillRectangle(85, 90, 130, 114, COLOR_WHITE);
				C--;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
			}
			if (RightSt > 1000)
			{
				tft.fillRectangle(85, 90, 130, 114, COLOR_WHITE);
				C++;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
			}
		}
		else if (UD == 2) // defrost
		{
			tft.drawRectangle(3, 87, 174, 116, COLOR_WHITE);
			tft.drawRectangle(100, 127, 135, 156, COLOR_BLUE);
			tft.drawRectangle(3, 167, 174, 196, COLOR_WHITE);
			if (RightSt > 1000)
			{
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
				tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
				con = 1;
				Defrost();
			}
		}
		else if (UD == 3) // fan speed
		{
			tft.drawRectangle(3, 167, 174, 196, COLOR_BLUE);
			tft.drawRectangle(100, 127, 135, 156, COLOR_WHITE);
			if (LeftSt > 1000)
			{
				tft.fillRectangle(145, 170, 160, 192, COLOR_WHITE);
				f--;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
			}
			if (RightSt > 1000)
			{
				tft.fillRectangle(145, 170, 160, 192, COLOR_WHITE);
				f++;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
			}
			if (f < 1)
			{
				f = 1;
			}
			if (f > 8)
			{
				f = 8;
			}
		}
		Setting();
		Fanspeed(f);
		if (EnterSt > 1000)
		{
			p = 1;
			analogWrite(Buzz, 255);
			delay(150);
			tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
			analogWrite(Buzz, 0);
		}
	}
	else if (p == 1) // wait for temp
	{
		if (millis() > Tc)
		{
			Tc = 1000 + millis();
			sensors.requestTemperatures();
			tempC = (sensors.getTempCByIndex(0));
			if (R1 == 0)
			{
				NT = tempC - C;
				R1 = 1;
			}
		}

		Serial.println(tempC);
		if (tempC > C)
		{
			tft.setGFXFont(&FreeSans12pt7b);
			tft.drawGFXText(18, 60, "Wait for temp", COLOR_BLUE);
			tft.drawGFXText(25, 110, "Remaining", COLOR_BLUE);
			tft.drawGFXText(110, 160, "Min", COLOR_BLUE);
			if (aa == 0)
			{
				tft.drawGFXText(60, 160, "5", COLOR_BLACK);
				aa = 1;
			}
			if (millis() > Tb)
			{
				Tb = 1000 + millis();
				Ta[a] = tempC;
				a++;
			}
			if (a == 5)
			{
				tft.fillRectangle(55, 140, 100, 165, COLOR_WHITE);
				ww = Ta[0] - Ta[4];
				ss = (NT * 5) / ww;
				Mi = ss / 60;
				if (Mi < 1)
				{
					Mi = 1;
				}
				if (ww == 0)
				{
					Mi = 99;
				}
				char buf5[4];
				sprintf (buf5, "%d", Mi);
				tft.drawGFXText(60, 160, buf5, COLOR_BLACK);
				a = 0;
			}
		}
		else if (tempC <= C)
		{
			p = 2;
		}
	}
	else if (p == 2) // Ready for operate
	{
		if (bb == 0)
		{
			tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
			bb = 1;
		}
		if (cc == 5)
		{
			analogWrite(Buzz, 255);
		}
		else if (cc == 100)
		{
			analogWrite(Buzz, 0);
		}
		else if (cc == 1000)
		{
			cc = 0;
		}
		cc++;
		if (millis() > Td)
		{
			Td = 1000 + millis();
			state = !state;
			if (state == 1)
			{
				tft.setGFXFont(&FreeSansBold18pt7b);
				tft.drawGFXText(30, 90, "Ready", COLOR_RED);
				tft.setGFXFont(&FreeSans12pt7b);
				tft.drawGFXText(25, 160, "Press Enter", COLOR_BLACK);
				digitalWrite(Ready, HIGH);
			}
			else
			{
				tft.fillRectangle(25, 40, 160, 130, COLOR_WHITE);
				digitalWrite(Ready, LOW);
			}
			TeMin = Min;
			TeC = C;
		}
		if (EnterSt > 1000)
		{
			digitalWrite(Ready, HIGH);
			digitalWrite(Out, HIGH);
			p = 3;
			Sec = 60;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
		}
	}
	else if (p == 3) // Operate
	{
		if (dd == 0)
		{
			tft.setGFXFont(&FreeSans12pt7b);
			tft.fillRectangle(0, 35, 176, 220, COLOR_WHITE);
			tft.fillRectangle(0, 0, 176, 34, COLOR_BLUE);
			tft.drawGFXText(45, 25, "Process", COLOR_WHITE);
			tft.drawGFXText(5, 80, "remaining time", COLOR_BLACK);
			tft.drawGFXText(120, 130, "Min", COLOR_BLACK);
			tft.drawGFXText(5, 190, "Fan speed :", COLOR_BLACK);
			dd = 1;
		}
		tft.drawGFXText(60, 130, ":", COLOR_BLACK);
		char buf8[4];
		sprintf (buf8, "%d", f);
		tft.drawGFXText(145, 190, buf8, COLOR_BLACK);
		if (Min - 1 > 9)
		{
			char buf6[3];
			sprintf (buf6, "%d", Min - 1);
			tft.drawGFXText(30, 130, buf6, COLOR_BLACK);
		}
		else
		{
			char buf6[3];
			sprintf (buf6, "%d", Min - 1);
			tft.drawGFXText(43, 130, buf6, COLOR_BLACK);
		}
		if (Sec > 9)
		{
			char buf7[4];
			sprintf (buf7, "%d", Sec);
			tft.drawGFXText(70, 130, buf7, COLOR_BLACK);
		}
		else
		{
			tft.drawGFXText(70, 130, "0", COLOR_BLACK);
			char buf7[4];
			sprintf (buf7, "%d", Sec);
			tft.drawGFXText(85, 130, buf7, COLOR_BLACK);
		}

		if (millis() > sttime)
		{
			sttime = 1000 + millis();
			Sec--;
			tft.fillRectangle(65, 110, 100, 130, COLOR_WHITE);
			if (zz < 3)
			{
				zz++;
			}
		}
		if (Sec == 0 && Min - 1 == 0)
		{
			p = 5;
		}
		if (Sec == 0)
		{
			Min--;
			Sec = 59;
			tft.fillRectangle(25, 100, 55, 130, COLOR_WHITE);
		}
		if (LeftSt > 1000)
		{
			tft.fillRectangle(145, 170, 160, 192, COLOR_WHITE);
			f--;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
		}
		if (RightSt > 1000)
		{
			tft.fillRectangle(145, 170, 160, 192, COLOR_WHITE);
			f++;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
		}
		if (f < 1)
		{
			f = 1;
		}
		if (f > 8)
		{
			f = 8;
		}
		Fanspeed(f);
		if (DownSt > 1000)
		{
			Min++;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
			tft.fillRectangle(25, 100, 55, 130, COLOR_WHITE);
		}
		if (UpSt > 1000)
		{
			Min--;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
			tft.fillRectangle(25, 100, 55, 130, COLOR_WHITE);
			if (Min == 2)
			{
				Min = 1;
			}
		}
		if (EnterSt > 1000 && zz == 3)
		{
			digitalWrite(Out, LOW);
			p = 4;
			ff = 0;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
			tft.fillRectangle(0, 35, 176, 220, COLOR_WHITE);
			tft.fillRectangle(0, 0, 176, 34, COLOR_BLUE);
		}
	}
	else if (p == 4) // Operate menu
	{
		if (UpSt > 1000)
		{
			ee++;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
		}
		if (DownSt > 1000)
		{
			ee--;
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
		}
		if (ee < 0 || ee > 2)
		{
			ee = 0;
		}
		tft.setGFXFont(&FreeSans12pt7b);
		tft.drawGFXText(55, 25, "Pause", COLOR_WHITE);
		tft.drawGFXText(50, 90, "Resume", COLOR_BLACK);
		tft.drawGFXText(50, 130, "Stop", COLOR_BLACK);
		tft.drawGFXText(50, 170, "Defrost", COLOR_BLACK);
		if (ee == 0)
		{
			tft.drawRectangle(45, 65, 150, 95, COLOR_BLUE);
			tft.drawRectangle(45, 105, 150, 135, COLOR_WHITE);
			tft.drawRectangle(45, 145, 150, 175, COLOR_WHITE);
			if (ff < 5)
			{
				ff++;
			}
			if (EnterSt > 1000 && ff == 5)
			{
				p = 3;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
				tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
				dd = 0;
				digitalWrite(Out, HIGH);
			}
		}
		else if (ee == 1)
		{
			tft.drawRectangle(45, 105, 150, 135, COLOR_BLUE);
			tft.drawRectangle(45, 65, 150, 95, COLOR_WHITE);
			tft.drawRectangle(45, 145, 150, 175, COLOR_WHITE);
			if (EnterSt > 1000)
			{
				p = 5;
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
			}
		}
		else if (ee == 2)
		{
			tft.drawRectangle(45, 145, 150, 175, COLOR_BLUE);
			tft.drawRectangle(45, 105, 150, 135, COLOR_WHITE);
			if (EnterSt > 1000)
			{
				analogWrite(Buzz, 255);
				delay(150);
				analogWrite(Buzz, 0);
				p = 6;
				tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
			}
		}
	}
	else if (p == 5) // Operate finish
	{
		if (R2 == 0)
		{
			tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
			R2 = 1;
		}
		tft.setGFXFont(&FreeSansBold18pt7b);
		tft.drawGFXText(30, 130, "Finish", COLOR_RED);
		analogWrite(Buzz, 255);
		delay(1500);
		analogWrite(Buzz, 0);
		p = 0;
		C = TeC;
		dd = 0;
		aa = 0;
		bb = 0;
		zz = 0;
		bg = 0;
		Min = TeMin;
		R2 = 0;
		ee = 0;
		ff = 0;
		digitalWrite(Ready, LOW);
		digitalWrite(Out, LOW);
	}
	else if (p == 6) // Defrost
	{
		digitalWrite(De, HIGH);
		if (millis() > yy)
		{
			yy = 1000 + millis();
			state = !state;
			if (state == 1)
			{
				tft.setGFXFont(&FreeSansBold18pt7b);
				tft.drawGFXText(27, 90, "Defrost", COLOR_CYAN);
			}
			else
			{
				tft.fillRectangle(25, 40, 160, 130, COLOR_WHITE);
			}
		}
		tft.setGFXFont(&FreeSans12pt7b);
		tft.drawGFXText(50, 160, "cencel", COLOR_BLACK);
		tft.drawRectangle(45, 140, 135, 165, COLOR_BLUE);
		if (EnterSt > 1000)
		{
			analogWrite(Buzz, 255);
			delay(150);
			analogWrite(Buzz, 0);
			p = 4;
			tft.fillRectangle(0, 35, 176, 220, COLOR_WHITE);
			tft.fillRectangle(0, 0, 176, 34, COLOR_BLUE);
			digitalWrite(De, LOW);
			ee = 0;
			ff = 0;
		}
	}
	delay(1);
}
void Defrost()
{
	while (con == 1)
	{
		LeftSt = analogRead(Left);
		digitalWrite(De, HIGH);
		EnterSt = analogRead(Enter);
		if (millis() > Ti)
		{
			Ti = 1000 + millis();
			state = !state;
			if (state == 1)
			{
				tft.setGFXFont(&FreeSansBold18pt7b);
				tft.drawGFXText(27, 90, "Defrost", COLOR_CYAN);
			}
			else
			{
				tft.fillRectangle(25, 40, 160, 130, COLOR_WHITE);
			}
		}
		tft.setGFXFont(&FreeSans12pt7b);
		tft.drawGFXText(103, 160, "ON", COLOR_GREEN);
		tft.drawRectangle(95, 140, 145, 165, COLOR_BLUE);
		tft.drawGFXText(25, 160, "OFF /", COLOR_RED);
		if (LeftSt > 1000)
		{
			analogWrite(Buzz, 255);
			digitalWrite(De, LOW);
			delay(150);
			analogWrite(Buzz, 0);
			con = 0;
			bg = 0;
			tft.fillRectangle(0, 0, 176, 220, COLOR_WHITE);
		}
	}
}
void Fanspeed(int f)
{
	if (f == 1)
	{
		one();
	}
	else if (f == 2)
	{
		two();
	}
	else if (f == 3)
	{
		three();
	}
	else if (f == 4)
	{
		four();
	}
	else if (f == 5)
	{
		five();
	}
	else if (f == 6)
	{
		six();
	}
	else if (f == 7)
	{
		seven();
	}
	else if (f == 8)
	{
		eight();
	}
}
void one()
{
	digitalWrite(SC, LOW);
	digitalWrite(SB, LOW);
	digitalWrite(SA, LOW);
}
void two()
{
	digitalWrite(SC, LOW);
	digitalWrite(SB, LOW);
	digitalWrite(SA, HIGH);
}
void three()
{
	digitalWrite(SC, LOW);
	digitalWrite(SB, HIGH);
	digitalWrite(SA, LOW);
}
void four()
{
	digitalWrite(SC, LOW);
	digitalWrite(SB, HIGH);
	digitalWrite(SA, HIGH);
}
void five()
{
	digitalWrite(SC, HIGH);
	digitalWrite(SB, LOW);
	digitalWrite(SA, LOW);
}
void six()
{
	digitalWrite(SC, HIGH);
	digitalWrite(SB, LOW);
	digitalWrite(SA, HIGH);
}
void seven()
{
	digitalWrite(SC, HIGH);
	digitalWrite(SB, HIGH);
	digitalWrite(SA, LOW);
}
void eight()
{
	digitalWrite(SC, HIGH);
	digitalWrite(SB, HIGH);
	digitalWrite(SA, HIGH);
}
void Setting()
{
	if (bg == 0)
	{
		tft.fillRectangle(0, 0, 176, 34, COLOR_BLUE);
		tft.fillRectangle(0, 35, 176, 220, COLOR_WHITE);
		bg = 1;
	}
	tft.setGFXFont(&FreeSans12pt7b);
	tft.drawGFXText(45, 25, "Setting", COLOR_WHITE);
	tft.drawGFXText(5, 70, "Time :", COLOR_BLACK);
	char buf2[3];
	sprintf (buf2, "%d", Min);
	tft.drawGFXText(90, 70, buf2, COLOR_BLACK);
	tft.drawGFXText(130, 70, "Min", COLOR_BLACK);
	tft.drawGFXText(5, 110, "Temp :", COLOR_BLACK);
	char buf3[4];
	sprintf (buf3, "%d", C);
	tft.drawGFXText(90, 110, buf3, COLOR_BLACK);
	tft.drawGFXText(135, 110, "*C", COLOR_BLACK);
	tft.drawGFXText(5, 150, "Defrost :", COLOR_BLACK);
	tft.drawGFXText(104, 150, "off", COLOR_RED);
	tft.drawGFXText(136, 150, "/on", COLOR_GREEN);
	tft.drawGFXText(5, 190, "Fan speed :", COLOR_BLACK);
	char buf1[4];
	sprintf (buf1, "%d", f);
	tft.drawGFXText(145, 190, buf1, COLOR_BLACK);
}
