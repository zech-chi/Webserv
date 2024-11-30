/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarim <skarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 11:24:19 by skarim            #+#    #+#             */
/*   Updated: 2024/11/30 11:40:37 by skarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
    
}
        // Location(string file_path); here do your shit alparser nigro o zid setters;
Location::~Location()
{
    
}

const string &Location::get_route(void) const
{
    return (route);
}

const vector<string> &Location::get_indexes(void) const
{
    return (indexes);
}

bool Location::get_auto_index(void) const
{
    return (auto_index);
}

const string &Location::get_root(void) const
{
    return (root);
}

const map<string, bool> &Location::get_methods(void) const
{
    return (methods);
}

const pair<string, string> &Location::get_redirections(void) const
{
    return (redirections);
}

bool Location::set_route(const string &route)
{
    this->route = route;
}

bool Location::set_indexes(const vector<string> &indexes)
{
    this->indexes = indexes;
}

bool Location::set_auto_index(bool auto_index)
{
    this->auto_index = auto_index;
}

bool Location::set_root(const string &root)
{
    this->root = root;
}

bool Location::set_methods(const map<string, bool> &methods)
{
    this->methods = methods;
}

bool Location::set_redirections(const pair<string, string> &redirections)
{
    this->redirections = redirections;
}