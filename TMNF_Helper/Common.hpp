#pragma once
#include <windows.h> 
#include <iostream> 
#include <thread> 
#include <chrono> 
#include <vector> 
#include <functional> 
#include <string> 
#include <map> 
#include <list> 
#include <memory> 
#include <type_traits> 
#include <filesystem> 
#include <ostream> 
#include <fstream> 
#include <sstream> 
#include <iterator> 
#include <algorithm> 
#include <iomanip> 
#include <array> 
#include <regex>
#include <tuple>
#include <set>

using std::string;
using std::vector;
typedef std::vector<uint8_t> binary_t;
typedef int key_t;
typedef std::pair<uint32_t, std::string> pair_t;

#define WINDOW_NAME "TrackMania Helper"
#define LOGGER_FILE "Tm-Helper.log"
#define HEX_ADDR_FMT(addr) std::setw(8) << std::setfill('0') << std::uppercase << std::hex << addr 
#define HEX_ADDR_STR(addr) (static_cast<std::stringstream const&>(std::stringstream() << HEX_ADDR_FMT(addr))).str()

//Defining Helper for Pointers. 
#define READ_PTR(addr) *(int*)addr 
#define READ_PTR_OFF(addr,off) (READ_PTR((addr + off)))
#define READ_PTR_OFF2(addr,off1,off2) *(int*)(READ_PTR_OFF(addr,off1) + off2)
#define READ_PTR_OFF3(addr,off1,off2,off3) *(int*)(READ_PTR_OFF2(addr,off1,off2) + off3)
#define READ_PTR_OFF4(addr,off1,off2,off3,off4) *(int*)(READ_PTR_OFF3(addr,off1,off2,off3) + off4)
#define READ_STATIC_PTR_OFF(addr,off) (READ_PTR(addr) + (DWORD)off)
#define READ_STATIC_PTR_OFF2(addr,off1,off2) READ_PTR_OFF(READ_PTR(addr),off1) + (DWORD)off2 
#define READ_STATIC_PTR_OFF3(addr,off1,off2,off3) READ_PTR_OFF2(READ_PTR(addr),off1,off2) + (DWORD)off3 
#define READ_STATIC_PTR_OFF4(addr,off1,off2,off3,off4) READ_PTR_OFF3(READ_PTR(addr),off1,off2,off3) + (DWORD)off4