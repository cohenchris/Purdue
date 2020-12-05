#include "http_messages.h"

/*
 * You should implement this function and use it in server.c
 */

http_response handle_htdocs(const http_request *request) {
    http_response resp = { 0 };
    resp.http_version = request->http_version;

    return resp;
}
