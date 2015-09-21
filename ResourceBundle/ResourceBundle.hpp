#ifndef RESOURCE_BUNDLE_HPP
#define RESOURCE_BUNDLE_HPP

#include <unordered_map>
#include <utility>
#include <memory>
#include <fstream>

/*
ResourceBundle files are in a binary format:
	Header:
		4 bytes: Number of resources

			List of each resource, with name, and byte offset and length:
				4 bytes: name length
				<name length> bytes: name
				4 bytes: byte offset
				4 bytes: byte length

		Resource data in bytes:
*/

class ResourceBundle
{
	public:
		using Resource = std::pair<std::unique_ptr<char[]>, unsigned int>;

		ResourceBundle() = default;
		~ResourceBundle() = default;

		ResourceBundle(const ResourceBundle&) = delete;

		bool openFile(const std::string& file);

		Resource get(const std::string& name) const;

		std::vector<std::string> contents() const;

	private:
		struct Item
		{
			Item()
			:	dataOffset(0),
				size(0)
			{}

			unsigned int dataOffset;
			unsigned int size;
		};

		std::unordered_map<std::string, Item> data;

		mutable std::ifstream fin;
};

#endif
