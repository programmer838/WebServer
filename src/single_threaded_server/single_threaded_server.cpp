#include <algorithm>
#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

// Global variable to hold the server socket file descriptor
int server_fd = -1;

std::string get_root_directory() {

    char temp[PATH_MAX];
    return (getcwd(temp, sizeof(temp)) ? std::string(temp) : std::string(""));
}

bool is_directory(const std::string &path) {

    struct stat statbuf;

    if (stat(path.c_str(), &statbuf) != 0)
        return false;

    return S_ISDIR(statbuf.st_mode);
}

// Handler for SIGINT to ensure proper cleanup (Handle ctrl+c cleanup)
void sigint_handler(int sig_num) {
    
     if (server_fd >= 0)
        close(server_fd);
    
    std::cout << "Server exiting.\n";
    exit(0);
}

// Function to URL-decode a string
std::string url_decode(std::string str) {
    
    std::string decoded;
    
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '%' && i + 2 < str.size()) {
            std::string hex = str.substr(i + 1, 2);
            char c = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
            decoded += c;
            i += 2;
        }
        else if (str[i] == '+') {
            decoded += ' ';
        }
        else {
            decoded += str[i];
        }
    }

    return decoded;
}

std::string format_file_size(off_t size) {

    std::ostringstream formatted_size;

    if (size >= 1024 * 1024 * 1024)
        formatted_size << std::fixed << std::setprecision(2) << (size / (1024.0 * 1024.0 * 1024.0)) << " GB";
    else if (size >= 1024 * 1024)
        formatted_size << std::fixed << std::setprecision(2) << (size / (1024.0 * 1024.0)) << " MB";
    else if (size >= 1024)
        formatted_size << std::fixed << std::setprecision(2) << (size / 1024.0) << " KB";
    else
        formatted_size << size << " B";

    return formatted_size.str();
}

std::string get_directory_listing_html(const std::string &directory, const std::string &root_directory) {
     
    std::vector<std::string> files;
    std::vector<std::string> directories;

    DIR *dir;
    struct dirent *ent;

    std::string dir_path = directory; // Assume full path is passed
     
    if ((dir = opendir(dir_path.c_str())) != NULL) {

        while ((ent = readdir(dir)) != NULL) {

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            std::string entry_name = ent->d_name;
            std::string full_path = dir_path + "/" + entry_name;
            
            if (is_directory(full_path))
                directories.push_back(entry_name);
            else 
                files.push_back(entry_name);
        }

        closedir(dir);
    }
    else {
        perror("Could not open directory");
        return "<p>Error: Could not list directory files.</p>";
    }
 
    // Building the HTML string for the directory and file listing page
    std::stringstream html;
    html << "<!DOCTYPE html>\n"
         << "<html lang=\"en\">\n"
         << "<head>\n"
         << "  <meta charset=\"UTF-8\">\n"
         << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
         << "  <title>Index of " << directory << "</title>\n"          
         << "  <style>\n"
         << "    body { background-color: #313336; font-family: Arial, sans-serif; }\n"
         << "    .container { width: 80%; margin: 30px auto; background-color: #323c4a; color: white; padding: 20px; box-shadow: 0 0 10px rgba(0,0,0,0.5); border-radius: 8px; }\n"
         << "    table { width: 100%; border-collapse: collapse; }\n"
         << "    th, td { text-align: left; padding: 8px; }\n"
         << "    tr:nth-child(even) { background-color: #2c3e50; }\n"
         << "    tr.header { background-color: #2980b9; color: white; }\n"
         << "    .flex-container { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; }\n"
         << "    .hidden { display: none; }\n"
         << "    .up-link { color: white; text-decoration: none; }\n" 
         << "    .checkbox { text-align: right; }\n"
         << "  </style>\n"
         << "    <script src=\"https://kit.fontawesome.com/35ca1b15a2.js\" crossorigin=\"anonymous\"></script>\n"
         << "    <script>\n"
         << "    function toggleHidden() {\n"
         << "      var checkBox = document.getElementById('showHidden');\n"
         << "      var hiddenFiles = document.querySelectorAll('.hidden');\n"
         << "      for (var i = 0; i < hiddenFiles.length; i++) {\n"
         << "        if (checkBox.checked) {\n"
         << "          hiddenFiles[i].style.display = 'table-row';\n"
         << "        } else {\n"
         << "          hiddenFiles[i].style.display = 'none';\n"
         << "        }\n"
         << "      }\n"
         << "    }\n"
         << "    </script>\n"	
         << "</head>\n"
         << "<body>\n"
         << "  <div class='container'>\n"
         << "    <h1>File and Directory Overview of " << directory << "</h1>\n"
         << "    <div class='flex-container'>\n"; 

    if (directory != root_directory) {

        size_t last_slash_pos = directory.rfind('/');
        std::string up_path = last_slash_pos != std::string::npos ? directory.substr(0, last_slash_pos) : ".";
        html << "<a href=\"" << up_path << "\" class=\"up-link\"><i class=\"fa-solid fa-folder-tree\" style=\"color: #ffffff;\"></i> Up to higher level directory</a>\n";
    }

    html << "<div class='checkbox'>\n"
         << "<label><input type='checkbox' id='showHidden' onclick='toggleHidden()'> Show hidden files</label>\n"
         << "</div>\n"
         << "</div>\n"
         << "<table>\n"
         << "<tr class='header'><th>Name</th><th>Size</th><th>Last Modified</th></tr>\n";

    for (const auto &dir_name : directories) {

        struct stat stat_buf;
        std::string full_path = dir_path + "/" + dir_name;

        if (stat(full_path.c_str(), &stat_buf) == -1) {
            perror("Could not get file information");
            continue;
        }

        std::string mod_time = std::asctime(std::localtime(&stat_buf.st_mtime));
        mod_time.pop_back(); // Remove the newline at the end

        std::string row_class = (dir_name[0] == '.') ? " class='hidden'" : "";
        html << "<tr" << row_class << "><td><a href=\"" << directory << "/" << dir_name << "\" class='up-link'><i class=\"fa-solid fa-folder-closed\" style=\"color: #f1c40f;\"></i> " << dir_name << "/</a></td><td>-</td><td>" << mod_time << "</td></tr>\n";
    }
    
    for (const auto &file_name : files) {

        struct stat stat_buf;
        std::string full_path = dir_path + "/" + file_name;

        if (stat(full_path.c_str(), &stat_buf) == -1) {
            perror("Could not get file information");
            continue;
        }

        std::string size = format_file_size(stat_buf.st_size);
        std::string mod_time = std::asctime(std::localtime(&stat_buf.st_mtime));
        mod_time.pop_back(); // Remove the newline at the end

        std::string row_class = (file_name[0] == '.') ? " class='hidden'" : "";
        html << "<tr" << row_class << "><td><a href=\"" << directory << "/" << file_name << "\" class='up-link'><i class=\"fa-solid fa-file\" style=\"color: #ecf0f1;\"></i> " << file_name << "</a></td><td>" << size << "</td><td>" << mod_time << "</td></tr>\n";
    }

    html << "</table>\n"
         << "</div>\n"
         << "</body>\n"
         << "</html>";

    return html.str();
}

void serve_file(int socket_fd, const std::string &path) {

    int fd = open(path.c_str(), O_RDONLY);

    if (fd == -1) {
        std::string header = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(socket_fd, header.c_str(), header.size(), 0);
        return;
    }

    struct stat stat_buf;
 
    if (fstat(fd, &stat_buf) == -1) {
        close(fd);
        std::string header = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        send(socket_fd, header.c_str(), header.size(), 0);
        return;
    }

    off_t filesize = stat_buf.st_size;
    std::string content_type = "Content-Type: text/plain\r\n";

    if (path.find(".html") != std::string::npos || path.find(".htm") != std::string::npos)
        content_type = "Content-Type: text/html\r\n";

    // Send the HTTP headers
    std::ostringstream header;
    header << "HTTP/1.1 200 OK\r\n"
           << content_type
           << "Content-Length: " << filesize << "\r\n"
           << "\r\n";
    
    send(socket_fd, header.str().c_str(), header.str().size(), 0);

    off_t offset = 0;
    ssize_t sent;

    while (offset < filesize) {

        sent = sendfile(socket_fd, fd, &offset, filesize - offset);
        
        if (sent <= 0)
            break;
    }

    close(fd);
}

std::string get_current_time() {

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

void log_request(const std::string &ip, const std::string &file_name) {

    std::string current_time = get_current_time();
    std::cout << "Request from " << ip << " for " << file_name << " at " << current_time << std::endl;
}

void run_server(int port) {

    std::string root_directory = get_root_directory(); // Capture the root directory at the start
    
    struct sockaddr_in address;
    int new_socket;
    int addrlen = sizeof(address);
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port " << port << std::endl;

    while (true) {
    
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        
        if (new_socket < 0) {
            perror("In accept");
            continue;
        }
        
        char buffer[30000] = {0};
        ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer));
            
        if (bytes_read < 0) {
            close(new_socket);
            continue;
        }
        
        std::string request(buffer);
        size_t pos = request.find("GET /");
        
        if (pos != std::string::npos) {

            size_t end = request.find(" ", pos + 4);
            std::string path = url_decode(request.substr(pos + 4, end - (pos + 4))); // +4 to include the "/"

            if (path.find("..") != std::string::npos) {

                std::string response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
                send(new_socket, response.c_str(), response.length(), 0);
                close(new_socket);
                continue;
            }

            log_request(inet_ntoa(address.sin_addr), path);

            // After decoding the URL and before any filesystem operations
            if (path.empty() || path == "/") {
                    path = root_directory + "/"; // Use root directory
            }
            else {
                // Check if the path is relative and needs the root directory prepended
                if (path.front() != '/' || path.substr(0, root_directory.size()) != root_directory) {
                    path = root_directory + (path.front() == '/' ? "" : "/") + path;
                }
            }

            // Normalize the path to remove any '..' or similar components that could lead to directory traversal
            char resolved_path[PATH_MAX];
            
            if (realpath(path.c_str(), resolved_path) == nullptr) {
                std::cerr << "Error resolving path: " << path << std::endl;
            }
            else {
                path = std::string(resolved_path);
            }

            // Ensure the path ends with a '/' if it is a directory
            struct stat path_stat;
        
            if (stat(path.c_str(), &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) {
                if (path.back() != '/') {
                    path += '/';
                }
            }

            // Standardize the directory path
            std::replace(path.begin(), path.end(), '\\', '/');
            
            if (path.back() == '/') {
                path.pop_back(); // Remove trailing slash for consistency
            }

            if (is_directory(path)) {
                std::string content = get_directory_listing_html(path, root_directory);
                std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(content.length()) + "\r\n\r\n" + content;
                send(new_socket, response.c_str(), response.length(), 0);
            }
            else {
                serve_file(new_socket, path);
            }
        }
        else {
            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            send(new_socket, response.c_str(), response.length(), 0);
        }

        close(new_socket);
    }

    if (server_fd >= 0) {
        close(server_fd);
    }

    std::cout << "[Server shutdown complete].\n";
}

int main(int argc, char const *argv[]) {

    signal(SIGINT, sigint_handler);

    int port = 8080;

    if (argc > 1) {
        port = std::atoi(argv[1]);
    }

    run_server(port);

    return 0;
}
