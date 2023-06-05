#include <iostream>
#include <string>
#include <fstream>
#include "Cavern.h"

void readCAV(const char* name, std::vector<std::shared_ptr<Cavern>>& __restrict caverns);
void writeCSN(const char* name, const std::vector<int>& __restrict solution);