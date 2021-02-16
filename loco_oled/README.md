# loco_oled
This is the code responsible for acting as a driver for the LoCO OLED.

You should look at the [loco_menu](https://github.com/loco-auv/loco_menu) package for more details on message format for running messages to the menu node.

Currently, the format is a std_msgs/String message, containing the first 4 characters "LIT;" to signify that it is meant to be interpreted literally.
Any following characters are printed on a line by line basis, with lines currently delimited by ';' characters (this might change to newlines, though).


## Arduino Board and Connection.
 We are currently using the Arduino MKRZero for this task, given its huge SRAM size compared to other boards. As far as connections....

 | OLED Pin | Arduino MKR Zero Pin|
 | ------------- | -------------- |
 | 1 | VCC |
 | 2 | GND |
 | 4 | 6 |
 | 7 | 9 |
 | 8 | 8 |
 | 15 | 11 |
 | 16 | 12 |
