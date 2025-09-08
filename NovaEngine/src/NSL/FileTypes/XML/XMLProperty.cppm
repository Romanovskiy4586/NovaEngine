export module XMLProperty;
#include "Core.h"

export namespace NSL
{
	class NSL_API XMLProperty
	{
	public:
		XMLProperty() NSL_NOEXCEPT
			: _property()
		{
		}
		XMLProperty(const std::string& property) NSL_NOEXCEPT
			: _property(property)
		{
		}

		const std::string& AsString() const NSL_NOEXCEPT
		{
			return _property;
		}
		bool AsBool() const NSL_NOEXCEPT
		{
			return _property == "true" ? true : false;
		}
		int AsInt() const NSL_NOEXCEPT
		{
			return atoi(_property.c_str());
		}
		float AsFloat() const NSL_NOEXCEPT
		{
			return atof(_property.c_str());
		}

	private:
		std::string _property;
	};
}