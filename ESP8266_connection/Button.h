

struct Button{

  String name;
  String route;
  String state;
  int output_pin;
  String ToggleButtonElement(Button* btn) {


  String btn_next_state = "";
  if (header.indexOf("GET /" + (*btn).route + "/on" ) >= 0) {
    (*btn).state = "on";
    digitalWrite((*btn).output_pin, HIGH);

  }
  else if (header.indexOf("GET /" + (*btn).route + "/off" ) >= 0) {
    (*btn).state = "off";
    digitalWrite((*btn).output_pin, LOW);
  }

  if ((*btn).state == "on") {
    btn_next_state = "off";
  }
  else {
    btn_next_state = "on";
  }

  return "<p><a href='/" + (*btn).route + "/" + btn_next_state + "'><button class='btn general-text btn-" + (*btn).state + "'>" + (*btn).name + "</button></a></p>";
}
void Initialize(String name , String route , String state , int output_pin , Button* btn){
    (*btn).name = name;
    (*btn).route = route;
    (*btn).state = state;
    (*btn).output_pin = output_pin;
    pinMode(output_pin , OUTPUT);
    digitalWrite(output_pin, LOW);
}
} ;