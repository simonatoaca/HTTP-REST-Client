#include "requests.h"
#include "buffer.h"

#include <vector>
#include <string>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

void compute_message(char *message, const char *line)
{
	strcat(message, line);
	strcat(message, "\r\n");
}

char *compute_get_request(const char *host, const char *url, std::vector<std::string> &cookies,
						 std::string authorization_token)
{
	char *message = (char *)calloc(BUFLEN, sizeof(char));
	char *line = (char *)calloc(LINELEN, sizeof(char));

	// Write the method name, URL and protocol type
	sprintf(line, "GET %s HTTP/1.1", url);

	compute_message(message, line);

	// Add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	// Add authorization token
	if (authorization_token != "") {
		sprintf(line, "Authorization: Bearer %s", authorization_token.c_str());
		compute_message(message, line);	
	}

	// Add cookies
	if (cookies.size()) {
		sprintf(line, "Cookie: %s", cookies[0].c_str());
		for (size_t i = 1; i < cookies.size(); i++) {
			strcat(line, "; ");
			strcat(line, cookies[i].c_str());
		}

		compute_message(message, line);
	}

	// Add final new line
	compute_message(message, "");
	return message;
}

char *compute_post_request(const char *host, const char *url, const char* content_type, const char *body_data,
							std::vector<std::string> &cookies, std::string authorization_token)
{
	char *message = (char *)calloc(BUFLEN, sizeof(char));
	char *line = (char *)calloc(LINELEN, sizeof(char));
	char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));

	// Write the method name, URL and protocol type
	sprintf(line, "POST %s HTTP/1.1", url);
	compute_message(message, line);
	
	// Add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	// Add authorization token
	if (authorization_token != "") {
		sprintf(line, "Authorization: Bearer %s", authorization_token.c_str());
		compute_message(message, line);	
	}

	strcat(body_data_buffer, body_data);

	sprintf(line, "Content-Type: %s", content_type);
	compute_message(message, line);

	sprintf(line, "Content-Length: %lu", strlen(body_data_buffer));
	compute_message(message, line);

	// Add cookies
	if (cookies.size()) {
		sprintf(line, "Cookie: %s", cookies[0].c_str());
		for (size_t i = 1; i < cookies.size(); i++) {
			strcat(line, "; ");
			strcat(line, cookies[i].c_str());
		}

		compute_message(message, line);
	}

	// Add new line at end of header
	compute_message(message, "");

	// Add the actual payload data
	memset(line, 0, LINELEN);
	strcat(message, body_data_buffer);

	free(line);
	return message;
}

char *compute_delete_request(const char *host, const char *url, std::vector<std::string> &cookies,
						 std::string authorization_token)
{
	char *message = (char *)calloc(BUFLEN, sizeof(char));
	char *line = (char *)calloc(LINELEN, sizeof(char));

	// Write the method name, URL and protocol type
	sprintf(line, "DELETE %s HTTP/1.1", url);

	compute_message(message, line);

	// Add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	// Add authorization token
	if (authorization_token != "") {
		sprintf(line, "Authorization: Bearer %s", authorization_token.c_str());
		compute_message(message, line);	
	}

	// Add cookies
	if (cookies.size()) {
		sprintf(line, "Cookie: %s", cookies[0].c_str());
		for (size_t i = 1; i < cookies.size(); i++) {
			strcat(line, "; ");
			strcat(line, cookies[i].c_str());
		}

		compute_message(message, line);
	}

	// Add final new line
	compute_message(message, "");
	return message;
}

void send_to_server(int sockfd, char *message)
{
	int bytes, sent = 0;
	int total = strlen(message);

	do
	{
		bytes = write(sockfd, message + sent, total - sent);
		if (bytes < 0) {
			error("ERROR writing message to socket");
		}

		if (bytes == 0) {
			break;
		}

		sent += bytes;
	} while (sent < total);
}

char *receive_from_server(int sockfd)
{
	char response[BUFLEN];
	buffer buffer = buffer_init();
	int header_end = 0;
	int content_length = 0;

	do {
		int bytes = read(sockfd, response, BUFLEN);

		if (bytes < 0){
			error("ERROR reading response from socket");
		}

		if (bytes == 0) {
			break;
		}

		buffer_add(&buffer, response, (size_t) bytes);
		
		header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

		if (header_end >= 0) {
			header_end += HEADER_TERMINATOR_SIZE;
			
			int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
			
			if (content_length_start < 0) {
				continue;           
			}

			content_length_start += CONTENT_LENGTH_SIZE;
			content_length = strtol(buffer.data + content_length_start, NULL, 10);
			break;
		}
	} while (1);
	size_t total = content_length + (size_t) header_end;
	
	while (buffer.size < total) {
		int bytes = read(sockfd, response, BUFLEN);

		if (bytes < 0) {
			error("ERROR reading response from socket");
		}

		if (bytes == 0) {
			break;
		}

		buffer_add(&buffer, response, (size_t) bytes);
	}
	buffer_add(&buffer, "", 1);
	return buffer.data;
}

char *basic_extract_json_response(char *str)
{
	return strstr(str, "{\"");
}
