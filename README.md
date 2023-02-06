# Birds Project


> ## Setup Wifi Settings

go to the [Wifi Settings](./ESP8266_connection/WiFi_settings.h) file and setup the network settings:
```c
// Enter your wifi network name and Wifi Password
const char* ssid = "network-name";
const char* password = "network-password";
```


> ## Creating an HTML file

### **Button**

1. Declare new `Button` variable at the main script:
```c
Button my_button;
```
2. Initialize the button at the setup function:
```c
// The Initialize method takes the button text, route to the button,
// initial state ("off" or "on") , pin number and an address of itself

my_button.Initialize("Button text", "button-route" , "off" ,D3 , &my_button);
```

3. Add the button to the `HTML_BODY()` funciton:

```c
my_button.ToggleButtonElement(&my_button) +
```


### **Pump**

1. Declare new `Pump` variable at the main script:
```c
Button my_pump;
```
2. Initialize the button at the setup function:
```c
// The pump Initialize with the button text , the route, 
// initial state ("off" or "on") , pin number , day freq., pump time
// and an address to the pump object
my_pump.Initialize("Pump Name" , "pump-route" , "off" ,D5 ,5 , 1 , &my_pump);
```

3. Add the button to the `HTML_BODY()` funciton:

```c
my_pump.PumpElement(&my_pump) +
```


