import sys
import time
from socket import socket, AF_INET, SOCK_STREAM


MAX_SIZE = 1000000          # Max file size is 1MB
HTTP_VERSION = "HTTP/1.1"


if len(sys.argv) != 3:
    print("Usage:\npython3.7 myclient.py <serverIP> <serverPort>")
else:
    server_name = ''
    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])
    client_sock = socket(AF_INET, SOCK_STREAM)                      # Create TCP client socket
    client_sock.connect((server_name, server_port))                 # Connect to server

    while True:
        request = []
        while len(request) != 2:
            if len(request) == 1 and request[0] == "close":
                close_request = "GET " + server_ip + ":" + str(server_port) + "/" + HTTP_VERSION + "\r\n" + "Connection: close\r\n\r\n"
                client_sock.send(close_request.encode())
                print("Closing socket...")
                client_sock.close()
                sys.exit()
            request = input("Input HTTP request in the form <command> <filename> (or 'close' to stop): ").split()

        command = request[0]
        filename = request[1]

        request_uri = server_ip + ":" + str(server_port) + filename

        # http_request = <command> <request_uri> <HTTP_VERSION>
        http_request = command + " " + request_uri + " " + HTTP_VERSION + "\r\n\r\n"

        client_sock.send(http_request.encode())

        received_data = client_sock.recv(MAX_SIZE)

        # Extract response header length to split data and response header
        response_header_length = len(received_data.decode(errors="ignore").split("\r\n\r\n")[0]) + 4
        # Extract response header
        response_header = received_data[:response_header_length].decode()
        # Extract data
        data = received_data[response_header_length:]
        
        print(response_header)
        if "png" not in filename and "jpg" not in filename:
            print(data.decode())
        else:
            print("picture " + filename+ " received")

        #If file is returned with a valid 200 OK response, it's available to download
        if (response_header.split()[1] == "200" and command == "GET"):
            if filename == "/":
                filename = "/index.html"
            
            with open("./Download" + filename, "wb+") as new_file:
                    new_file.write(data)