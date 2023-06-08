#iot rule

# resource "aws_iot_topic_rule" "iot_rule" {
#   name = "esp8266iotrule"
#   sql = "SELECT * FROM '${var.AWS_IOT_PUBLISH_TOPIC}'"
#   sql_version = "2016-03-23"
#   enabled     = true
#   description = "Send data from the AWS IoT Core to AWS Timestream"

#   timestream {
#       database_name = aws_timestreamwrite_database.timestream_database.database_name

#         dimension {}

# }
# }

# database creation

resource "aws_timestreamwrite_database" "timestream_database" {
  database_name = "iot-esp8266"

  tags = {
    Name = "iot-esp8266"
  }
}

