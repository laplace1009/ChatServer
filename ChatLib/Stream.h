#pragma once

class Stream
{
public:
	virtual bool Bind() = 0;
	virtual bool Bind(std::string, uint16) = 0;
	virtual bool Connect() = 0;
	virtual bool Recv() = 0;
	virtual bool Send() = 0;
};

