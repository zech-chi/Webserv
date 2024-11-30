/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarim <skarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 12:53:50 by skarim            #+#    #+#             */
/*   Updated: 2024/11/30 14:06:08 by skarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const vector<int> &ports, const vector<string> &server_names, const long long &client_max_body_size,
                const vector<Location> &locations, const string &global_root, const pair<string, string> &redirection,
                const vector<string> &indexes, bool autoindex, const vector<string> &error_pages) :
                ports(ports), server_names(server_names), client_max_body_size(client_max_body_size),
                locations(locations), global_root(global_root), redirection(redirection), indexes(indexes), autoindex(autoindex), 
                error_pages(error_pages)
{
    
}

const vector<int> &Server::get_ports(void) const
{
    return (ports);
}

const vector<string> &Server::get_server_names(void) const
{
    return (server_names);
}

const long long &Server::get_client_max_body_size(void) const
{
    return (client_max_body_size);
}

const vector<Location> &Server::get_locations(void) const
{
    return (locations);
}

const string &Server::get_global_root(void) const
{
    return (global_root);
}

const pair<string, string> &Server::get_redirection(void) const
{
    return (redirection);
}

const vector<string> &Server::get_indexes(void) const
{
    return (indexes);
}

bool Server::get_autoindex(void) const
{
    return (autoindex);
}

const vector<string> &Server::get_error_pages(void) const
{
    return (error_pages);
}

void Server::print_server_info(void) const {
    cout << BOLD_YELLOW << "************* location info *************" << RESET << endl;
    cout << BG_BLACK;

    cout << BOLD_BLUE << "ports: " << BOLD_WHITE << "[";
    for (int i = 0; i < ports.size(); i++) {
        if (i > 0)
            cout << ", ";
        cout << ports[i];
    }
    cout << "]" << endl;

    cout << BOLD_BLUE << "server names: " << BOLD_WHITE << "[";
    for (int i = 0; i < server_names.size(); i++) {
        if (i > 0)
            cout << ", ";
        cout << server_names[i];
    }
    cout << "]" << endl;

    cout << BOLD_BLUE << "indexes: " << BOLD_WHITE << "[";
    for (int i = 0; i < indexes.size(); i++) {
        if (i > 0)
            cout << ", ";
        cout << indexes[i];
    }
    cout << "]" << endl;

    cout << BOLD_BLUE << "client_max_body_size: " << BOLD_WHITE << client_max_body_size << " Byte\n";

    cout << BOLD_BLUE << "global_root: " << BOLD_WHITE << global_root << "\n";

    cout << BOLD_BLUE <<  "redirection: " << BOLD_WHITE;
    cout << "[" << redirection.first << "]" << " [" << redirection.second << "]\n";

    cout << BOLD_BLUE << "auto_index: " << BOLD_WHITE;
    if (autoindex) cout << "on" << endl;
    else cout << "off" << endl;

}