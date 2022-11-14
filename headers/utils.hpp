#pragma once

#include <unistd.h>
#include <fcntl.h> 
#include <cctype>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cassert>

#include <map>
#include <vector>

#include <QMessageBox>

#ifndef UTILS_HPP
#define UTILS_HPP

#define MAX_STR_LEN 300
#define MAX_SHORT_STR_LEN 48
#define MAX_IP_LEN 16
#define SOCKET_BUFFER_SIZE 4096
#define SOCKET_TIMEOUT_SECONDS 5
#define HWND unsigned int

inline void upperCase(char* input) 
{
    int i = 0;
    int imax = strnlen(input, MAX_STR_LEN);
    for(i = 0; i < imax; i++)
    {
        if(isalpha(input[i]))
            input[i] = toupper(input[i]);
    }
}

inline void lowerCase(char* input) 
{
    int i = 0;
    int imax = strnlen(input, MAX_STR_LEN);
    for(i = 0; i < imax; i++)
    {
        if(isalpha(input[i]))
            input[i] = tolower(input[i]);
    }
}

inline std::map<std::string, std::string> parseDigest(const char* input)
{
    std::map<std::string, std::string> res;
    int i = 0, imax = strnlen(input, MAX_STR_LEN), state = 0;
    std::string key = "", val = "";

    for(i = 0; i < imax; i++)
    {
        if(input[i] == ' ' && state == 0)
            key.clear();
        else if(input[i] == '=' && state == 0)
        {
            state = 1;
        }
        else if(input[i] == ',' && state == 1)
        {
            res[key] = val;
            key.clear();
            val.clear();
            state = 0;
        }
        else if(input[i] != '"')
        {
            if(state == 0)
                key = key  + input[i];
            else if(state == 1)
                val = val + input[i];
        }
    }
    
    return res;
}

inline std::string mapToString(std::map<std::string, std::string> input)
{
    std::string res = "";

    for (const auto& [key, value] : input) 
    {
        res = res + '[' + key + "] = " + value + "; ";
    }

    return res;
}

inline std::string mapToString(std::map<int, int> input)
{
    std::string res = "";

    for (const auto& [key, value] : input) 
    {
        res = res + '[' + std::to_string(key) + "] = " + std::to_string(value) + "; ";
    }

    return res;
}


inline std::vector<std::vector<std::string>> readDelimetedString(const char* input, const char* delim)
{
    std::vector<std::vector<std::string>> res;
    std::vector<std::string> lines;
    std::string inputCopy = input;
    char* strtokBuffer = strtok((char*)inputCopy.c_str(), "\n");
    while(strtokBuffer != NULL)
    {
        lines.push_back(strtokBuffer);
        strtokBuffer = strtok(NULL, "\n");
    }

    for(auto line: lines)
    {
        std::vector<std::string> lineVec;
        strtokBuffer = strtok((char*)line.c_str(), delim);
        while(strtokBuffer != NULL)
        {
            lineVec.push_back(strtokBuffer);
            strtokBuffer = strtok(NULL, delim);
        }
        
        res.push_back(lineVec);
    }

    return res;
}

inline std::map<std::string, std::string> mapDelimetedString(const char* input, const char* delim)
{
    std::map<std::string, std::string> res;
    std::vector<std::string> lines;
    std::string inputCopy = input;
    char* strtokBuffer = strtok((char*)inputCopy.c_str(), "\n");
    while(strtokBuffer != NULL)
    {
        lines.push_back(strtokBuffer);
        strtokBuffer = strtok(NULL, "\n");
    }

    for(auto line: lines)
    {
        strtokBuffer = strtok((char*)line.c_str(), delim);
        res[strtokBuffer] = strtok(NULL, delim);
    }

    return res;
}


class JSONB
{
    public:
        JSONB(const char* input = NULL)
        {
            if(input != NULL)
                this->parse(input);
        }
        ~JSONB()
        {
            this->_dict.clear();
        }
        int keyExists(const char* key)
        {
            return this->_dict.find(std::string(key)) != this->_dict.end();
        }
        std::map<std::string, std::string>::iterator getFirstPair()
        {
            return this->_dict.begin();
        }
        int isEmpty()
        {
            return this->_dict.size() == 0;
        }
        void parse(const char* input)
        {
            int i = 0, imax = strnlen(input, MAX_STR_LEN), state = 0;
            std::string key = "", value = "";
            this->_dict.clear();

            for(i = 0; i < imax; i++)
            {
                if(input[i] == '{')
                {
                    state = 0;
                    key.clear();
                    value.clear();
                }
                else if((input[i] == '}' || input[i] == ',') && state == 1)
                {
                    this->_dict[key] = value;
                    state = 0;
                    key.clear();
                    value.clear();
                }
                else if(input[i] == ':')
                    state = 1;
                else if(input[i] == '"')
                {
                    if(state == 1)
                        state = 2;
                    else if(state == 2)
                        state = 1;
                }
                else if(input[i] != '"')
                {
                    if(state == 0 && input[i] != ' ' && input[i] != ',')
                        key = key + input[i];
                    else if((state == 1 && input[i] != ' ') || state == 2)
                        value = value + input[i];
                }            
            }
        }
        const char* getValue(const char* key)
        {
            return this->_dict[std::string(key)].c_str();
        }
        friend std::ostream&
        operator<<(std::ostream& os, JSONB& jsonb) {
            return os << mapToString(jsonb._dict);
        }

    private:
        std::map<std::string, std::string> _dict;
};

enum ERROR_CODES {LOGIN_RET_PING_FAILED, LOGIN_RET_INVALID_USERNAME, LOGIN_RET_INVALID_CREDENTIALS, LOGIN_RET_LOCKED, LOGIN_RET_TIMEOUT, LOGIN_RET_DEVICE_BUSY, LOGIN_RET_MAX_CONNECTIONS, 
                  LOGIN_RET_UNKNOWN, ERR_NO_FREE_FRAMES, ERR_INVALID_SN, ERR_INVALID_INPUT, ERR_PLAYBACK_NOT_EXIST, ERR_AUTOCOMPLETE_PARAM_MISSING_IP, ERR_AUTOCOMPLETE_NOT_FOUND,
                  ERR_NO_CHANNELS_FOUND, ERR_UNKNOWN};

class MaleniaException {
    public:
        static void show(ERROR_CODES ret, const char* custom = NULL)
        {
            char* msg = (char*)custom;
            if(msg == NULL)
            {
                switch (ret)
                {
                    case LOGIN_RET_PING_FAILED:
                        msg = (char*)"LOGIN : Failed to reach device";
                        break;
                    case LOGIN_RET_INVALID_USERNAME:
                        msg = (char*)"LOGIN : Invalid username";
                        break;
                    case LOGIN_RET_INVALID_CREDENTIALS:
                        msg = (char*)"LOGIN : Invalid username or password";
                        break;
                    case LOGIN_RET_LOCKED:
                        msg = (char*)"LOGIN : Locked from logging in";
                        break;
                    case LOGIN_RET_TIMEOUT:
                        msg = (char*)"LOGIN : Login timed out";
                        break;
                    case LOGIN_RET_DEVICE_BUSY:
                        msg = (char*)"LOGIN : Device is busy";
                        break;
                    case LOGIN_RET_MAX_CONNECTIONS:
                        msg = (char*)"LOGIN : Device reached max connections limit";
                        break;
                    case LOGIN_RET_UNKNOWN:
                        msg = (char*)"LOGIN : Unknown login error";
                        break;
                    case ERR_NO_FREE_FRAMES:
                        msg = (char*)"There is an insufficient amount of free frames";
                        break;
                    case ERR_INVALID_SN:
                        msg = (char*)"Invalid Serial Number";
                        break;
                    case ERR_INVALID_INPUT:
                        msg = (char*)"Invalid input";
                        break;
                    case ERR_PLAYBACK_NOT_EXIST:
                        msg = (char*)"Playback in that time range does not exist";
                        break;
                    case ERR_AUTOCOMPLETE_PARAM_MISSING_IP:
                        msg = (char*)"You did not provide an IP Address to search for";
                        break;
                    case ERR_AUTOCOMPLETE_NOT_FOUND:
                        msg = (char*)"Did not find a password for this IP Address";
                        break;
                    case ERR_NO_CHANNELS_FOUND:
                        msg = (char*)"Failed to find any channels for this device";
                        break;
                    case ERR_UNKNOWN:
                        msg = (char*)"Unknown error";
                        break;
                    default:
                        msg = (char*)"Unknown error";
                        break;
                }
            }

            QMessageBox::critical(NULL, QObject::tr("ERROR"), QObject::tr(msg));
        }
};

inline char* strncpy_t(char* out, size_t outsz, const char* in, size_t insz)
{
    if(in == NULL || out == NULL)
        return NULL;

    insz = strnlen(in, insz);
    assert(outsz > 0);
    while(--outsz > 0 && insz > 0 && *in) 
    { 
        *out++ = *in++; // copy character value and increase ptrs
        insz--; 
    }

    *out = 0; // set last byte to zero
    return out;
}

typedef struct datetime
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
} datetime;

inline datetime calcDatetimeOffset(tm* localDt, int nvrYear, int nvrMonth, int nvrDay, int nvrHours, int nvrMinutes, int nvrSeconds)
{
    datetime offset;
    offset.year = (localDt->tm_year + 1900) - nvrYear;
    offset.day = localDt->tm_mday - nvrDay;
    offset.hours = localDt->tm_hour - nvrHours;
    offset.minutes = localDt->tm_min - nvrMinutes;
    
    return offset;
}

inline std::string int_to_hex(int i)
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(2) 
         << std::hex << i;
  return stream.str();
}

inline std::string datetime_to_string(tm* input)
{
    std::stringstream ss;
    std::string res;
    ss << std::put_time(input, "%d/%m/%Y %H:%M");
    res = ss.str();
    return res;
}

inline std::string get_mac_address()
{
    std::string res = "a4:bb:6d:50:1b:e9";
    upperCase((char*)res.c_str());
    return res;
}

inline int bytes_contain(char* input, int size, char* f)
{
    int i = 0, cursor = 0, fsize = strnlen(f, MAX_STR_LEN);
    for(i = 0; i < size; i++)
    {
        if(cursor == fsize - 1)
            return 1;
        if(input[i] == f[cursor])
            cursor++;
        else
            cursor = 0;
    }

    return 0;
}

inline void ltrim(std::string& s) 
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void rtrim(std::string& s) 
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

inline void trim(std::string& s) 
{
    rtrim(s);
    ltrim(s);
}

inline datetime calc_datetime_offset(datetime original, datetime offset)
{
    datetime result;
    result.year = original.year;
    result.month = original.month;
    result.day = original.day;
    result.hours = original.hours;
    result.minutes = original.minutes;
    result.seconds = original.seconds;

    result.seconds -= offset.seconds;
    if(result.seconds < 0)
    {
        result.minutes -= - 1;
        result.seconds += 60;
    }

    result.minutes -= offset.minutes;
    if(result.minutes < 0)
    {
        result.hours -= 1;
        result.minutes += 60;
    }

    result.hours -= offset.hours;
    if(result.hours < 0)
    {
        result.day -= 1;
        result.hours += 60;
    }

    result.day -= offset.day;
    if(result.day < 0)
    {
        result.month -= 1;
        result.day += 30;
    }

    result.month -= offset.month;
    if(result.month < 0)
    {
        result.year -= 1;
        result.month += 12;
    }

    result.year -= offset.year;
    
    return result;
}
#endif
