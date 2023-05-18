## HTTP REST Client
### Implementation by Toaca Alexandra Simona, 322CA 


### Usage
- First, this project uses ```nlohmann's JSON library``` due to its extensive features, ease of use, and compatibility with C++.
- This command is needed for its setup: ```sudo apt install nlohmann-json3-dev```.
- Also, I used **C++ 17** for extra features.
- The ```Makefile``` builds the executable, which is run simply as ```./client```.

### Implementation details
- Due to the server protection mechanisms and Keep-Alive of 5 seconds, the client needs to check for connection before sending requests. That is done via 2 **epoll_wait()** calls.
- Epoll waits for EPOLLIN from STDIN, or EPOLLRDHUP / EPOLLOUT from the server. If the client has a request waiting to be sent and receives an EPOLLRDHUP, it reopens the connection and sends the request.
- For saving requests, I used the **partial application** of different commands (I wanted to experiment with **functional programming** in C++). So, each command is returned
from the method ```parse_command```, partially applied to its custom parameters, the only one left to be transmitted being the **socket file descriptor**.
- Internally, each command has access to the same ```user_t struct```, which stores the ```cookies``` and ```authentication token```.
- Command parsing is done via the extensive use of **regex** expressions. I used regex for both **input validation** and **server response parsing**.
- Also, I used C++ 17 's feature **std::optional** for checking if something was successful or not (for example
if an error was found in the response).
- A walkthrough for a given command would be:
    - receive user input, return proper fields to be completed, parse them as well and check for errors.
    - command is returned and is waiting for its application.
    - when applied, the request is sent to the server.
    - the server response is first checked for an
    ```"error"``` field in the body.
    - if an error was encountered, it is parsed and shown
    to the user on stdout.
    - if everything was ok, the response is parsed for info
    and that info (for example a cookie) is saved in the user_t structure. Then, a message indicating success is shown to the user.




