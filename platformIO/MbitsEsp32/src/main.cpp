#include <Button.h>
#include "remote_ai_client.hpp"
#include "matrix_plotter.hpp"
#include "vehicle_driver.hpp"
#include "communication_manager.hpp"

CommunicationManager cm;
Vehicle car{cm};

RemoteAIClient rai_client;
MatrixPlotter plotter;
String topic = "face";

void setup()
{
  cm.begin();
  rai_client = RemoteAIClient{"Cohen2", "25sep1963"};
  delay(1000);
  rai_client.connect_host("3.237.252.225", 3000);
  delay(1000);
  rai_client.add_topic("face");
  rai_client.add_topic("forward");
  rai_client.add_topic("backward");
}

void loop() {
  if (buttonA.isPressed() && !buttonB.isPressed()) {
      plotter.draw_smile();
      plotter.show_untill(5000);
  }
  if (buttonB.isPressed() && !buttonA.isPressed()) {
    plotter.ShowString("elecrow",plotter.blue());
  }
  String response = rai_client.wait_for_response(10);
  if(response.length() > 0){
    if(response.startsWith("face")){
        plotter.draw_smile();
        plotter.show_for_ms(1000);
    }
    else if(response.startsWith("forward")){
      car.fwd(2048);
      delay(1000);
      car.stop();
    }
    else if(response.startsWith("backward")){
      car.bwd(2048);
      delay(1000);
      car.stop();
    }
    else if(response.compareTo("ok")==0){
      car.blink_both();
      delay(500);
      car.blink_stop();
    }
  }
}

/*
void loop() {
  car.fwd(2048);
  delay(1000);
  car.stop();
  delay(1000);
  car.bwd(2048);
  delay(1000);
}
*/