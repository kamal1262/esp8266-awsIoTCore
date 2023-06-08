
# IoT endpoint- The client needs to know where to connect. For this, we’ll use a data source and output its result
data "aws_iot_endpoint" "iot_endpoint" {
	endpoint_type = "iot:Data-ATS"
}

output "iot_endpoint" {
	value = data.aws_iot_endpoint.iot_endpoint.endpoint_address
}