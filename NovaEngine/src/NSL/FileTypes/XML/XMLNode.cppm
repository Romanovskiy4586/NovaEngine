export module XMLNode;
#include "Core.h"
export import XMLProperty;

namespace NSL
{
	class NSL_API XMLNode
	{
	public:
		const XMLNode& Child(const std::string& name) const NSL_NOEXCEPT
		{
			for (auto& i : _children) if (i.first == name) return i.second;
		}
		std::vector<const XMLNode*> Children(const std::string& name) const NSL_NOEXCEPT
		{
			std::vector<const XMLNode*> children;
			for (auto& i : _children) if (i.first == name) children.push_back(&i.second);
			return children;
		}
		const XMLProperty& Attribute(const std::string& name) const NSL_NOEXCEPT
		{
			return _attributes.at(name);
		}
		const XMLProperty& Element() const NSL_NOEXCEPT
		{
			return _element;
		}

		void AddChild(XMLNode&& node) NSL_NOEXCEPT;
		void AddAttribute(const std::string& name, const std::string& value) NSL_NOEXCEPT;

	private:
		std::vector<std::pair<std::string, XMLNode>> _children;
		std::map<std::string, XMLProperty> _attributes;
		XMLProperty _element;
	};
}