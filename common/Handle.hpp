#ifndef COMMON_HANDLE_HPP
#define COMMON_HANDLE_HPP

#include "common/handle/CHandleObject.hpp"
#include <cstddef>
#include <cstdint>

#include <storm/Handle.hpp>

typedef void* HANDLE;

DECLARE_HANDLE(HOBJECT);

HOBJECT HandleCreate(CHandleObject* ptr);

void HandleClose(HOBJECT handle);

CHandleObject* HandleDereference(HOBJECT handle);

HOBJECT HandleDuplicate(HOBJECT handle);

#endif
