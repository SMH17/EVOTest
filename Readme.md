Test olive oil quality using a laser diode with wavelength in 405nm-445nm range to check absorbance properties of chlorophyll and elicit its fluorescence and a TCS34725 sensor to analyze color spectrum, considering that chlorophyll fluorescence emission peaks around 680nm-700nm wavelength and chlorophyl absorbance peaks around 400nm-450nm wavelength.
In EVO oils the chlorophyll isn't destroyed by chemical processing and heat treatments, so it gives a clue of oil quality.
It is also important to note that this test cannot directly measure the oil acidity, however if the oil has high acidity level the chlorophyll will degrade faster over the time. 


NOTES:
- The project model and firmware code are just preliminary concept versions that could be improved refining the current analysis method and adding additional modules and test routines to evaluate other aspects.
- This code has been optimized for the use with STM32F030F4P6 and has been tested on STM32F030F4 Demo Board, in order to fit in small flash of this board the default Arduino USART serial support must be disabled.
- This code uses customized libraries so they aren’t available in standard repositories. Use the libraries provided e.g., copying them from libs folder to your Arduino\libraries folder (or configuring them properly for whatever IDE you use).
- Use low power <5mW laser diodes (as the one used for pointers), do not use high power lasers.
- Laser modules even at same wavelength and power level may have different lens configuration and collimation, so you need to do some calibration to get consistent results with different laser modules. 
- The stereolithographic files STL in 3D models libraries can be easily printed with common FDM 3D printer and filaments, however make sure to avoid to use transparent materials that could make filter external light in analysis chamber, this is very important. (I have used black PLA to print my prototype.)

Check PDF documentation for more details.


Version: 0.1a

Website:
www.silviomarano.tk
