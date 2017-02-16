# Adruino_LCD_MENU README

Original library: By David Andrews - May 6, 2012

Modified by Tweep, February 2017

License: Modified BSD (See complete license at end)

Purpose: To shorten the time to get an Adriuno project running using a menuing system.


Changes : 
 - new constructor added which allows to enable / disable the execution of callback functions when the root 
   menu has been selected.  

    const bool doRootAction = false;
    MenuLCD g_menuLCD( LCDRS, LCDE, LCDD4, LCDD5, LCDD6, LCDD7, 16, 2);
    MenuManager g_menuManager( &g_menuLCD, doRootAction); 


Getting Started: Open the Arduino_Menu_LCD.ino sketch in the Arduino IDE.  Upload the sketch.  In the serial window, send the following keys to control the menu:

u - move UP
d - move DOWN
s - Select a menu option
b - move back in the menu tree

The idea is that you can invoke the menu system when appropriate, and have the Menu code drive all the navigation by piping your hardware inputs into the appropriate call in MenuManager.  When a menu item is selected that has an action associated with it, the menu system calls your callback so you can accomplish whatever the menu item is supposed to do.

This library builds on top of the Liquid Crystal library for doing LCD output.  On a new system, you should verify that you can create simple outputs using the Liquid Crystal library first, then move to this library to do your menuing.

The built-in actions right now are:

Back - Add a menu item labled "Back" and pass it the MenuEntry_BackCallbackFunc pointer, and you now have a Back menu entry (in case you don't have a hardware back button)

BoolTrue/False - These set a boolean to true or false so you can have off/on functionality easily.  This callback needs some more thinking and work right now to provide user feedback that the option was selected.


In addition, there is a MenuIntHelper class that assists in taking an integer input and setting a value. (See MenuIntHelper.h)


/*
 Arduino_LCD_Menu Library
Copyright Dustin Andrews, David Andrews 2012 
Licensed under the follwing license:

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution. 
The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
