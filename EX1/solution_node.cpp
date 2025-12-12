#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "sysmonitor_interfaces/msg/sysmon.hpp"

using std::placeholders::_1;

class ExamNode : public rclcpp::Node
{
public:
  ExamNode()
  : Node("ex_solution_node") 
  {
    // Publisher su /test
    publisher_ = this->create_publisher<std_msgs::msg::Float64>("/test", 10);

    // Subscriber su /system_info
    subscription_ = this->create_subscription<sysmonitor_interfaces::msg::Sysmon>(
      "/system_info", 
      10, 
      std::bind(&ExamNode::topic_callback, this, _1));
      
    RCLCPP_INFO(this->get_logger(), "Nodo avviato (ex_solution). In ascolto su /system_info...");
  }

private:
  void topic_callback(const sysmonitor_interfaces::msg::Sysmon::SharedPtr msg) const
  {
    (void)msg;
    auto message = std_msgs::msg::Float64();
    message.data = 1.0; // Pubblica 1.0
    publisher_->publish(message);
  }

  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
  rclcpp::Subscription<sysmonitor_interfaces::msg::Sysmon>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ExamNode>());
  rclcpp::shutdown();
  return 0;
}
