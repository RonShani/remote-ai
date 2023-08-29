#include <Button.h>
#include "remote_ai_client.hpp"
#include "matrix_plotter.hpp"

RemoteAIClient rai_client;
MatrixPlotter plotter;
String topic = "face";

void setup()
{
  Serial.begin(115200);
  rai_client = RemoteAIClient{"Cohen2", "25sep1963"};
  delay(1000);
  rai_client.connect_host("3.235.42.183", 3000);
  delay(1000);
  rai_client.add_topic("face");
}

void loop() {
  if (buttonA.isPressed() && !buttonB.isPressed()) {
    plotter.ShowString("!",plotter.green());
  }
  if (buttonB.isPressed() && !buttonA.isPressed()) {
    plotter.ShowString("elecrow",plotter.blue());
  }
  String response = rai_client.wait_for_response(10);
  if(response.length() > 0){
    if(response.startsWith("face")){
        plotter.draw_smile();
        plotter.show_for_ms(1000);
    } else {
      Serial.println(response);
    }
  }
}