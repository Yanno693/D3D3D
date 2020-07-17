#include "D3D3DException.h"

D3D3DException::D3D3DException(int _line, const char* _file) noexcept  : line(_line), file(_file) {}

const char* D3D3DException::what() const noexcept {
	std::ostringstream stream;
	stream << getType() << std::endl << getOriginSgring();
	whatBuffer = stream.str();
	return whatBuffer.c_str();
}

const char* D3D3DException::getType() const noexcept {
	return "D3D3D Exeption";
}

int D3D3DException::getLine() const noexcept {
	return line;
}

const std::string& D3D3DException::getFile() const noexcept {
	return file;
}

std::string D3D3DException::getOriginSgring() const noexcept {
	std::ostringstream stream;
	stream << "[File] " << file << std::endl << " [Line] " << line;
	return stream.str();
}