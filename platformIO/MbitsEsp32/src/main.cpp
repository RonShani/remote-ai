#include <vector>
#include "remote_ai_client.hpp"
#include "matrix_plotter.hpp"
#include "vehicle_driver.hpp"
#include "communication_manager.hpp"

CommunicationManager cm;
Vehicle car{cm};

RemoteAIClient rai_client;
MatrixPlotter plotter;

void blink_ok(Vehicle &a_car)
{
    a_car.blink_both();
    delay(500);
    a_car.blink_stop();
}

void setup()
{
  rai_client = RemoteAIClient{"Cohen2", "25sep1963"};
  rai_client.connect_host("44.203.26.117", 3000);
  cm.begin();
  std::vector<String> topics = {"face","forward","backward","left"};
  rai_client.topic_loader(topics, blink_ok, car);
}

void loop() {
  String response = rai_client.wait_for_response(10);
  if(response.length() > 0){
    if(response.startsWith("face")){
        plotter.draw_smile();
        plotter.show_for_ms(1000);
    }
    else if(response.startsWith("forward")){
      car.forward(2048);
      delay(1000);
      car.stop();
    }
    else if(response.startsWith("backward")){
      car.backward(2048);
      delay(1000);
      car.stop();
    }
    else if(response.startsWith("left")){
      car.left_turn(400, 2048);
    }
  }
}