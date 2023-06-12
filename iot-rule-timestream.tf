## iot rule

resource "aws_iot_topic_rule" "iot_rule" {
  name = "esp8266iotrule"
  sql = "SELECT * FROM '${var.AWS_IOT_PUBLISH_TOPIC}'"
  sql_version = "2016-03-23"
  enabled     = true
  description = "Send data from the AWS IoT Core to AWS Timestream"

  timestream {
      database_name = aws_timestreamwrite_database.timestream_database.database_name
      table_name    =  aws_timestreamwrite_table.IoTSensorTable.table_name # "device_esp8266"

      dimension {
      name  = "deviceid"
      value = "$${deviceid}" # double "$" is required to avoid the special charater, otherwise it will raise an error. 
      }
      timestamp {
        value = "$${time}"
        unit  = "MILLISECONDS"
        }
    role_arn = "arn:aws:iam::${data.aws_arn.thing.account}:role/service-role/esp8266_timestream_rule"
  }

}

## database creation

resource "aws_timestreamwrite_database" "timestream_database" {
  database_name = "iot-esp8266"

  tags = {
    Name = "iot-esp8266"
  }
}


/*

#### policy rule for -  role_arn = "arn:aws:iam::${data.aws_arn.thing.account}:role/service-role/esp8266_timestream_rule
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "timestream:WriteRecords"
            ],
            "Resource": "arn:aws:timestream:ap-southeast-2:722141136946:database/iot-esp8266/table/IoTSensor_ESP8266v1"
        },
        {
            "Effect": "Allow",
            "Action": [
                "timestream:DescribeEndpoints"
            ],
            "Resource": "*"
        }
    ]
}
*/


resource "aws_timestreamwrite_table" "IoTSensorTable" {
  database_name = aws_timestreamwrite_database.timestream_database.database_name
  table_name    = "IoTSensor_ESP8266v1"

  retention_properties {
    magnetic_store_retention_period_in_days = 3
    memory_store_retention_period_in_hours  = 1
  }

  tags = {
    Name = "sensor-reading-timestream-table"
  }
}
