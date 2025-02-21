/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zech-chi <zech-chi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 20:22:49 by zech-chi          #+#    #+#             */
/*   Updated: 2025/01/21 20:24:23 by zech-chi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <cstddef>
#include <sys/unistd.h>


HttpResponse::HttpResponse(HttpRequest *_request, WebServ *_webserv) {
    this->request = _request;
    this->webserv = _webserv;
    index_location = -1;
}

HttpResponse::~HttpResponse() {

}


void HttpResponse::check_post_location() {
    index_location = longest_common_location().first;
    // location not found
    if (index_location == -1) {
        request->set_is_complete_post(true);
        request->set_status_code("403");
        request->set_is_cgi(false);
        request->set_is_cgi_complete(true);
        return;
    }
    // method not allowed
    if (!is_allowed(index_location, "POST")) {
        request->set_is_complete_post(true);
        request->set_status_code("405");
        request->set_is_cgi(false);
        request->set_is_cgi_complete(true);
        return;
    }
}

const string& HttpResponse::get_script_path() const{
    vector<string> route = request->get_url();
    // check for root
    if (index_location != -1)
        request->set_cgi_input_file(request->get_server().get_locations()[index_location].get_root());
    // if not root for this location use global root
    if (index_location == -1 || request->get_cgi_input_file().empty())
        request->set_cgi_input_file(request->get_server().get_global_root());
    for (size_t i = 0; i < route.size(); i++) {
        if (i > 0)
            request->set_cgi_input_file( request->get_cgi_input_file() + "/");
        request->set_cgi_input_file( request->get_cgi_input_file() + route[i]);
    }
    if (request->get_cookie() == 2) {
        if (request->get_cgi_input_file().find(COOKIE2_SCRIPT_NAME) == string::npos)
            request->set_cgi_input_file(request->get_cgi_input_file() + "/" + COOKIE2_SCRIPT_NAME);
    }
    return request->get_cgi_input_file();
}

static string generate_session_id(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    // Extract seconds and microseconds
    time_t rawTime = tv.tv_sec;
    int microseconds = tv.tv_usec;
    
    // Convert to local time
    struct tm *timeInfo = localtime(&rawTime);

    // Create the formatted string
    ostringstream oss;
    oss << (timeInfo->tm_year + 1900) << "_"       // Full Year (e.g., 2025)
        << (timeInfo->tm_mon + 1) << "_"           // Month
        << timeInfo->tm_mday << "_"               // Day
        << timeInfo->tm_hour << "_"               // Hours
        << timeInfo->tm_min << "_"                // Minutes
        << timeInfo->tm_sec << "_"                // Seconds
        << microseconds;               // Microseconds

    return oss.str();
}

void    HttpResponse::handle_cookie1() {
    // checl url for cookie1
    if (request->get_url().size() > 2 || request->get_url().size() == 0)
        return;
    if (request->get_url().size() == 1 && request->get_url()[0] != "cookie")
        return;
    if (request->get_url().size() == 2 && request->get_url()[1] != "cookie.py")
        return;
    // // check location for cookie1
    int idx = longest_common_location().first;
    if (idx == -1) // /cookie location not found
        return;
    // // check cgi permission
    const string path = request->get_server().get_locations()[idx].get_path_cgi("py");
    if (path.empty()) {
        return;
    }
    if (request->get_header().find("cookie") != request->get_header().end()) {
        size_t pos = request->get_header().find("cookie")->second.find("session_id_1=");
        if (pos != string::npos) {
            string session_path = SESSION_MANAGEMENT + request->get_header().find("cookie")->second.substr(pos + 13);
            session_path = session_path.substr(0, session_path.find(";"));
            if (is_a_file(session_path) && access(session_path.c_str(), R_OK) == 0) {
                request->set_file_path(session_path);
                request->set_is_chunked(true);
                request->set_is_complete_post(true);
                request->set_is_cgi(false);
            }
            DEBUG_MODE && cout << BG_GREEN << "("  << session_path << ")" << "\n";
        }
    }
    request->set_cookie(1);
}

void    HttpResponse::handle_cookie2() {
    if ( request->get_cookie() == 0  && request->get_url().size() >= 1 &&  request->get_url()[0] == "cookie2" ) {
        bool flag = false;
        if (request->get_header().find("cookie") != request->get_header().end()) {
            size_t pos = request->get_header().find("cookie")->second.find("session_id_2=");
            if (pos != string::npos) {
                string session_path = request->get_header().find("cookie")->second.substr(pos + 13);
                session_path = session_path.substr(0, session_path.find(";"));
                request->set_session_id(session_path);
                session_path = SESSION_MANAGEMENT + session_path;
                if (is_a_file(session_path) && access(session_path.c_str(), R_OK) == 0) {
                    request->set_file_path(session_path);
                    request->set_cgi_path_post(session_path);
                    if (request->get_method() == "POST") {
                        request->set_is_complete_post(true);
                        request->set_is_chunked(true);
                        // request->set_cgi_path_post(session_path); // here we shoul remove input_file.txt was created before
                        fstream file(request->get_file_path(), ios::out | ios::trunc);
                        file.write(request->get_body().c_str(), request->get_body().size());
                        file.close();
                        flag = true;
                    }
                    request->set_cookie(2);
                }
            }
        }
        if (request->get_cookie() == 0 && !flag) {
            request->set_session_id(generate_session_id());
            request->set_cgi_path_post(SESSION_MANAGEMENT + request->get_session_id());
            fstream file(SESSION_MANAGEMENT + request->get_session_id(), ios::out);
            file.write("mode=light&lang=ar", 18);
            file.close();
            request->set_cookie(2);
        }
    }
}


void    HttpResponse::serv() {

    DEBUG_MODE && cout << "we are in serv\n";
    handle_cookie1(); // handle cookie1
    handle_cookie2(); // hangle cookie2
     
    if (request->get_method() == "GET")
        get_method();
    else if (request->get_method() == "POST") {
        check_post_location();
        post_method();
    }
    else if (request->get_method() == "DELETE")
        delete_method();
}


HttpRequest *HttpResponse::get_request() const {
    return this->request;
}

string HttpResponse::generate_file_name() const {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    // Extract seconds and microseconds
    time_t rawTime = tv.tv_sec;
    int microseconds = tv.tv_usec;
    
    // Convert to local time
    struct tm *timeInfo = localtime(&rawTime);

    // Create the formatted string
    ostringstream oss;
    oss << CGI_PATH
        << (timeInfo->tm_year + 1900) << "_"       // Full Year (e.g., 2025)
        << (timeInfo->tm_mon + 1) << "_"           // Month
        << timeInfo->tm_mday << "_"               // Day
        << timeInfo->tm_hour << "_"               // Hours
        << timeInfo->tm_min << "_"                // Minutes
        << timeInfo->tm_sec << "_"                // Seconds
        << microseconds << ".html";               // Microseconds

    return oss.str();
}
