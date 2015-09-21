#include "ResourceBundle.hpp"

bool ResourceBundle::openFile(const std::string& file)
{
	if(!data.empty())
		data.clear();

	fin.open(file, std::ios::binary);

	if(!fin)
		return false;

	unsigned int numRes = 0;
	fin.read(reinterpret_cast<char*>(&numRes), sizeof(numRes));

	data.reserve(numRes);

	for(auto i = 0u; i < numRes; ++i)
	{
		Item item;

		unsigned int nameSize = 0;
		fin.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

		std::string name(nameSize, '\0');
		fin.read(&name[0], nameSize);

		fin.read(reinterpret_cast<char*>(&item.dataOffset), sizeof(item.dataOffset));
		fin.read(reinterpret_cast<char*>(&item.size), sizeof(item.size));

		data.emplace(name, item);
	}
}

ResourceBundle::Resource ResourceBundle::get(const std::string& name) const
{
	auto it = data.find(name);

	if(it == data.end())
		return {nullptr, 0};

	fin.seekg(it->second.dataOffset);

	Resource res;
	res.first = std::make_unique<char[]>(it->second.size);
	res.second = it->second.size;

	fin.read(res.first.get(), res.second);

	return res;
}

std::vector<std::string> ResourceBundle::contents() const
{
	std::vector<std::string> keys;

	for(auto& d : data)
		keys.push_back(d.first);

	return keys;
}