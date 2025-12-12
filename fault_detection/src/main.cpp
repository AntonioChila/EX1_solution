#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sysmonitor_interfaces/msg/sysmon.hpp"
#include "fault_detection/magiclib.h"
using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber(): Node("minimal_subscriber")
    {
      subscription_ = this->create_subscription<sysmonitor_interfaces::msg::Sysmon>(
      "Orin_telemetry", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

  private:
    void topic_callback(const sysmonitor_interfaces::msg::Sysmon & msg) const
    {
      RCLCPP_INFO(this->get_logger(), "I heard Orin Telemetry");
      try {
        magic_function(msg.cpu_usage, msg.cpu_temp, msg.ram_usage, msg.gpu_usage, msg.gpu_temp);
      	} 
      catch (const std::invalid_argument& e) {
        RCLCPP_ERROR(this->get_logger(), "CRASH RILEVATO! Messaggio: %s", e.what());
  	RCLCPP_ERROR(this->get_logger(), "--- REPORT DEI SOSPETTI ---");
  	RCLCPP_ERROR(this->get_logger(), "CPU Temp: %.2f", msg.cpu_temp);
  	RCLCPP_ERROR(this->get_logger(), "CPU Usage: %.2f", msg.cpu_usage);
  	RCLCPP_ERROR(this->get_logger(), "GPU Temp: %.2f", msg.gpu_temp);
  	RCLCPP_ERROR(this->get_logger(), "GPU Usage: %.2f", msg.gpu_usage);
  	RCLCPP_ERROR(this->get_logger(), "RAM Usage: %.2f", msg.ram_usage);
  	RCLCPP_ERROR(this->get_logger(), "GPURAM Usage: %.2f", msg.gpuram_usage); 
      }

    }
    rclcpp::Subscription<sysmonitor_interfaces::msg::Sysmon>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
