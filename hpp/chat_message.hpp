#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class chat_message
{
public:
    enum {header_length = 4};
    enum {username_header_length = 2};
    enum {max_username_length = 20};
    enum {max_body_length = 512};

    chat_message(): body_length_(0), username_length_(0) {}

    // write data buffer
    const char *data() const{ return data_; }

    // read data buffer
    char *data() { return data_; }

    std::size_t length() const
    {
        return header_length + username_header_length + max_username_length + body_length_;
    }

    // write body buffer
    const char *body() const
    {
        return data_ + header_length + username_header_length + max_username_length;
    }

    // read body buffer
    char *body()
    {
        return data_ + header_length + username_header_length + max_username_length;
    }

    // write username buffer
    const char *username() const
    {
        return data_ + header_length + username_header_length;
    }

    // read username buffer
    char *username()
    {
        return data_ + header_length + username_header_length;
    }

    std::size_t body_length() const
    {
        return body_length_;
    }

    void body_length(std::size_t new_length)
    {
        body_length_ = new_length;
        if (body_length_ > max_body_length)
            body_length_ = max_body_length;
    }


    std::size_t username_length() const
    {
        return username_length_;
    }

    void username_length(std::size_t new_username_length)
    {
        username_length_ = new_username_length;
        if (username_length_ > max_username_length)
            username_length_ = max_username_length;
    }

    // decode body length
    bool decode_header()
    {
        char header[header_length + 1] = "";
        std::strncat(header, data_, header_length);
        body_length_ = std::atoi(header);
        if (body_length_ > max_body_length)
        {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    // decode username length
    bool decode_username_header()
    {
        char username_header[username_header_length + 1] = "";
        std::strncat(username_header, data_+header_length, username_header_length);
        username_length_ = std::atoi(username_header);
        if (username_length_ > max_username_length)
        {
            username_length_ = 0;
            return false;
        }
        return true;
    }

    // to store body length
    void encode_header()
    {
        char header[header_length + 1] = "";
        std::sprintf(header, "%4d", static_cast<int>(body_length_));
        std::memcpy(data_, header, header_length);
    }

    // to store username length
    void encode_username_header()
    {
        char username_header[username_header_length + 1] = "";
        std::sprintf(username_header, "%2d", static_cast<int>(username_length_));
        std::memcpy(data_+header_length, username_header, username_header_length);
    }
 
private:
    char data_[header_length + max_username_length + max_body_length];
    std::size_t body_length_;
    std::size_t username_length_;
};

#endif // CHAT_MESSAGE_HPP
