#include "iloabot/c_api.h"

#include "iloabot/device.h"
#include "iloabot/devices/iloabot_device.h"
#include "iloabot/product_factory.h"

#include <memory>
#include <mutex>
#include <new>
#include <string>

namespace {

std::once_flag g_factoryInitOnce;
thread_local std::string g_lastError;
thread_local std::string g_resultBuffer;

} // namespace

struct ILoabotHandle {
    ILoabotDevice device;

    ILoabotHandle()
        : device(iloabot::factories::ProductFactoryRegistry::instance()) {}
};

struct ILoabotAssembledHandle {
    std::unique_ptr<AssembledDevice> assembled;
};

namespace {

void setError(const char* message) {
    g_lastError = message ? message : "unknown error";
}

void clearError() {
    g_lastError.clear();
}

void ensureFactoriesRegistered() {
    std::call_once(g_factoryInitOnce, [] {
        iloabot::factories::registerILoabotProductFactories();
    });
}

const DeviceProduct* getProductAt(const ILoabotAssembledHandle* assembled, int index) {
    if (!assembled || !assembled->assembled) {
        setError("assembled handle is null");
        return nullptr;
    }
    if (index < 0) {
        setError("index out of range");
        return nullptr;
    }

    const auto& products = assembled->assembled->products();
    const auto idx = static_cast<size_t>(index);
    if (idx >= products.size()) {
        setError("index out of range");
        return nullptr;
    }

    const auto& ptr = products[idx];
    if (!ptr) {
        setError("product is null");
        return nullptr;
    }

    clearError();
    return ptr.get();
}

} // namespace

extern "C" {

ILoabotHandle* ILOABOT_CALL iloabot_create(void) {
    try {
        ensureFactoriesRegistered();
        clearError();
        return new ILoabotHandle();
    } catch (const std::exception& ex) {
        setError(ex.what());
    } catch (...) {
        setError("failed to create iLoabot handle");
    }
    return nullptr;
}

void ILOABOT_CALL iloabot_destroy(ILoabotHandle* handle) {
    delete handle;
}

ILoabotAssembledHandle* ILOABOT_CALL iloabot_assemble(
    ILoabotHandle* handle,
    const char* robotModel,
    const char* agvModel,
    const char* suctionCupModel,
    const char* batteryModel,
    const char* chargerModel,
    const char* lightModel,
    const char* plcModel) {
    if (!handle) {
        setError("handle is null");
        return nullptr;
    }

    try {
        DeviceBuildSpec spec;
        if (robotModel) {
            spec.robotModel = robotModel;
        }
        if (agvModel) {
            spec.agvModel = agvModel;
        }
        if (suctionCupModel) {
            spec.suctionCupModel = suctionCupModel;
        }
        if (batteryModel) {
            spec.batteryModel = batteryModel;
        }
        if (chargerModel) {
            spec.chargerModel = chargerModel;
        }
        if (lightModel) {
            spec.lightModel = lightModel;
        }
        if (plcModel) {
            spec.plcModel = plcModel;
        }

        auto assembled = std::make_unique<ILoabotAssembledHandle>();
        assembled->assembled = handle->device.assemble(spec);
        if (!assembled->assembled) {
            setError("assemble returned null");
            return nullptr;
        }

        clearError();
        return assembled.release();
    } catch (const std::exception& ex) {
        setError(ex.what());
    } catch (...) {
        setError("failed to assemble device");
    }

    return nullptr;
}

void ILOABOT_CALL iloabot_assembled_destroy(ILoabotAssembledHandle* assembled) {
    delete assembled;
}

int ILOABOT_CALL iloabot_assembled_product_count(const ILoabotAssembledHandle* assembled) {
    if (!assembled || !assembled->assembled) {
        setError("assembled handle is null");
        return -1;
    }

    clearError();
    return static_cast<int>(assembled->assembled->products().size());
}

const char* ILOABOT_CALL iloabot_assembled_product_type(const ILoabotAssembledHandle* assembled, int index) {
    const auto* product = getProductAt(assembled, index);
    if (!product) {
        return nullptr;
    }
    g_resultBuffer = product->type();
    return g_resultBuffer.c_str();
}

const char* ILOABOT_CALL iloabot_assembled_product_model(const ILoabotAssembledHandle* assembled, int index) {
    const auto* product = getProductAt(assembled, index);
    if (!product) {
        return nullptr;
    }
    g_resultBuffer = product->model();
    return g_resultBuffer.c_str();
}

const char* ILOABOT_CALL iloabot_assembled_product_name(const ILoabotAssembledHandle* assembled, int index) {
    const auto* product = getProductAt(assembled, index);
    if (!product) {
        return nullptr;
    }
    g_resultBuffer = product->name();
    return g_resultBuffer.c_str();
}

const char* ILOABOT_CALL iloabot_last_error(void) {
    return g_lastError.empty() ? nullptr : g_lastError.c_str();
}

} // extern "C"
