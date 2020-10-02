//
//Exceptions.h
//

#include <exception>
#include <string>

class badRecieverException: public std::exception{
	virtual const char* what() const throw()
	{
		return "Neuron's reciever is not valid in the Network";
	}
};

class invalidBoundsException: public std::exception{
	virtual const char* what() const throw()
	{
		return "Index was less than 0 or greater than size";
	}
};

class invalidIndexException: public std::exception{
	virtual const char* what() const throw()
	{
		return "Index did not hold a valid Neuron of the Network";
	}
};

class corruptedNetworkException: public std::exception{
	virtual const char* what() const throw()
	{
		return "The data within the Network has been corrupted";
	}
};
