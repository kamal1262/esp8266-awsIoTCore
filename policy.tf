data "aws_arn" "thing" {
	arn = aws_iot_thing.thing.arn
}


resource "aws_iot_policy" "policy" {
  name = "thingpolicy_esp8266"

  policy = jsonencode({
	"Version": "2012-10-17",
	"Statement": [
        {
            Action = [
                "iot:Connect",
            ]
            Effect   = "Allow"
            Resource = "arn:aws:iot:${data.aws_arn.thing.region}:${data.aws_arn.thing.account}:client/${aws_iot_thing.thing.name}"
        },
        {
            Action = [
                "iot:Publish",
                
            ]
            Effect   = "Allow"
            Resource = "arn:aws:iot:${data.aws_arn.thing.region}:${data.aws_arn.thing.account}:topic/${var.AWS_IOT_PUBLISH_TOPIC}"
        },
                {
            Action = [
                "iot:Receive",
                
            ]
            Effect   = "Allow"
            Resource = "arn:aws:iot:${data.aws_arn.thing.region}:${data.aws_arn.thing.account}:topic/${var.AWS_IOT_SUBSCRIBE_TOPIC}"
        },
        {
            Action = [
                "iot:Subscribe",
                
            ]
            Effect   = "Allow"
            Resource = "arn:aws:iot:${data.aws_arn.thing.region}:${data.aws_arn.thing.account}:topicfilter/${var.AWS_IOT_SUBSCRIBE_TOPIC}"
        },
        {
            Effect = "Allow"
            Action = [
                 "timestream:WriteRecords"
            ]
            Resource = [
                "${aws_timestreamwrite_database.timestream_database.arn}"
            ]
      }
                
	]
})
}



resource "aws_iot_policy_attachment" "attachment" {
	policy = aws_iot_policy.policy.name
	target = aws_iot_certificate.cert.arn
}

# output "policy" {
# 	value = aws_iot_policy.policy.policy
# }