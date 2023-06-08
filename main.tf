resource "random_id" "id" {
	byte_length = 8
}

#AWS-IoT-Thing
resource "aws_iot_thing" "thing" {
  name = "esp8266"
}


output "thing_name" {
	value = aws_iot_thing.thing.name
}