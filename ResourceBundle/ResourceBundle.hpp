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
			4 bytes: byte offset (from beginning of file)
			4 bytes: byte length

	Resource data:
*/

/*
Class holding an open file handle to a resource file. Data is only extracted upon request, ie:
The whole file is not read and stored in memory.
A call to ResourceBundle::get reads the bytes from the file and returns a pointer to said bytes.
A call to the same resource name WILL read the file again.
*/
class ResourceBundle
{
	public:
		// debating about unique_ptr now... Works fine for APIs that end up copying the memory anyway.
		// but some APIs may not copy. Unless they're already set up to use smart pointers, this causes an issue.
		using Resource = std::pair<std::unique_ptr<char[]>, unsigned int>;

		ResourceBundle() = default;
		~ResourceBundle() = default;

		// Non-copyable
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
