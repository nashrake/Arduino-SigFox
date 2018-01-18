## **Arduino MKRFOX1200 SigFox**

### **First Hand Setup** [For Windows]:

1. Download Arduino IDE 1.8.5 or latest.
2. Install board MKRFOX1200 in the IDE from Tools--> Board Manager--> Type 'MKRFOX1200' --> Install.
3. Programmer Selection --> Atmel ICE.
4. Libraries to Install => Sketch --> Include Library --> Manage Library --> Install `SigFox` && `Arduino Low Power` && `RTC Zero` libraries.
5. Board is ready for work!

#####  First configuration  ||  Linux Setup  ||  example codes  see on this link [Gearing Arduino] (https://www.arduino.cc/en/Guide/MKRFox1200)

**Note**: If reporting error repeatedly `Couldn't find a Board on the selected port. Check that you have the correct port selected.`

	--> Try the device connecting to a different COM port.
	--> !ok --> double click the reset button on the board.
	--> Open new IDE with a basic code eg:Blink.
	--> Choose the COM port which is different from usual.
	--> Upload the code.
	--> Once done, go back to the IDE which you were working on, choose the usual COMx port --> upload [May sounds crazy, But will work!].

**Source**: arduino forum 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**********~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### **SigFox_Potentiometer.ino**

* Code to measure the I/Os of Arduino and sending it to SigFox backend Sever.
* To measure battery voltage. 

### **Circuit Connection**:
    

| Device | Potentiometer | Arduino |
| ------ | ------------- | ------- |
|  Pin   |      Out      |    A0   |
|  Pin   | LED(optional) |    A2   |
|  Pin   |      Vcc      |    Vcc or (Manual Digital input)  |
|  Pin   |      Gnd      |    Gnd  |


### **Payload Format**:

|    Pin Usage   | Pin to Read  |  Bit Length  |
| -------------- | ------------ | ------------ |
| AnalogPin 0    |     A0       |     10       |
| AnalogPin 1    |     A1       |     10       |
| AnalogPin 2    |     A2       |     10       |
| AnalogPin 3    |     A3       |     10       |
| AnalogPin 4    |     A4       |     10       |
| AnalogPin 5    |     A5       |     10       |
| AnalogPin 6    |     A6       |     10       |
| DigitalPin 0-7 |     0-7      |1 bit for each|
|    Battery     | ADC_BATTERY  |     16       |


#### **For Example**

Payload format:  ***3ff5f937240d5370f4c00003***

##### For decoding battery:

* Voltage x ( 5015/1023 )

#### Note:

Digital pin 6 in the program is used to power the Vcc of potentiometer for the case of power consumption.

P.S: 5.015v being maximum for 1023 analog value maximum.

#### For Battery Life calculation:

Have to need 

* mAH capacity of battery.

* mA when sleep.

* mA when awake.

* How many times awake per hour.

* Duration of wake period.


[Calculate battery consumption] (http://oregonembedded.com/batterycalc.htm)


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**********~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
