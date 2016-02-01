Questions? Contact powertechsupport@exar.com

Steps to setup Exar's XR programmer code library:

1. Download and install the Arduino IDE from http://arduino.cc/en/Main/Software (Arduino IDE 1.6.7 is the latest at this point).

2. Click on "UnivPMIC" on Exar's GitHub page - https://github.com/exarcorp and download the zip file.

3. Extract the contents of the zip file in your Arduino library folder (C:\Users\Your_User_Name\Documents\Arduino\libraries on Windows 7).

4. Rename the folder to "UnivPMIC" (Remove the "-master" at the end of the folder name).

5. Open the Arduino IDE, go to File->Examples->UnivPMIC->Hex_to_EEPROM_Flash. Upload the code to your Arduino Uno board by clicking the arrow icon "->".

6. Go to the XR programmer folder inside the extracted UnivPMIC folder (..../Arduino/libraries/UnivPMIC/XR Programmer). Double-click on XR_Programmer.exe.

7. Click on the browse button to select a hex file on your machine to be programmed into the flash memory of the Exar board.

8. Once the file is selected, click on the "Program the Arduino EEPROM" button. This will program the contents of the hex file into the EEPROM of the Arduino Uno board. A pop up message will be displayed when the programming is completed.

9. After the file has been programmed in the EEPROM, press the button on the shield board to program the flash of the Exar board.

10. The PRGM_DONE led blinks a couple of times to indicate that programming has started.

11. After programming is done, either the PRGM_DONE light or the PRGM_ERROR light comes on. 

12. The PRGM_DONE light means that the chip was programmed successfully. The PRGM_ERROR light means there was an error during programming. In case both lights come on, it means either no hex file was loaded in the Arduino board using the XR Programmer or the Arduino board couldn't allocate enough memory to hold the contents of the hex file. In case of error, try resetting the board and run the XR_Programmer.exe over the command line to see debug messages.

13. The Arduino EEPROM can be erased by pressing the "Erase the Arduino EEPROM" button in the XR Programmer GUI. When the EEPROM is erased, trying to program the Exar board by pressing the button on the shield board would make both the led lights come on, which means a new hex file needs to be loaded using XR Programmer.


