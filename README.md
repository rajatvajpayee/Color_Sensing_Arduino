# 6-DoF Robot using Arduino
Algorithm to design a robot which can sense object's color and pick it up then place it on a specific position according to the sensed color.
It also has ability to stop the robotic arm if there is an object in front of it.

## Process
Servo motor will start from the initial position and hence the robotic arm will move. If it senses an object it will automatically
stop at that place. After that, it will sense the object's color. Then it will pick the object and place it the specified place.

### Object_Sensing
* A proximity sensor has used for the detection of the object
* Circuit breaks at threshold value of proximity sensor
* As a result motor stops rotating

### Color_Sensing
* R,G,B LED's and LDR are used for the color sensing 
* After stoping at the object, R,G,B LED's will start emmiting light respectively. LDR will store the values for all 
the three values
* Color corresponding to the minimum value obtained from LDR will be our object's color

### Object_Placing
* After color sensing, electromagnet present on the arm will turn on
* Then it picks it up. Finally, arm will rotate and place it on the specified location

### Calibration
* It is necessary to calibrate the R,G,B values before starting the experiment
* Here *Auto* and *Manual* Calibration are provided in the code
