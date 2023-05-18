#pragma once

#define MAX_CONNECTIONS 2
#define EPOLL_TIMEOUT 1000

int init_connection();
int add_to_epoll(int epollfd, int fd, int flags);
int remove_from_epoll(int epollfd, int fd, int flags);