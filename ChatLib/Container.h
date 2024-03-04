#pragma once
#include "Types.h"
#include <vector>
#include "StlAllocator.h"

template<typename Type>
using Vector = std::vector<Type, StlAllocator<Type>>;