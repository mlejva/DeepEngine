#pragma once

template <typename T>
class FunctionInterface
{
public:

	FunctionInterface()
	{
	}

	virtual ~FunctionInterface()
	{
	}

	virtual T apply(T& param) = 0;
};

