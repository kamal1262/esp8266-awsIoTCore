variable "AWS_IOT_PUBLISH_TOPIC" {
  description = "AWS_IOT_PUBLISH_TOPIC"
  type        = string
  default     = "esp8266/pub"
}

variable "AWS_IOT_SUBSCRIBE_TOPIC" {
  description = "AWS_IOT_SUBSCRIBE_TOPIC to subscribe on device to receive msg from AWS IoT Core"
  type        = string
  default     = "esp8266/sub"
}