#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class GameException : public std::exception
{
public:
    std::string wut;

    GameException() : wut("Generic Game Exception.") {}
    GameException(const std::string &lol) : wut(lol) {}
        
    virtual const char* what() const throw() override
    {
        return wut.c_str();
    }
};


class NoSuchItemException : public GameException
{
  public:
    NoSuchItemException() : GameException("No Such Item.") {}
};

class NoSuchExitException : public GameException
{
  public:
    NoSuchExitException() : GameException("No such exit.") {}
};

class NoSuchActorException : public GameException
{
  public:
    NoSuchActorException() : GameException("No such actor.") {}
};

class InvalidFileException : public GameException
{
public:
    InvalidFileException() : GameException("Invalid file.") {}
    InvalidFileException(const std::string &msg) : GameException("Invalid File: " + msg) {}
    InvalidFileException(unsigned row, const std::string &msg) :
        GameException("Invalid File at Row " + std::to_string(row) + ": " + msg) 
    {
    }
    
};

    

#endif /* EXCEPTIONS_HPP */
