#pragma once
#include <exception>
#include <string>
#include <sstream>

class D3D3DException : public std::exception {
public:
	D3D3DException(int line, const char* file) noexcept;
	const char* what() const noexcept;
	virtual const char* getType() const noexcept;
	int getLine() const noexcept;
	const std::string& getFile() const noexcept;
	std::string getOriginSgring() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};