This directory contains a Python HTTPS Webserver with certificates that going to be uploaded as binary into ESP Flash .rodata section.

To create custom certifies:
openssl req -new -x509 -keyout ca_key.pem -out ca_cert.pem -days 365 -nodes
openssl x509 -in ca_cert.pem -out ca_cert.crt

To start Host Python HTTPS server:
python3 .\https_server.py

SSL cert files are uploaded to ESP because them was specificied using -DCOMPONENT_EMBED_TXTFILES build flag inside platformio.init file.

To access cert binary data from ESP program code, use:
extern const uint8_t server_cert_pem_start[] asm("_binary_certs_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_certs_ca_cert_pem_end");

