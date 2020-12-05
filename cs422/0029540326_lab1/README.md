# SERVER
Run using: `python3.7 myserver.py <port_number>`
** Server will terminate client connection after 15 seconds! **


# CLIENT
Run using: `python3.7 myclient.py <server_ip> <port_number>`
Once launched, run using the format `<command>  <filename>`

Example Requests:
```
  GET /
  HEAD /file.txt
```

When finished sending requests, you must type `close` to properly close the
connection.


NOTE: Run the command `chmod -r Upload/forbidden.txt` for the forbidden file to function properly
