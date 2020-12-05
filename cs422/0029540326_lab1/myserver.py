import os
from socket import socket
import sys
from stat import ST_SIZE
from select import select
import threading
import time

from http_helper import parse_request, create_response, create_tcp_sock     # Function imports
from http_helper import HTTP_VERSION, MAX_SIZE, MAX_REQUEST                 # Constant imports 
from http_helper import OK, FORBIDDEN, NOT_FOUND, NOT_ALLOWED               # Response code imports


"""
    Service socket connection
"""
def dispatch_connection(client_sock, client_addr):
    ##################################
    ##### RECEIVE CLIENT REQUEST #####
    ##################################
    # file_request = <command> <request_uri> <http_version>
    while True:
        # This infinite loop keeps handling requests until socket requested to close

        # Extract client request header (going to -2 leaves out last /r/n)
        socket_ready = select([client_sock], [], [], 15)
        if socket_ready[0]:
            client_request = client_sock.recv(MAX_REQUEST).decode().split("\r\n")[:-2]
            #print("REQUEST =        " + str(client_request))
        else:
            client_sock.close()
            print("ERROR: Socket Timeout")
            break

        if not client_request:
            # If client_request is blank, nothing sent, client closed connection
            print("Terminating connection with address " + str(client_addr))
            break
        else:
            client_command, client_request_uri, headers = parse_request(client_request)            
            send_file = False

        #############################################
        ##### CREATE HTTP RESPONSE AND GET DATA #####
        #############################################
        available_files = os.listdir("./Upload/")
        if client_command != "GET" and client_command != "HEAD":
            # Command is invalid, send 405 Not Allowed
            response, response_data = create_response(NOT_ALLOWED, client_command, client_request_uri, headers)
        elif client_request_uri in available_files:
            # File is present in 'Upload' folder

            # Check if file has read permissions
            readable = os.access("./Upload/" + client_request_uri, os.R_OK)             # TODO: is this sufficient for 403?

            if readable:
                # File has correct permissions --> send 200 OK and file data
                file_size = os.stat("./Upload/" + client_request_uri).st_size
                response, response_data = create_response(OK, client_command, client_request_uri, headers, file_size)
                send_file = True
            else:
                # File does NOT have correct permissions --> send 403 Forbidden
                response, response_data = create_response(FORBIDDEN, client_command, client_request_uri, headers)
        else:
            # File is not present in 'Upload' folder --> send 404
            response, response_data = create_response(NOT_FOUND, client_command, client_request_uri, headers)

        client_sock.send(response.encode())
        
        if client_command == "HEAD":
            # 'HEAD' only sends headers, not data
            send_file = False

        if send_file:
            client_sock.send(response_data if type(response_data) is bytes else response_data.encode())

        if "Connection: close" in headers:
            # Last file requested
            break

    print("Terminating connection with address " + str(client_addr))



                                        ##########################
                                        ########## MAIN ##########
                                        ##########################
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage:\npython3.7 myserver.py <port_number>")
    else:
        server_sock = create_tcp_sock(int(sys.argv[1]))                         # Creates server socket with port num specified in arguments

        # Infinite loop makes sure server doesn't terminate after accepting a connection
        while True:
            client_sock, client_addr = server_sock.accept()        # Accepts incoming connection
            print("accepted connection with address " + str(client_addr))
            threading.Thread(target=dispatch_connection, args=(client_sock, client_addr)).start()
