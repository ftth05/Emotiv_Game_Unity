/****************************************************************************
**
** Copyright 2017 by Emotiv. All rights reserved
** Example - Using C++ Wrapper for SDK
** How to use C++ Wrapper for SDK
** Author: Dinh Duong Ha ha@emotiv.com
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif

#if __linux__ || __APPLE__
    #include <unistd.h>
    #include <ncurses.h>
#endif

#define DEFAULT_USERNAME "user"   // Default user name
#define DEFAULT_PASSWORD "password"  // Default user password

#include "Emotiv.hpp"
#include "MyEmotivCallback.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#if __linux__ || __APPLE__
    int _kbhit(void);
#endif

#ifdef __cplusplus
}
#endif

using namespace Emotiv;

int  main()
{
    MyEmotivCallback* cbk = new MyEmotivCallback();
    Emotiv::EmotivEngine emoEngine(cbk);    
#if __linux__
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
#endif

    if (emoEngine.Connect() != EDK_OK) {
        return -1;
    }

    std::string userName = DEFAULT_USERNAME;
    std::string password = DEFAULT_PASSWORD;

    EmotivUser user(userName, password);
    user.login();
    if (user.isLogged()) {
    }
    while(1) {
#if HAVE_CLOUD
        if (!user.isLogged()) {
            if(user.login() == EDK_OK) {
                user.FetchProfile();
            }
            //continue;
        }
#endif
        emoEngine.EventLoop();
        emoEngine.DataLoop();        
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    emoEngine.Disconnect();
    return 0;
}


#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec=0;
    tv.tv_usec=0;

    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);

    if(select(1, &read_fd,NULL, NULL, &tv) == -1)
    return 0;

    if(FD_ISSET(0,&read_fd))
        return 1;
    char tmp;
    read(0, &tmp, 1);
    return 0;
}
#endif

#ifdef __APPLE__
int _kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif


