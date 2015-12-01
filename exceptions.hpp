#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class GameException : public std::exception
{
public:
    virtual const char* what() const throw() override
    {
        return "Generic Game Exception.";
    }
};

class NoSuchItemException : public GameException
{
  public:
    virtual const char* what() const throw() override
    {
        return "No such item.";
    }
};

class NoSuchExitException : public GameException
{
  public:
    virtual const char* what() const throw() override
    {
        return "No such exit.";
    }
};

class NoSuchActorException : public GameException
{
  public:
    virtual const char* what() const throw() override
    {
        return "No such actor.";
    }
};

#endif /* EXCEPTIONS_HPP */
