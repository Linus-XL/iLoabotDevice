#pragma once

#include <stddef.h>

#if defined(_WIN32)
#if defined(iLoabotDevice_EXPORTS)
#define ILOABOT_API __declspec(dllexport)
#else
#define ILOABOT_API __declspec(dllimport)
#endif
#define ILOABOT_CALL __cdecl
#else
#define ILOABOT_API
#define ILOABOT_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ILoabotHandle ILoabotHandle;
typedef struct ILoabotAssembledHandle ILoabotAssembledHandle;

ILOABOT_API ILoabotHandle* ILOABOT_CALL iloabot_create(void);
ILOABOT_API void ILOABOT_CALL iloabot_destroy(ILoabotHandle* handle);

ILOABOT_API ILoabotAssembledHandle* ILOABOT_CALL iloabot_assemble(
    ILoabotHandle* handle,
    const char* robotModel,
    const char* agvModel,
    const char* suctionCupModel,
    const char* batteryModel,
    const char* chargerModel,
    const char* lightModel,
    const char* plcModel);
ILOABOT_API void ILOABOT_CALL iloabot_assembled_destroy(ILoabotAssembledHandle* assembled);

ILOABOT_API int ILOABOT_CALL iloabot_assembled_product_count(const ILoabotAssembledHandle* assembled);
ILOABOT_API const char* ILOABOT_CALL iloabot_assembled_product_type(
    const ILoabotAssembledHandle* assembled,
    int index);
ILOABOT_API const char* ILOABOT_CALL iloabot_assembled_product_model(
    const ILoabotAssembledHandle* assembled,
    int index);
ILOABOT_API const char* ILOABOT_CALL iloabot_assembled_product_name(
    const ILoabotAssembledHandle* assembled,
    int index);

ILOABOT_API const char* ILOABOT_CALL iloabot_last_error(void);

#ifdef __cplusplus
}
#endif
