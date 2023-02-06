

struct Pump{
  Button button;
  int pump_freq;
  int pump_duration;
  TimeStamp last_pump;
  String ControllerElement(String title , String inc_route , String dec_route , int value , String text) {
  return "<div class='controler'><p class='general-text float'>" + title + " :</p> <a class='float' href='" + inc_route + "'><button type='btn' name='button' class='btn-controler general-text'>+</button></a> <p class='general-text float'>" + String(value) + " " + text + "</p> <a class='float' href='" + dec_route + "'><button type='btn' name='button' class='btn-controler general-text'>-</button></a></div>";
  }
  String PumpElement(Pump* pump) {


  if (header.indexOf("GET /" + (*pump).button.route + "/dec/pumptime" ) >= 0) {
    if ((*pump).pump_duration != 0) {
      (*pump).pump_duration--;
    }

  }
  else if (header.indexOf("GET /" + (*pump).button.route + "/inc/pumptime" ) >= 0) {
    (*pump).pump_duration++;
  }
  else if (header.indexOf("GET /" + (*pump).button.route + "/dec/freq" ) >= 0) {
    if ((*pump).pump_freq != 0) {
      (*pump).pump_freq --;
    }

  }
  else if (header.indexOf("GET /" + (*pump).button.route + "/inc/freq" ) >= 0) {
    (*pump).pump_freq ++;
  }

  String HTML = "";
  HTML = HTML +
         (*pump).button.ToggleButtonElement(&((*pump).button)) +

         "<div class='pump-info'>" +

         ControllerElement("Once Every" , "/" + (*pump).button.route + "/inc/freq" , "/" + (*pump).button.route + "/dec/freq" , (*pump).pump_freq , "days") +
         ControllerElement("Pump time" , "/" + (*pump).button.route + "/inc/pumptime" , "/" + (*pump).button.route + "/dec/pumptime" , (*pump).pump_duration , "seconds") +

         "</div>";

  return HTML;
}
  void Initialize(String name , String route , String state , int output_pin ,int pump_freq , int pump_duration , Pump* pump){
    (*pump).button.name = name;
    (*pump).button.route = route;
    (*pump).button.state = state;
    (*pump).button.output_pin = output_pin;
    (*pump).pump_freq = pump_freq;
    (*pump).pump_duration = pump_duration;
    pinMode(output_pin , OUTPUT);
    digitalWrite(output_pin, LOW);
  }
  void activate(Pump* pump){
    digitalWrite((*pump).button.output_pin, HIGH);
    Serial.println((*pump).button.name);
    delay((*pump).pump_duration*1000);
    Serial.println("finished!");
    digitalWrite((*pump).button.output_pin, LOW);
  }

} Pump1 , Pump2;



