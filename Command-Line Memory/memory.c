#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int read_bytes(int infile, uint8_t *buf, int amount_to_read) {
    if (amount_to_read < 1 || buf == NULL) {
        return -1;
    }

    int num_bytes_read = 0;

    while (amount_to_read > 0) {
        int result = read(infile, &buf[num_bytes_read], amount_to_read);

        // error
        if (result < 0) {
            return result;
        }

        // end of file
        if (result == 0) {
            break;
        }

        num_bytes_read += result;
        amount_to_read -= result;
    }

    return num_bytes_read;
}

int write_bytes(int outfile, uint8_t *buf, int amount_to_write) {
    if (amount_to_write < 1 || buf == NULL) {
        return -1;
    }

    int num_bytes_written = 0;

    while (amount_to_write > 0) {
        int result = write(outfile, &buf[num_bytes_written], amount_to_write);

        // error
        if (result < 0) {
            return result;
        }

        num_bytes_written += result;
        amount_to_write -= result;
    }

    return num_bytes_written;
}

bool validate_command(const char *command) {
    if (command == NULL) {
        return false;
    }

    if (strcmp(command, "get") == 0 || strcmp(command, "set") == 0) {
        return true;
    }

    return false;
}

bool validate_location(const char *location) {
    if (location == NULL || strlen(location) >= PATH_MAX) {
        return false;
    }

    struct stat location_info;

    // checks if stat operation is successful
    if (stat(location, &location_info) == 0) {
        if (S_ISDIR(location_info.st_mode)) {
            return false;
        }
    }

    return true;
}

int execute_get(uint8_t *buf, char *location) {
    int fd = open(location, O_RDONLY);

    // if opening file fails
    if (fd == -1) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    int result;

    // reading the file & writing to the stdout until no more bytes left
    while ((result = read_bytes(fd, buf, BUFFER_SIZE)) > 0) {
        write_bytes(1, buf, result);
    }

    close(fd);

    if (result == -1) {
        fprintf(stderr, "Operation Failed\n");
        return 1;
    }

    return 0;
}

int execute_set(uint8_t *buf, int buf_index, uint64_t bytes_in_buffer, char *location,
    uint64_t content_length) {
    // open file for writing; if file doesn't exist, create new one & if file exists, truncate its content
    int fd = open(location, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // if opening file fails
    if (fd == -1) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    // entire reading/writing process
    while (content_length > 0) {
        if (bytes_in_buffer > content_length) {
            bytes_in_buffer = content_length;
        }

        if (bytes_in_buffer > 0) {
            int write_result = write_bytes(fd, &buf[buf_index], bytes_in_buffer);

            if (write_result == -1) {
                fprintf(stderr, "Operation Failed\n");
                close(fd);
                return 1;
            }

            else {
                if (bytes_in_buffer < (BUFFER_SIZE - (uint64_t) buf_index)) {
                    content_length = 0;
                }

                else {
                    content_length -= write_result;
                    bytes_in_buffer -= write_result;
                    buf_index += write_result;
                }
            }
        }

        else {
            // amount to read is buffer size or content length based if its less or greater than
            int amount_to_read = content_length > BUFFER_SIZE ? BUFFER_SIZE : content_length;
            int read_result = read_bytes(0, buf, amount_to_read);

            if (read_result == -1) {
                fprintf(stderr, "Operation Failed\n");
                close(fd);
                return 1;
            }

            // no more data left
            else if (read_result == 0) {
                break;
            }

            else {
                bytes_in_buffer = read_result;
                buf_index = 0;
                if (read_result < amount_to_read) {
                    content_length = read_result;
                }
            }
        }
    }

    fprintf(stdout, "OK\n");
    close(fd);
    return 0;
}

int main(void) {
    uint8_t buf[BUFFER_SIZE] = { 0 };
    int buf_index = 0;
    int result = read_bytes(0, buf, BUFFER_SIZE);

    // when reading bytes failed (negative number)
    if (result < 0) {
        fprintf(stderr, "Operation Failed\n");
        return 1;
    }

    // parsing the buffer
    char *command = strtok((char *) buf, "\n");
    if (!validate_command(command)) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    // keeps track of the amount of data in buffer that's been read
    buf_index = buf_index + strlen(command) + 1;

    // remembers previous strtok use (NULL assumes to start at current loc)
    char *location = strtok(NULL, "\n");

    if (!validate_location(location)) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    // updating the buf_index
    buf_index = buf_index + strlen(location) + 1;

    uint64_t content_length = 0;

    // checking if given command is set
    if (strcmp(command, "set") == 0) {
        char *content_length_str = strtok(NULL, "\n");

        if (content_length_str == NULL) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        content_length = strtoul(content_length_str, NULL, 10);

        if (errno == ERANGE || content_length < 0) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        buf_index = buf_index + strlen(content_length_str) + 1;

        if (buf_index > result) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        return execute_set(buf, buf_index, result - buf_index, location, content_length);
    }

    // occurs if the command given is get
    else {
        // if command has no newline or any extra arguments
        if (buf_index != result) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }
        return execute_get(buf, location);
    }

    return 0;
}
