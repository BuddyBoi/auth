#pragma once
#include <iostream>
#include "webrequest.h"
#include "util.h"

std::string domain = "http://185.150.119.6";
enum COMMANDS
{
    COMMAND_LOGIN,
    COMMAND_MOTD,
    COMMAND_SETHWID,
    COMMAND_SETIP,
    COMMAND_CLIENT_GETIP,
    COMMAND_VERSION,
    COMMAND_STATUS,
};

enum ROLES
{
    ROLE_REGISTERED = 0,
    ROLE_ADMIN = 10,
};

std::string get_role(int role)
{
    switch (role)
    {
        case ROLE_ADMIN:
            return "Admin";
            break;
        case ROLE_REGISTERED:
            return "Registered";
            break;
        default:
            return "error";
    }
}

std::string admin_key = "92565";
std::string id;
std::string username;
std::string password;
std::string ip;
std::string client_ip;
std::string hwid;
std::string client_hwid;
int role;

std::string get_response(COMMANDS command, std::vector<std::string> parameters = {})
{

    std::string request;
    switch (command)
    {
        //Check login
        case COMMAND_LOGIN:
        {
            request = "/login.php?username=" + parameters[0] + "&password=" + parameters[1];
            std::string response = web_read(domain, request);
            id = extract_string(response, "<id>", "<user>");
            hwid = extract_string(response, "<hwid>", "<role>");
            ip = extract_string(response, "<ip>", "<hwid>");
            role = std::stoi(extract_string(response, "<role>", "<end>"));
            return response;
            break;
        }
        //Get MOTD
        case COMMAND_MOTD:
        {
            request = "/motd.txt";
            std::string temp = web_read(domain, request);
            return str_remove_whitespace(temp);
            break;
        }            
        case COMMAND_VERSION:
        {
            request = "/version.txt";
            std::string temp = web_read(domain, request);
            return str_remove_whitespace(temp);
            break;
        }            
        case COMMAND_STATUS:
        {
            request = "/status.txt";
            std::string temp = web_read(domain, request);
            return temp;
            break;
        }            
        case COMMAND_SETHWID:
            request = "/hwid.php?username=" + parameters[0] + "&hwid=" + parameters[1] + "&key=" + admin_key;
            return web_read(domain, request);
            break;
        case COMMAND_SETIP:
        {
            request = "/ip.php?username=" + parameters[0] + "&ip=" + parameters[1] + "&key=" + admin_key;
            return web_read(domain, request);
            break;
        }
        case COMMAND_CLIENT_GETIP:
        {
            std::string temp = web_read("checkip.amazonaws.com", "/");
            return str_remove_whitespace(temp);
            break;
        }

        default:
            return "Invalid command";
            break;
    }

    return "error";
}

bool api_set_ip(std::string username)
{
    try
    {
        std::string response = get_response(COMMAND_SETIP, { username, client_ip });
        return true;
    }
    catch (std::exception e)
    {
        return false;
    }
}

bool api_set_hwid(std::string username)
{
    try
    {
        std::string response = get_response(COMMAND_SETHWID, { username, client_hwid });
        return true;
    }
    catch (std::exception e)
    {
        return false;
    }
}

bool api_login(std::string username, std::string password)
{
    try
    {
        std::string response = get_response(COMMAND_LOGIN, { username, password });
        return true;
    }
    catch (std::exception e)
    {
        return false;
    }
}

//get client info before comparing data to the server
bool setup()
{
    client_hwid = "testingID";
    client_ip = get_response(COMMAND_CLIENT_GETIP);

    return true;
}

bool auth()
{
    if (api_login(username, password))
    {
        std::cout << "[ api ] Login Successful" << std::endl;

        if (hwid == "")
        {
            std::cout << "[ api ] Hwid not found." << std::endl;
            if (!api_set_hwid(username))
            {
                std::cout << "[ api ] Failed to set HWID" << std::endl;
                std::exit(0);
            }
            else
                std::cout << "[ api ] Hwid set" << std::endl;
        }

        if (client_ip != ip)
        {
            std::cout << "[ api ] Ip does not match" << std::endl;
            return false;
        }

        api_set_ip(username);
        std::cout << "[ api ] Auth complete" << std::endl;
        return true;
    }

    return false;
}