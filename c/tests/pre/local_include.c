/* 6.10.2p3: "..." searches the including file's directory first;
   #pragma once and include guards. */
#include "local_inc.h"
#include "local_inc.h"
#include "local_guard.h"
#include "local_guard.h"
int count = LOCAL_COUNT;
#include "stddef.h"
size_t z;
