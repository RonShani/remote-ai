#include <vector>
#include "remote_ai_client.hpp"
#include "matrix_plotter.hpp"
#include "vehicle_driver.hpp"
#include "communication_manager.hpp"

CommunicationManager cm;
Vehicle car{cm};
RemoteAIClient rai_client;
MatrixPlotter plotter;

void setup()
{
  rai_client = RemoteAIClient{"Cohen2", "25sep1963"};
  rai_client.connect_host("44.203.26.117", 3000);
  cm.begin();
  std::vector<String> topics = {"ok","face","forward","backward","left"};
  std::vector<Functionality> actions = {{Vehicle::blink_ok, car},{MatrixPlotter::face, plotter}, {Vehicle::go_forward, car}, {Vehicle::go_backward, car}, {Vehicle::turn_left, car}};
  rai_client.topic_loader(topics, actions);
}

void loop() {
  rai_client.wait_for_response(10);
}
