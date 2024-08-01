#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <lua.hpp>

std::vector<std::string> documents = 
{
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
    "Integer nec odio. Praesent libero. Sed cursus ante dapibus diam.",
    "Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi.",
};

void performSearch(const std::string& luaQuery) 
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_newtable(L);
    for (size_t i = 0; i < documents.size(); ++i) 
    {
        lua_pushnumber(L, i + 1);
        lua_pushstring(L, documents[i].c_str());
        lua_settable(L, -3);
    }
    lua_setglobal(L, "documents");

    if (luaL_dostring(L, luaQuery.c_str()) != LUA_OK) 
    {
        std::cerr << "Error executing Lua query: " << lua_tostring(L, -1) << std::endl;
    }

    if (lua_istable(L, -1)) 
    {
        int tableSize = lua_rawlen(L, -1);
        std::cout << "Search Results:" << std::endl;
        for (int i = 1; i <= tableSize; ++i)
            {
            lua_pushnumber(L, i);
            lua_gettable(L, -2);
            int documentIndex = lua_tonumber(L, -1);
            std::cout << "Document " << documentIndex << ": " << documents[documentIndex - 1] << std::endl;
            lua_pop(L, 1);
        }
    }
    else{std::cerr << "Invalid result from Lua query." << std::endl;}

    lua_close(L);
}

int main() 
{
    std::ifstream file("query.lua");
    if (!file.is_open()) 
    {std::cerr << "Error opening query.lua" << std::endl;return 1;}

    std::string query;
    std::string line;
    while (std::getline(file, line)) 
    {
        query += line + "\n";
    }

    file.close();

    performSearch(query);

    return 0;
}
