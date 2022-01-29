#pragma once

// Lower memory limit at 4mb, kernel stack grows down back to 0 but we don't need much, 4mb is enough.
#define LOW_MEMORY (4 << 20)