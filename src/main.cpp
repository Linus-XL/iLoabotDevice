#include "iloabot/comm/tcp_server.h"
#include "iloabot/product_factory.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <array>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

namespace {

std::atomic_bool gKeepRunning{true};

#ifdef _WIN32
constexpr char kServiceName[] = "iLoabotDeviceService";

SERVICE_STATUS_HANDLE gServiceStatusHandle = nullptr;
SERVICE_STATUS gServiceStatus{};
#endif

void signalHandler(int) {
    gKeepRunning.store(false);
}

void logMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void runTcpClientSession(Socket client, std::atomic_bool& running) {
    std::array<uint8_t, 1024> buffer{};
    while (running.load()) {
        const int bytes = client.receive(buffer.data(), buffer.size());
        if (bytes <= 0) {
            continue;
        }

        const int sent = client.send(buffer.data(), static_cast<std::size_t>(bytes));
        if (sent <= 0) {
            continue;
        }
    }

    client.close();
}

void runTcpServer(std::atomic_bool& running, uint16_t port) {
    TcpServerConfig config;
    config.bindAddress = "0.0.0.0";
    config.port = port;
    config.backlog = 16;
    config.timeoutMs = 1000;

    TcpServer server(config);
    if (!server.start()) {
        std::cerr << "[TCP] failed to start on port " << port << '\n';
        running.store(false);
        return;
    }

    std::cout << "[TCP] listening on 0.0.0.0:" << port << '\n';
    while (running.load()) {
        Socket client = server.acceptClient();
        if (!client.isValid()) {
            continue;
        }

        std::thread(runTcpClientSession, std::move(client), std::ref(running)).detach();
    }

    server.stop();
}

void runHttpClientSession(tcp::socket socket) {
    beast::error_code ec;
    beast::flat_buffer buffer;
    http::request<http::string_body> request;

    http::read(socket, buffer, request, ec);
    if (ec) {
        return;
    }

    http::response<http::string_body> response{http::status::ok, request.version()};
    response.set(http::field::server, "iLoabotDevice");
    response.set(http::field::content_type, "application/json");

    if (request.target() == "/health") {
        response.body() = "{\"status\":\"ok\",\"service\":\"iLoabotDevice\"}";
    } else {
        response.body() = "{\"message\":\"iLoabotDevice HTTP server is running\"}";
    }

    response.keep_alive(false);
    response.prepare_payload();
    http::write(socket, response, ec);
    socket.shutdown(tcp::socket::shutdown_both, ec);
}

void runHttpServer(std::atomic_bool& running, uint16_t port) {
    asio::io_context ioContext;
    beast::error_code ec;
    tcp::acceptor acceptor(ioContext);
    const tcp::endpoint endpoint{tcp::v4(), port};

    acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr << "[HTTP] open failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.set_option(asio::socket_base::reuse_address(true), ec);
    if (ec) {
        std::cerr << "[HTTP] set_option failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.bind(endpoint, ec);
    if (ec) {
        std::cerr << "[HTTP] bind failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "[HTTP] listen failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.non_blocking(true, ec);
    if (ec) {
        std::cerr << "[HTTP] non_blocking failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    std::cout << "[HTTP] listening on 0.0.0.0:" << port << '\n';
    while (running.load()) {
        tcp::socket socket(ioContext);
        acceptor.accept(socket, ec);
        if (ec == asio::error::would_block || ec == asio::error::try_again) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        if (ec) {
            std::cerr << "[HTTP] accept error: " << ec.message() << '\n';
            continue;
        }

        std::thread(runHttpClientSession, std::move(socket)).detach();
    }

    acceptor.close(ec);
}

void runWebSocketClientSession(tcp::socket socket) {
    beast::error_code ec;
    websocket::stream<tcp::socket> ws(std::move(socket));
    ws.accept(ec);
    if (ec) {
        return;
    }

    beast::flat_buffer buffer;
    while (true) {
        ws.read(buffer, ec);
        if (ec == websocket::error::closed) {
            break;
        }
        if (ec) {
            break;
        }

        ws.text(ws.got_text());
        ws.write(buffer.data(), ec);
        if (ec) {
            break;
        }
        buffer.consume(buffer.size());
    }

    ws.close(websocket::close_code::normal, ec);
}

void runWebSocketServer(std::atomic_bool& running, uint16_t port) {
    asio::io_context ioContext;
    beast::error_code ec;
    tcp::acceptor acceptor(ioContext);
    const tcp::endpoint endpoint{tcp::v4(), port};

    acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr << "[WS] open failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.set_option(asio::socket_base::reuse_address(true), ec);
    if (ec) {
        std::cerr << "[WS] set_option failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.bind(endpoint, ec);
    if (ec) {
        std::cerr << "[WS] bind failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "[WS] listen failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    acceptor.non_blocking(true, ec);
    if (ec) {
        std::cerr << "[WS] non_blocking failed: " << ec.message() << '\n';
        running.store(false);
        return;
    }

    std::cout << "[WS] listening on 0.0.0.0:" << port << '\n';
    while (running.load()) {
        tcp::socket socket(ioContext);
        acceptor.accept(socket, ec);
        if (ec == asio::error::would_block || ec == asio::error::try_again) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        if (ec) {
            std::cerr << "[WS] accept error: " << ec.message() << '\n';
            continue;
        }

        std::thread(runWebSocketClientSession, std::move(socket)).detach();
    }

    acceptor.close(ec);
}

int runServerLoop() {
    iloabot::factories::registerILoabotProductFactories();

    constexpr uint16_t kTcpPort = 9000;
    constexpr uint16_t kHttpPort = 8080;
    constexpr uint16_t kWsPort = 8081;

    gKeepRunning.store(true);

    logMessage("iLoabotDevice service starting...");

    std::vector<std::thread> workers;
    workers.emplace_back(runTcpServer, std::ref(gKeepRunning), kTcpPort);
    workers.emplace_back(runHttpServer, std::ref(gKeepRunning), kHttpPort);
    workers.emplace_back(runWebSocketServer, std::ref(gKeepRunning), kWsPort);

    logMessage("Service is running.");

    while (gKeepRunning.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    logMessage("Service stopping...");
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    logMessage("Service stopped.");
    return 0;
}

#ifdef _WIN32
void updateServiceStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint) {
    if (gServiceStatusHandle == nullptr) {
        return;
    }

    gServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    gServiceStatus.dwCurrentState = currentState;
    gServiceStatus.dwWin32ExitCode = win32ExitCode;
    gServiceStatus.dwWaitHint = waitHint;
    gServiceStatus.dwControlsAccepted =
        currentState == SERVICE_START_PENDING ? 0 : (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
    gServiceStatus.dwCheckPoint =
        (currentState == SERVICE_RUNNING || currentState == SERVICE_STOPPED) ? 0 : gServiceStatus.dwCheckPoint + 1;

    SetServiceStatus(gServiceStatusHandle, &gServiceStatus);
}

void WINAPI serviceControlHandler(DWORD controlCode) {
    switch (controlCode) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
            updateServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);
            gKeepRunning.store(false);
            break;
        default:
            break;
    }
}

void WINAPI serviceMain(DWORD, LPSTR*) {
    gServiceStatusHandle = RegisterServiceCtrlHandlerA(kServiceName, serviceControlHandler);
    if (gServiceStatusHandle == nullptr) {
        return;
    }

    gServiceStatus = {};
    updateServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    const int exitCode = runServerLoop();
    updateServiceStatus(SERVICE_STOPPED, exitCode == 0 ? NO_ERROR : ERROR_SERVICE_SPECIFIC_ERROR, 0);
}

std::string getExecutablePath() {
    std::string path(MAX_PATH, '\0');
    const DWORD length = GetModuleFileNameA(nullptr, path.data(), static_cast<DWORD>(path.size()));
    path.resize(length);
    return path;
}

bool installWindowsService() {
    SC_HANDLE scm = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
    if (scm == nullptr) {
        std::cerr << "OpenSCManager failed: " << GetLastError() << '\n';
        return false;
    }

    const std::string binaryPath = "\"" + getExecutablePath() + "\"";
    SC_HANDLE service = CreateServiceA(
        scm,
        kServiceName,
        kServiceName,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        binaryPath.c_str(),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr);

    if (service == nullptr) {
        const DWORD error = GetLastError();
        std::cerr << "CreateService failed: " << error << '\n';
        CloseServiceHandle(scm);
        return false;
    }

    SERVICE_DESCRIPTIONA description{};
    std::string descriptionText = "iLoabotDevice background service with TCP, HTTP and WebSocket listeners.";
    description.lpDescription = descriptionText.data();
    ChangeServiceConfig2A(service, SERVICE_CONFIG_DESCRIPTION, &description);

    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return true;
}

bool uninstallWindowsService() {
    SC_HANDLE scm = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (scm == nullptr) {
        std::cerr << "OpenSCManager failed: " << GetLastError() << '\n';
        return false;
    }

    SC_HANDLE service = OpenServiceA(scm, kServiceName, DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (service == nullptr) {
        std::cerr << "OpenService failed: " << GetLastError() << '\n';
        CloseServiceHandle(scm);
        return false;
    }

    SERVICE_STATUS status{};
    ControlService(service, SERVICE_CONTROL_STOP, &status);
    const bool deleted = DeleteService(service) != 0;
    if (!deleted) {
        std::cerr << "DeleteService failed: " << GetLastError() << '\n';
    }

    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return deleted;
}
#endif

}  // namespace

int main(int argc, char* argv[]) {
    try {
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

#ifdef _WIN32
        if (argc > 1) {
            const std::string command = argv[1];
            if (command == "--install") {
                return installWindowsService() ? 0 : -1;
            }
            if (command == "--uninstall") {
                return uninstallWindowsService() ? 0 : -1;
            }
            if (command == "--console") {
                return runServerLoop();
            }
        }

        SERVICE_TABLE_ENTRYA serviceTable[] = {
            {const_cast<LPSTR>(kServiceName), serviceMain},
            {nullptr, nullptr},
        };

        if (StartServiceCtrlDispatcherA(serviceTable) != 0) {
            return 0;
        }

        if (GetLastError() != ERROR_FAILED_SERVICE_CONTROLLER_CONNECT) {
            std::cerr << "StartServiceCtrlDispatcher failed: " << GetLastError() << '\n';
            return -1;
        }
#endif

        return runServerLoop();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return -1;
    }
}
