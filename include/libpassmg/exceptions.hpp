#include <exception>
#include <string>

#ifndef _PASSMG_EXCEPTIONS_
#define _PASSMG_EXCEPTIONS_

class PasswordManagerException : public std::exception {
    public:
        explicit PasswordManagerException(const char *message)
            : msg_(message) {}

        explicit PasswordManagerException(const std::string &message)
            : msg_(message) {}

        virtual ~PasswordManagerException() {}

        const char *what() const noexcept override { return msg_.c_str(); }

    protected:
        std::string msg_;
};

class DatabaseException : public PasswordManagerException {
    public:
        explicit DatabaseException(const char *message)
            : PasswordManagerException(message) {}

        explicit DatabaseException(const std::string &message)
            : PasswordManagerException(message) {}

        explicit DatabaseException()
            : PasswordManagerException("A DatabaseException has accured") {}
};

class IntegrityException : public DatabaseException {
    public:
        explicit IntegrityException(const char *message)
            : DatabaseException(message) {}

        explicit IntegrityException(std::string message)
            : DatabaseException(message) {}

        explicit IntegrityException()
            : DatabaseException("IntegrityException has accured") {}
};

class InvalidInputException : public PasswordManagerException {
    public:
        explicit InvalidInputException(const char *message)
            : PasswordManagerException(message) {}

        explicit InvalidInputException(const std::string &message)
            : PasswordManagerException(message) {}

        explicit InvalidInputException()
            : PasswordManagerException("A DatabaseException has accured") {}
};

class WrongCredentialsException : public PasswordManagerException {
    public:
        explicit WrongCredentialsException(const char *message)
            : PasswordManagerException(message) {}

        explicit WrongCredentialsException(const std::string &message)
            : PasswordManagerException(message) {}

        explicit WrongCredentialsException()
            : PasswordManagerException("Username or password are incorrect") {}
};

class AuthenticationRequired : public PasswordManagerException {
    public:
        explicit AuthenticationRequired()
            : PasswordManagerException("Authentication required") {}
};

#endif // !_PASSMG_EXCEPTIONS_
