#ifndef ROUTES_H
#define ROUTES_H

#include "http_messages.h"

http_response handle_cgi_bin(const http_request *request);
http_response handle_htdocs(const http_request *request);
http_response handle_default(const http_request *request);

typedef http_response (*route_handle) (const http_request *);

typedef struct{
    char *route_name;
    route_handle handle_func;
}  routes;

// A routes map, pairing string served and handle function
routes route_map[] = {
    {"/cgi-bin", &handle_cgi_bin},
    {"/", &handle_htdocs},
    {"", &handle_default}
};

#endif  // ROUTES_H
