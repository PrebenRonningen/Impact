#pragma once
#include <exception>
#include <string>

class ImpException : public std::exception
{
public:
	ImpException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOrigin() const noexcept;
protected:
	mutable std::string m_WhatBuffer;
private:
	int m_Line;
	std::string m_File;
};