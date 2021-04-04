// This code is a part of the LoCO AUV project.
// Copyright (C) The Regents of the University of Minnesota

// Maintainer: Junaed Sattar <junaed@umn.edu> and the Interactive Robotics and Vision Laboratory

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#define USE_USBCON // This defines the ROS serial to use the USB connector rather than pins 13 & 14 on the MKRZero. Remove this if you're actually using those.
#include <ros.h>
#include <std_msgs/String.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1325.h>


//LoCO LOGO
// 'loco', 128x64px
const unsigned char loco [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x00, 0x00, 0x1f, 0x00, 0x7c, 0x00, 0x0e, 0x02, 0x03, 0x18, 0x0c, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x00, 0x00, 0x3f, 0xc0, 0xff, 0x00, 0x0f, 0x03, 0x03, 0x18, 0x0c, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x00, 0x00, 0x70, 0xe1, 0x83, 0x00, 0x0f, 0x03, 0x03, 0x18, 0x0c, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x00, 0x00, 0x60, 0x61, 0x81, 0x80, 0x1b, 0x03, 0x03, 0x0c, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x01, 0xf0, 0xc0, 0x23, 0x01, 0x80, 0x19, 0x83, 0x03, 0x0c, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x03, 0xf8, 0xc0, 0x03, 0x01, 0x80, 0x19, 0x83, 0x03, 0x0c, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x03, 0x0c, 0xc0, 0x03, 0x01, 0x80, 0x31, 0x83, 0x03, 0x04, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x06, 0x0c, 0xc0, 0x03, 0x01, 0x80, 0x30, 0xc3, 0x03, 0x06, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x06, 0x04, 0xc0, 0x03, 0x01, 0x80, 0x70, 0xc3, 0x03, 0x06, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x06, 0x06, 0xc0, 0x03, 0x01, 0x80, 0x60, 0xc3, 0x03, 0x02, 0x20, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x06, 0x04, 0xc0, 0x23, 0x01, 0x80, 0x7f, 0xe3, 0x03, 0x03, 0x60, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x06, 0x0c, 0x60, 0x61, 0x81, 0x80, 0xff, 0xe3, 0x03, 0x03, 0x60, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x03, 0x0c, 0x70, 0xe1, 0x83, 0x00, 0xc0, 0x33, 0x87, 0x03, 0x40, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0xe3, 0xf8, 0x3f, 0xc0, 0xff, 0x00, 0xc0, 0x31, 0xfe, 0x01, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0xe1, 0xf0, 0x1f, 0x00, 0x7c, 0x01, 0x80, 0x30, 0xfc, 0x01, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x10, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x11, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x32, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0e, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x06, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x15, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x14, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0e, 0x00, 0xe0, 0x00, 0x07, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0e, 0x03, 0xe0, 0x00, 0x07, 0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x07, 0x0c, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x07, 0xf2, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x07, 0xf1, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x07, 0xf1, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0e, 0x03, 0xe0, 0x00, 0x07, 0x0a, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0e, 0x00, 0xe0, 0x00, 0x07, 0x04, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0e, 0x00, 0xe0, 0x00, 0x07, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x10, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x19, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x25, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x26, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x2e, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x11, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x11, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// If using software SPI, define CLK and MOSI
#define OLED_CLK 9
#define OLED_MOSI 8

// These are needed for both hardware & softare SPI
#define OLED_CS 11
#define OLED_RESET 12
#define OLED_DC 6

//Splits lines in literal mode. Should be a newline, probably.
char delimiter = ';';

// this is software SPI, slower but any pins
//Adafruit_SSD1325 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// this is for hardware SPI, fast! but fixed pins.
Adafruit_SSD1325 display(OLED_DC, OLED_RESET, OLED_CS);

// Ros handle define. You can set number of publishers and subscribers here if you need to.
ros::NodeHandle oled_node;

String last_message = "";

void messageCb(const std_msgs::String &msg){ 
    String message = String(msg.data);
    oled_node.loginfo("Echoing menu display message recieved: ");
    oled_node.loginfo(message.c_str());

    display.clearDisplay();

    bool literal = true;

    if(message == last_message){
        return;
    }
    last_message = message;
    
    String signifier = message.substring(0,4);
    oled_node.logdebug(signifier.c_str());
    literal = (signifier == "LIT;"); //Set literal equal to the value provided by comparing the signifier to the LIT signifier.

    if(literal){ //If the string message contains the signifier for a literal printing of the message, we go here.
      message = message.substring(4, message.length());
      oled_node.logdebug(message.c_str());
      
      String lines[6]; // We can only fit 6 lines on this OLED with the current font size, so going larger than that doesn't make sense.
      int base= 0, lc=0; //current base index for the next substring and line counter(lc).

      // Iterate through the delimiter characters.
      for(int i=0; i< message.length(); i++){
          if(message.charAt(i) == delimiter){
              lines[lc] = message.substring(base, i);
              base = (i+1);
              lc++;
          }

          if(lc == 5){
            break;
          }
      }
  
      if(base < message.length()){
        lines[lc] = message.substring(base,message.length());
      }
      
      display.setCursor(2,5);
  
      for(int k=0; k <= lc; k++){
          oled_node.logdebug(lines[k].c_str());
          display.println(lines[k].c_str());
          display.setCursor(2, (5+ ((k+1)*10)));
          display.display();
      } 
    }else{
      //Otherwise, we do other things. This will be used to send messages about drawing images and such.
      oled_node.logdebug("OLED in non-literal mode.");
    }
    
}

// Subscriber defined.
ros::Subscriber<std_msgs::String> sub("loco/menu_display", &messageCb);

void setup(){  
  // ros node
    oled_node.initNode();
    oled_node.subscribe(sub);
  
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(); // init done
    display.display(); // show Adafruit splashscreen
    delay(1000);
    display.clearDisplay();   // clears the screen and buffer
    display.setTextWrap(false);  // longer lines will wrap to the next line
    display.setTextColor(WHITE);
    loco_logo();
}

void loop(){  
    oled_node.spinOnce();
    delay(1);  
}

// display the Loco Logo
void loco_logo(){
    display.clearDisplay();
    display.drawBitmap(0,0, loco, 128, 64,1);
    display.display();
    delay(100);
}
