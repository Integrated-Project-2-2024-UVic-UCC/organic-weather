#  ORGANIC WEATHER STATION
The "Organic Weather Station" blends botanical aesthetics with robotics, creating a responsive flower-like robot. Equipped with photodiodes, a stepper motor, and an ESP32, this design emulates the natural behavior of flower petals that open and close with changes in light levels. It demonstrates a harmonious integration of components, providing both environmental feedback and decorative value in indoor settings.

## Features
- **Adaptive Petal Movement**:  Responds to light intensity data from photodiodes, controlling the movement of its petals to mimic natural responses.
- **Environmental Feedback**: Responds to light intensity data from photodiodes, controlling the movement of its petals to mimic natural responses.
- **Remote Weather Updates**: Connects with a Telegram chat to retrieve and display weather conditions from any specified location.
- **Ambient LED Effects**: Incorporates LEDs within the petals that shift colors based on the ambient room temperature.

![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/planta_1.PNG)

If you are interested in carrying out this project, we invite you to the following sections!

## README STRUCTURE
- [Promotional Video](#promotional-video)
- [Materials](#materials)
- [Mechanical part](#mechanical-part)
    - [ 3D printed parts](#3d-printed-parts)
    - [Hardware parts](#hardware-parts)
- [Electrical Part](#electrical-part)
    - [Datasheets](#datasheets)
    - [Electric scheme](#electric-scheme)
- [Programming Part](#programming-part)

## PROMOTIONAL VIDEO
![Video Screenshot](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/ScreenShot.png)
[Promotional video](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/OrganicWeather_PromotionalVideo.mp4)
## MATERIALS
Before starting with the mechanical part, we will show you the list of materials that have been used and the links where you can buy it:
|Component|Description|Link|Price (€)|
|---|---|---|---|
|![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/assets/148632510/175b6df2-47b3-4d7a-a043-f7cd1067a346) |ESP32-DEVKITC-32UE, MAIN SHIELD |(https://www.digikey.es/es/products/detail/espressif-systems/ESP32-DEVKITC-32UE/12091813) |9,23 €|
|![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/assets/148632510/90ea6707-07f6-4620-b829-95b92183f802) |TFT LCD 3.2″ 240×320 RGB SPI Display with Touchscreen| https://protosupplies.com/product/tft-lcd-3-2-240x320-rgb-spi-display-with-touchscreen/ | 17,7 €|
|![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/assets/148632510/9e236e71-0359-471f-8cdb-d2269f496330) |Stepper motor 28BYJ-48 (5V) with ULN2003 driver|https://tienda.bricogeek.com/motores-paso-a-paso/969-motor-paso-a-paso-28byj-48-5v-con-driver-uln2003.html?gad_source=1&gclid=Cj0KCQiA84CvBhCaARIsAMkAvkLmYwUtcC3NNPF3OEbdK23PZQW-ia57QxV4Llk7v_9HG6Ht4EFABe0aAlZhEALw_wcB | 2,95 €|
|![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/assets/148632510/49f7c3ee-893f-472c-8295-112737e02a60) | DHT22 humidity and temperature sensor| https://eu.robotshop.com/es/products/dht22-temperature-humidity-sensor?%20_gl=1*x1xww6*_up*MQ..&gclid=Cj0KCQiAoeGuBhCBARIsAGfKY7zt4lGBBzL5W1FGmKmtYYcLwor4nTV0EWnDHFL52kem3NKnSJv13vYaAqKYEALw_wcB | 7,49 €|
|![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/assets/148632510/3bac220b-56c5-45f9-97d6-7bc834eaad6f) | SFH 203 P Photodiode | https://www.digikey.es/es/products/detail/ams-osram-usa-inc/SFH-203-P/1893875 | 1 - 0,92 € |
|![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/assets/148632510/537e5a60-5057-4fd6-8ab6-7c92300786e6) |WS2812B Module Circuit with RGB Led| https://www.shoptronica.com/led-bicolor-tricolor-rgb/3959-circuito-modulo-ws2812b-con-led-rgb-0689593949912.html?mot_tcid=8e6c0a71-d75a-4b1b-a4d5-eda81f9adca2#/63-opciones_color-blanco | 1 - 0,2610 €|

The datasheets for these components can be found in the following folder: [Datasheets](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/tree/main/Electronic/Datasheets)

## MECHANICAL PART
  ### 3D printed parts

After reviewing the description of the organic weather station and understanding its required functions and movements, this section will explain the mechanisms used to ensure the flowers open and close correctly each time.
Flower mechanism
As a first option, it was proposed to move the petals using four gears, each connected to a petal, and a worm gear in the center, connected to the motor. This mechanism was to allow the petals to open and close depending on the light intensity of the environment. The worm gear turned and had to engage with the gears to make them rotate, which worked in the simulation. However, when printed, the system generated too much friction, which caused constant breakage and malfunction. Because of these friction and unreliability problems, the design was eventually discarded.
After this, a new design was started. This innovative design does not have gears or worm gear, in short, it has two pieces without considering the petals. The first piece will be a central piece, one end of which will be connected to the stepper motor that will carry out the movement and the other part is where the petals will be hooked. This part will be like a crown where the petals will be placed, that is, they will fit together, allowing the petal to have complete movement. We could imagine the movement as that of a hinge. The second piece will be both functional and visual. We can see that visually it will be a calyx of a flower and inside it will have the shape of a coup. Its function will be that when the central piece lowers, the petals, when resting on the calyx, begin to close on the inside of the cup, so when the central piece has reached its minimum, the flower is completely closed. 

![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/planta_2.PNG)
|Component|Description|3D part|
|---|---|---|
| POT | Pot of the plant, inside it has all the electronic components | ![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/pot.png) |
| INNER PART | The Main bar, to move the petals of the flower, is connected to the stepper motor | ![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/innerpart.png)|
| FLOWER PETAL | Six of these complete the flower, with the hinge on the bottom it connects to the Inner Part, to be able to open and close | ![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/lower_petal.png) |
| LEAVES | The greener is the support of the leaf that it is attached to the stem of the plant. We have two, one on each side, the left and the right. | ![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/leaves.png) |

These are the parts of the mechanism inside the pot, that we use:

![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Images/esquematic%20movement.png)

  ### Hardware parts
  
## ELECTRICAL PART
  ### Datasheets
  You can find the datasheets for the following components in the [datasheets folder](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/tree/main/Electronic/Datasheets):
  - ESP32
  - Photodiodes
  - LEDs RGB
  - DHT22 humidity and temperature sensor
  ### Electric scheme
  ![Schematic_Weather_Station](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Electronic/Schematics/Schematic_Weather_Station))
  [![Electric Scheme]([https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Electronic/Schematics/Schematic_Weather_Station.png](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Electronic/Schematics/Schematic_Integrated%20Project_2024-04-24.png))](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/tree/main/Electronic/Schematics)

  
## PROGRAMMING PART
The libraries used are located in the Code folder, as a folder called LIBRARIES. 
Here is explained synthesized the main workflow of the code. 

### Workflow of the HTTP Client 
![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Codes/HTTP%20client%20workflow.png?raw=true)

### Global/Loop workflow 
![image](https://github.com/Integrated-Project-2-2024-UVic-UCC/organic-weather/blob/main/Codes/global%20workflow.png)

### Setup
For inicialize the plant:
- Starts the screen with the welcome screen
- Reset the position of the stepper motor
- Starts the WiFi connection
- Configure the Real Time Clock of the ESP32
- Do a first run of two main processes
  - Light sensing and motor position
  - Data adquire and data display

### Loop
When the system has been started up correctly, the main loop only execute two subprocesses:
- Light sensing and motor position
- Data aquire and data display
  
This two subprocesses have diferent delay time, controlled by comparison of the millis() function.
For the aperture of the leaves needs a fast frequency, is set to 0,5 seconds.
For the data aquire and data display doesn't need a high frequency because the data may not vary significantly. The delay is set to 30 seconds. 

