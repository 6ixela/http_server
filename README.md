# Multi-Threaded HTTP Server

This project implements a multi-threaded HTTP server in C, designed to handle multiple client requests concurrently. The server is capable of serving static content and includes features like configurable settings and error handling.

## Features

- Multi-threaded architecture for concurrent request handling
- Static file serving
- Configurable server settings
- 404 error handling for not found resources

## Requirements

- GLib library

### Key Directories

- `web_server/`: Contains the main web server implementation
  - `src/`: Source code files
  - `include/`: Header files
  - `www/`: Web content (HTML, CSS, images)
  - `src/config/`: Configuration-related files
  - `src/daemon/`: Daemon process related files
  - `src/server/`: Server process related files

## Building the Project

### 1. Building the Server

To compile and run the server, follow these steps:

1. **Navigate to the server directory:**
   ```bash 
   cd web_server

2. **Build the project:**
    ```bash
    make

3. **Run the server:**
By default, the program uses the `config.txt` file located in the `web_server` directory. But you can use a different configuration file. You can specify it as an argument when launching the server, like so:

    ./http_server [config_file]

## Configuration
The web server can be configured using the `config.txt` file located in the web_server directory. This file allows you to specify various server settings, such as server parameters, file paths, and other options.

Here’s an example configuration entry in the `config.txt` file:
```text 
server_name = my_server
port = 2048
ip = 127.0.0.1
root_dir = www