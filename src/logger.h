#pragma once

#define LOG_DEBUG(...) \
printf("[%s:%u:%s] DEBUG ", __FILE__, __LINE__, __FUNCTION__); \
printf(__VA_ARGS__); \
printf("\n");