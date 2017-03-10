#pragma once

namespace Functions {
	template <typename T>
	class ActivationFunctionInterface {
	/* Constructor & Destructor */
	public:
		ActivationFunctionInterface() { }
		virtual ~ActivationFunctionInterface() { }

	/* Public Methods */
	public:
		virtual T Apply(const T& param) = 0;
	};
}
