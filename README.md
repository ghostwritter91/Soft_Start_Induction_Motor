# PCB_Soft_Start_Induction_Motor
Circuit for driving soft start for induction motor 800 W. Device has load detection circuit so it detects when pump is starting and starts launching procedure. Starting procedure is indicated on 5 diodes. Diodes are turned on in sequence from green to red (the more LEDs on, the higher the engine speed). 5 V section provided by USB input (used also for programming) is completly insulated from 230 V by optocouplers.

When motor is of, diodes indicate starting time of motor. There are 5 times to choose (plese see MOTOR_ThresholdsDuration_s 2D array). Time is selected by the potentiometer (knob):
- if one diode is turned on, motor starts slow (knob sets maximum to the left side) - this mode is intended to use when there is no power supply and motor works on UPS supply. 
- when knob is maximum turned to the right side all diodes are turned on - motor starts immediately. Use it for normal operation when pump is powered from standard 230 V power supply.

Device has also trigger button which starts launching sequence - when load detection circuit does not work properly or for tests.

<img width="357" alt="2023-02-05 14_59_31-CN=Microsoft Windows, O=Microsoft Corporation, L=Redmond, S=Washington, C=US" src="https://user-images.githubusercontent.com/33268158/216824213-160bc77a-7b4a-4455-8cff-b172cb1573e3.png">

## IDE and EDA
KiCAD 5.1.5 and Microchip studio 7.0

## Hardware
Hardware based on [AVT1007](https://sklep.avt.pl/avt1007.html) and [ATTINY104 XPLAINED NANO EVALUATION KIT](https://www.microchip.com/en-us/development-tool/attiny104-xnano). Potentiometer on AVT1007 is set to minimum (triac almost closed). 10uF capacitor was added in parallel to C3 capacitor. PWM from Attiny104 is used to charge the 10 uF capacitor and change the speed of the motor. U2008 IC has a U/I phases shift compensation and triggering pulse is generated when the voltage on C3 reaches the voltage on C2. Additional potentiometer on AVT1007 kit is used to control the speed of charging C2 capacitor.

![obraz](https://user-images.githubusercontent.com/33268158/216825864-0ec04b10-0045-41b2-99f3-120b53c3d526.png)

## Knowing issues
- Sometimes when device starts motor again and again, 10 uF capacitor stays charged and motor speed is set to high value. After restart of the device there is no problem.
