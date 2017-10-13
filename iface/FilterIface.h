#pragma once

#include <memory>
#include <string>
#include <vector>


namespace glucose {

	class IFilter_Pipe {
	public:
	};

	using SFilter_Pipe = std::shared_ptr<IFilter_Pipe>;

	class IFilter {
	public:
	};

	using SFilter =  std::shared_ptr<IFilter>;

	class IFilter_Factory {
	public:
		virtual std::wstring description() const = 0;
			//returns filter description

		virtual SFilter create_filter(SFilter_Pipe &input, SFilter_Pipe &output) const = 0;
			//instiantiate a filter and connects it to its input and output
	};

	using SFilter_Factory = std::shared_ptr<IFilter_Factory>;
}