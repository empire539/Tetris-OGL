#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
public:
	std::string errorMsg;

	GameException(const std::string& error = "")
		: errorMsg(error) {
	}

	virtual ~GameException() {}

	const char* what() const {
		return errorMsg.c_str();
	}
};

#endif
