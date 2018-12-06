#!/usr/bin/env python3
# -*- coding: utf-8 -*-

####################################################################################################

### Note: http.server is not recommended for production, it only implements basic security. ###

from http.server import HTTPServer, SimpleHTTPRequestHandler
import ssl
import signal
import os

####################################################################################################

port = 4443
server_path_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), "server")
certs_path_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), "certs")
server_cert_file = os.path.join(certs_path_dir, "ca_cert.pem")
server_key_file = os.path.join(certs_path_dir, "ca_key.pem")

####################################################################################################

### Termination signals handler for program process ###
def signal_handler(signal, frame):
    '''Termination signals (SIGINT, SIGTERM) handler for program process'''
    print("Program close successfully.\n")
    os._exit(0)


### Signals attachment ###
signal.signal(signal.SIGTERM, signal_handler) # SIGTERM (kill pid) to signal_handler
signal.signal(signal.SIGINT, signal_handler) # SIGINT (Ctrl+C) to signal_handler

####################################################################################################

### Main Function ###

def main():
    '''Main Function'''
    # Change to Server subdirectory
    os.chdir(server_path_dir)
    # Create the Server
    httpd = HTTPServer(("", port), SimpleHTTPRequestHandler)
    httpd.socket = ssl.wrap_socket(httpd.socket, server_side=True, certfile=server_cert_file, 
	                               keyfile=server_key_file)
    print("\nServer started at https://localhost:{}".format(str(port)))
    # Run the Server
    httpd.serve_forever()


####################################################################################################

if __name__ == '__main__':
    main()

### End Of Code ###