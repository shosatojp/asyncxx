#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#if __GNUC__ >= 9
#include <filesystem>
#else
#include <experimental/filesystem>
namespace std {
namespace filesystem = std::experimental::filesystem;
}
#endif

#define MAX_EVNENTS 100
#define EP_TIMEOOUT 10000
#include <deque>

template <class T>
class Promise {
   public:
    Promise(std::function<void(std::function<void(T result)> resolve,
                               std::function<void(T reason)> reject)>
                fn) {
        fn(this->resolve, this->reject);
    }
    
    ~Promise() {
    }

    Promise<T> then(std::function<void(T result)> fn) {
        this->next = new Promise<T>([&fn, this](auto resolve, auto reject) {
            try {
                fn(this->result);
                resolve(1);
            } catch (const std::exception &e) {
                reject(0);
            }
        });
    }

    // Promise<T> catch_(std::function<void(T error)> fn) {
    //     return *this;
    // }
}

Promise<T> *next;
Promise<T> *error;

private:
std::function<void(T)> resolve = [this](T result) {
    this->result = result;
};
std::function<void(T)> reject = [this](T reason) {
    this->reason = reason;
};

T result;
T reason;
}
;
int main() {
auto p = Promise<int>([](auto resolve, auto reject) {
    std::cout << "run" << std::endl;
    resolve(5);
});
p.then([](auto result) {
    std::cout << "then 1" << std::endl;
});
//     .then([](auto result) {
//         std::cout << "then 2" << std::endl;
//     });
}

// int add_read_file_event(int epfd, const char* path) {
//     create_event(epfd, open(path, O_RDONLY | O_NONBLOCK), EPOLLIN);
// }

// int add_write_file_event(int epfd, const char* path) {
//     create_event(epfd, open(path, O_CREAT | O_RDWR | O_NONBLOCK), EPOLLOUT);
// }

// int add_listen_tcp_event(int epfd, int port) {
//     int sockfd;
//     struct sockaddr_in reader_addr;
//     reader_addr.sin_family = PF_INET;
//     reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     reader_addr.sin_port = htons(port);

//     if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("socket");
//         exit(1);
//     }
//     if (bind(sockfd, (struct sockaddr*)&reader_addr, sizeof reader_addr) < 0) {
//         perror("bind");
//         exit(1);
//     }
//     if (listen(sockfd, SOMAXCONN) < 0) {
//         close(sockfd);
//         perror("listen");
//         exit(1);
//     }
//     create_event(epfd, sockfd, EPOLLIN);
// }

// enum EPOLL_EVENT_TYPE {
//     EPOLL_EVENT_SOCK_ACCEPT,
//     EPOLL_EVENT_SOCK_READ,
//     EPOLL_EVENT_SOCK_WRITE,
//     EPOLL_EVENT_FILE_READ,
//     EPOLL_EVENT_FILE_WRITE,
// };

// int loop(int epfd) {
//     struct epoll_event events[MAX_EVENTS];
//     struct tick_event tick_events[MAX_EVENTS];
//     int tick_events_count = 0;

//     while (1) {
//         // IO Event (wait 1ms)
//         int nfd;
//         if ((nfd = epoll_wait(epfd, events, MAX_EVENTS, 1)) < 0) {
//             perror("epoll_wait");
//             exit(1);
//         }
//         for (int i = 0; i < nfd; i++) {
//             lambda fn;
//             if (fn = find_lambda_by_id(events[i].data.u64)) {
//                 fn();
//             }
//     }       }
//     }
// }
