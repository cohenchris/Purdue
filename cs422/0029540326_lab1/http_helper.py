from socket import socket, AF_INET, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR


MAX_REQUEST = 1024
MAX_SIZE = 1000000          # Max file size is 1MB
HTTP_VERSION = "HTTP/1.1"

# Status codes and reason phrases
OK = ["200", "OK"]
FORBIDDEN = ["403", "Forbidden"]
NOT_FOUND = ["404", "Not Found"]
NOT_ALLOWED = ["405", "Not Allowed"]


"""
    Parses incoming client request from the socket and returns necessary information
"""
def parse_request(client_request):
    file_request = client_request[0].split()
    client_command = file_request[0]                      # COMMAND
    client_request_uri = file_request[1].split('/')[1]    # REQUEST_URI
    if client_request_uri == "":                          # / --> /index.html for HTTP servers
        client_request_uri = "index.html"
    headers = client_request[1:]

    return client_command, client_request_uri, headers

"""
    Gets the file extension for the requested file in the server
"""
def get_content_type(uri):
    file_extension = uri.split('.')[-1]
    if file_extension == "txt":
        return "text/plain"
    elif file_extension == "png" or file_extension == "jpg":
        return "image/" + file_extension
    elif file_extension == "html":
        return "text/html"

"""
    Gets data in a readable format for the requested URI on the server
"""
def get_data(uri, file_size):
        # Open file and get data
        read_mode = "r" if get_content_type(uri) == "text/plain" else "rb"       # rb for images, r for pics
        with open("./Upload/" + uri, read_mode) as requested_file:
            data = requested_file.read(file_size)
        return data

"""
    Create HTTP response with following format:
    <HTTP_VERSION> <status_code> <reason_phrase>
    <headers>
    ...
    <response_data>
"""
def create_response(code, command, uri, response_headers, file_size=0):
    response = HTTP_VERSION + " " + code[0] + " " + code[1] + "\r\n"

    response += "Content-Length: " + str(file_size) + "\r\n"
    if file_size > 0 :
        response_data = get_data(uri, file_size)
        response += "Content-Type: " + get_content_type(uri) + "\r\n\r\n"
    else:
        response_data = ""
        response += "\r\n"
    return response, response_data

"""
    Creates and binds a TCP socket that is listening for connections on the specified port number
"""
def create_tcp_sock(port_num):
    server_sock = socket(AF_INET, SOCK_STREAM)                              # Creates a TCP socket ready for use
    server_sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)                     # Makes used port immediately available after termination of server
    server_sock.bind(('', port_num))                                        # Binds the TCP socket for use from any address
    server_sock.listen(1)                                                   # Listens for connections on socket
    print("Listening on port " + str(port_num) + "...")
    return server_sock