# BAoBOS
BAoBOS is a robotic arm that executes, in real life, all of the movements done via the BAoBUI interface.

## Hardware
BAoBOS is powered by 5 servo-motors whose movement is handled by an Arduino-Uno board.
Each servo motors pushed and pulls a syringe acting as a simple hydraulic piston.
Syringes connected to the arm itself are thus extended or contracted.

![alt text](https://i.imgur.com/A8EGFUp.jpg "BAoBOS")

`Servo Motor #0` extends and retracts the magnet on the head of BAoBOS.
`Servo Motor #1` changes the z-axis angle that the magnet has with the base of the board.
`Servo Motor #2` extends the upper component of the robotic arm.
`Servo Motor #3` extends the middle component of the robotic arm.
`Servo Motor #4` rotates the base of the robot and, thus, the robot itself.

## Pressure Surfaces
BAoBOS has five distinct pressure surfaces, triggeable by placing on them objects made of conductable materials, such as metal.
During the demo with BAoBUI, a metal cube was used.
The pressure surfaces are made


![alt text](https://i.imgur.com/R3f6FqE.png "BAoBOS")

![alt text](https://i.imgur.com/jYU34C7.jpg "BAoBOS")

