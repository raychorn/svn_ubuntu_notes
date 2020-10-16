openssl genrsa -des3 -out server.key 2048
openssl rsa -in server.key -out server.key.insecure
openssl req -new -key server.key -out server.csr
openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt
