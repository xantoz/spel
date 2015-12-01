#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class NoSuchItemException : public std::exception
{
  public:
    virtual const char* what() const throw() override
    {
        return "No such item";
    }
};

class NoSuchExitException : public std::exception
{
  public:
    virtual const char* what() const throw() override
    {
        return "No such exit";
    }
};

class NoSuchActorException : public std::exception
{
  public:
    virtual const char* what() const throw() override
    {
        return "No such actor";
    }
};

#endif /* EXCEPTIONS_HPP */
