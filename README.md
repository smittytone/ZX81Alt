ZX81Alt
=======

This software runs on an Arduino Leonardo board connected to the keyboard of a Sinclair ZX81 home computer, connverting the device into a USB keyboard.

The program is derived from code written by Dave Curran of Tynemouth Software (http://blog.tynemouthsoftware.co.uk/2012/02/arduino-based-zx81-usb-keyboard.html) but modified to allow the entry of non-alphanumeric characters.

As per Dave's code, un-Shiifted the keyboard will produce lowercase letters and numbers. Holding down Shift will produce uppercase letters, and the cursor keys may be used. If the Function key is pressed while Shift is held down, the keyboard will go into 'symbol' mode. Pressing a subsequent key (without Shift) will produce the symbol shown on the key.

Note: whether you get the # or the Sterling symbol will depend upon the host system's own keyboard localisation settings.


The Keys

Please refer to a picture of the ZX81 keyboard (http://smittytone.files.wordpress.com/2014/02/p1020039.jpg?w=810)

Note _ indicates no key action, however the key E in Symbol mode WILL generate an underscore


No Shift

1 2 3 4 5 6 7 8 9 0
q w e r t y u i o p
a s d f g h j k l NewLine
z x c v b n m . Space


Shift

Escape _ _ _ _ LEFT DOWN UP RIGHT _ Backspace
Q W E R T Y U I O P
A S D F G H J K L SymbolSelect
Z X C V B N M , #


Symbol Mode

[ ] { } LEFT DOWN UP RIGHT ! Backspace
" _ underscore \ ^ % $ ( ) "
@ | ~ _ _ * - + = NewLine
: ; ? / * < > ' #

