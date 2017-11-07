# BAoBOS
BAoBOS is a robotic arm that executes, in real life, all of the movements done via the BAoBUI interface.  

[![BAOBOS VIDEO](https://imgur.com/afMjANT.png)](https://vimeo.com/236781582)

## Hardware
BAoBOS is powered by 5 servo-motors whose movement is handled by an Arduino-Uno board.
Each servo motors pushed and pulls a syringe acting as a simple hydraulic piston.
Syringes connected to the arm itself are thus extended or contracted.

![alt text](https://i.imgur.com/A8EGFUp.jpg "BAoBOS complete view")

`Servo Motor #0` extends and retracts the magnet on the head of BAoBOS.
`Servo Motor #1` changes the z-axis angle that the magnet has with the base of the board.
`Servo Motor #2` extends the upper component of the robotic arm.
`Servo Motor #3` extends the middle component of the robotic arm.
`Servo Motor #4` rotates the base of the robot and, thus, the robot itself.

### Pressure Surfaces
BAoBOS has five distinct pressure surfaces, triggeable by placing on them objects made of conductable materials, such as metal.
During the demo with BAoBUI, a metal cube was used.  
The pressure surfaces are made out of interwoven `5V` and `GND` wires. Closing the circuit between these two wires will close the circuit signalling Arduino.  

### The Breadboard
The following is the wiring necessary to make BAoBOS work.  
2 external power suppplies of 7.5V (1000 mA) each are necessary to make the servo motors work properly under load.  
The control `PINS` of the servo motors are connected to `PINS 2-6` on the Arduino Uno.  
Analog `PINS A1-A5` are used to detect presssures of the Pressure Surfaces.  

![alt text](https://i.imgur.com/ZLAfAbN.png "BAoBOS breadboard")

### Schematic
Below is a simplified schematic view of the circuit.

![alt text](https://i.imgur.com/88pF45s.png "BAoBOS schematic")

## Port Communication
Communication to BAoBOS can occur via a Serial Port with Baud Rate of 115200. For this particular project the port is initialised by the BAoBUI user interface.  
Communication to BAoBOS occurs exchanging packages of 6 bytes of input, in the following form:
  
`In-bound communication:` 
 * `byte #0` Sender ID  
 * `bytes #1-5` Servo Motors #0-4 Angles  
 
 When BAoBUI is sending data, it's sender ID is 200.  
 Servo Motor angles can be in range 0°-180°.  
 Negative values have undefined behaviour as all data is cast to `uint8_t`.  
   
 `Out-bound communication:` 
 * `byte #0` Sender ID  
 * `bytes #1-5` Pressure Surfaces #0-4 States  
 
 When BAoBOS is sending data, it's sender ID is 201.
 Pressure Surface states can either be 0 for not-pressed, or 1 for pressed.

## Extra Images

![alt text](https://i.imgur.com/jYU34C7.jpg "BAoBOS frontal view")

