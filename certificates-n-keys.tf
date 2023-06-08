### ref : https://advancedweb.hu/how-to-generate-iot-core-certificates-with-terraform/
## CA-signed certificate ##
/*
PEMs:

cert_request_pem:
		 is the certificate request for the device certificate
ca_private_key_pem:
		 is the private key for the CA
ca_cert_pem:
		 is the certificate of the CA

*/

# generate the CA certificate. This is the root of trust, so it is self-signed:

resource "tls_private_key" "key" {
	algorithm = "RSA"
	rsa_bits  = 2048
}

resource "tls_self_signed_cert" "ca" {
	private_key_pem = tls_private_key.key.private_key_pem

	validity_period_hours = 240

	allowed_uses = [
		"cert_signing",
	]

	is_ca_certificate = true

	subject {
		organization = "CA test signer"
	}
}

# generate the private key for the device certificate
resource "tls_private_key" "signed_1" {
	algorithm = "RSA"
	rsa_bits  = 2048
}
#create a signing request for this key
resource "tls_cert_request" "signed_1" {
	private_key_pem = tls_private_key.signed_1.private_key_pem

	subject {
		organization = "CA test"
	}
}

# sign the certificate request with the CA key
resource "tls_locally_signed_cert" "signed_1" {
	cert_request_pem   = tls_cert_request.signed_1.cert_request_pem
	ca_private_key_pem = tls_self_signed_cert.ca.private_key_pem
	ca_cert_pem        = tls_self_signed_cert.ca.cert_pem

	validity_period_hours = 240

	allowed_uses = [
	]
}

# add the device certificate to IoT Core
resource "aws_iot_certificate" "cert" {
	certificate_pem = trimspace(tls_locally_signed_cert.signed_1.cert_pem)
	active          = true
}

output "device_certificate" {
	value = tls_locally_signed_cert.signed_1.cert_request_pem
}

output "private_key" {
	value = nonsensitive(tls_locally_signed_cert.signed_1.ca_private_key_pem)
	# sensitive = true
}

output "ca_cert" {
	value = tls_locally_signed_cert.signed_1.ca_cert_pem
}








# # generate a private key using an appropriate algorithm

# resource "tls_private_key" "key" {
# 	algorithm   = "RSA"
# }

# # generate a self-signed certificate using the key
# resource "tls_self_signed_cert" "cert" {
# 	private_key_pem = tls_private_key.key.private_key_pem

# 	validity_period_hours = 240

# 	allowed_uses = [
# 	]

# 	subject {
# 		organization = "test"
# 	}
# }

# # Add the certificate to IoT Core
# resource "aws_iot_certificate" "cert" {
# 	certificate_pem = trimspace(tls_self_signed_cert.cert.cert_pem)
# 	active          = true
# }

# #Attach the certificate to the thing- To allow connections to a thing, the certificate needs to be attached to it
# resource "aws_iot_thing_principal_attachment" "attachment" {
# 	principal = aws_iot_certificate.cert.arn
# 	thing     = aws_iot_thing.thing.name
# }

# #Exports- Finally, as the client needs both the private key and the certificate, add outputs for them:
# output "cert" {
# 	value = tls_self_signed_cert.cert.cert_pem
# }

# output "key" {
# 	value = tls_private_key.key.private_key_pem
# 	sensitive = true
# }

