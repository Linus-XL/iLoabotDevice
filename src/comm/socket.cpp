#include "iloabot/comm/socket.h"

#include <boost/asio.hpp>

#include <chrono>
#include <memory>
#include <string>

namespace {

using boost::asio::ip::tcp;

template <typename StartOperation, typename CancelOperation>
bool runTimedOperation(
    boost::asio::io_context& ioContext,
    int timeoutMs,
    StartOperation&& startOperation,
    CancelOperation&& cancelOperation,
    boost::system::error_code& ec,
    std::size_t* transferred = nullptr) {
    ioContext.restart();

    bool completed = false;
    bool timedOut = false;
    boost::asio::steady_timer timer(ioContext);

    if (timeoutMs > 0) {
        timer.expires_after(std::chrono::milliseconds(timeoutMs));
        timer.async_wait([&](const boost::system::error_code& timerEc) {
            if (!timerEc && !completed) {
                timedOut = true;
                cancelOperation();
            }
        });
    }

    startOperation([&](const boost::system::error_code& operationEc, std::size_t bytesTransferred) {
        completed = true;
        ec = operationEc;
        if (transferred != nullptr) {
            *transferred = bytesTransferred;
        }

        if (timeoutMs > 0) {
            boost::system::error_code ignoredEc;
            timer.cancel();
        }
    });

    ioContext.run();

    if (timedOut && ec == boost::asio::error::operation_aborted) {
        ec = boost::asio::error::timed_out;
    }

    return completed && !ec;
}

tcp::endpoint makeEndpoint(const std::string& address, uint16_t port, boost::system::error_code& ec) {
    if (address.empty() || address == "0.0.0.0") {
        return tcp::endpoint(tcp::v4(), port);
    }

    const auto ipAddress = boost::asio::ip::make_address(address, ec);
    if (ec) {
        return tcp::endpoint();
    }

    return tcp::endpoint(ipAddress, port);
}

}  // namespace

struct Socket::Impl {
    boost::asio::io_context ioContext;
    std::unique_ptr<tcp::socket> socket;
    std::unique_ptr<tcp::acceptor> acceptor;
    bool reuseAddr = false;
    bool nonBlocking = false;
    int timeoutMs = 0;
};

Socket::Socket() = default;

Socket::Socket(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}

Socket::~Socket() { close(); }

Socket::Socket(Socket&& other) noexcept = default;

Socket& Socket::operator=(Socket&& other) noexcept = default;

bool Socket::create() {
    close();
    impl_ = std::make_unique<Impl>();
    return true;
}

void Socket::close() {
    if (!impl_) {
        return;
    }

    boost::system::error_code ec;

    if (impl_->socket) {
        impl_->socket->cancel(ec);
        ec.clear();
        impl_->socket->close(ec);
    }

    ec.clear();
    if (impl_->acceptor) {
        impl_->acceptor->cancel(ec);
        ec.clear();
        impl_->acceptor->close(ec);
    }

    impl_.reset();
}

bool Socket::isValid() const {
    return impl_ && ((impl_->socket && impl_->socket->is_open()) ||
                     (impl_->acceptor && impl_->acceptor->is_open()));
}

bool Socket::bind(const std::string& address, uint16_t port) {
    if (!impl_) {
        return false;
    }

    boost::system::error_code ec;
    impl_->socket.reset();
    impl_->acceptor = std::make_unique<tcp::acceptor>(impl_->ioContext);

    const auto endpoint = makeEndpoint(address, port, ec);
    if (ec) {
        return false;
    }

    impl_->acceptor->open(endpoint.protocol(), ec);
    if (ec) {
        return false;
    }

    impl_->acceptor->set_option(boost::asio::socket_base::reuse_address(impl_->reuseAddr), ec);
    if (ec) {
        return false;
    }

    impl_->acceptor->non_blocking(impl_->nonBlocking, ec);
    if (ec) {
        return false;
    }

    impl_->acceptor->bind(endpoint, ec);
    return !ec;
}

bool Socket::listen(int backlog) {
    if (!impl_ || !impl_->acceptor) {
        return false;
    }

    boost::system::error_code ec;
    impl_->acceptor->listen(backlog, ec);
    return !ec;
}

Socket Socket::accept() {
    if (!impl_ || !impl_->acceptor || !impl_->acceptor->is_open()) {
        return Socket();
    }

    auto acceptedImpl = std::make_unique<Impl>();
    acceptedImpl->socket = std::make_unique<tcp::socket>(acceptedImpl->ioContext);
    acceptedImpl->timeoutMs = impl_->timeoutMs;
    acceptedImpl->nonBlocking = impl_->nonBlocking;
    acceptedImpl->reuseAddr = impl_->reuseAddr;

    boost::system::error_code ec;
    const bool ok = runTimedOperation(
        impl_->ioContext,
        impl_->timeoutMs,
        [&](auto&& handler) {
            impl_->acceptor->async_accept(
                *acceptedImpl->socket,
                [handler = std::forward<decltype(handler)>(handler)](const boost::system::error_code& acceptEc) mutable {
                    handler(acceptEc, 0);
                });
        },
        [&]() {
            boost::system::error_code ignoredEc;
            impl_->acceptor->cancel(ignoredEc);
        },
        ec);

    if (!ok) {
        boost::system::error_code ignoredEc;
        acceptedImpl->socket->close(ignoredEc);
        return Socket();
    }

    acceptedImpl->socket->non_blocking(acceptedImpl->nonBlocking, ec);
    if (ec) {
        boost::system::error_code ignoredEc;
        acceptedImpl->socket->close(ignoredEc);
        return Socket();
    }

    return Socket(std::move(acceptedImpl));
}

bool Socket::connect(const std::string& address, uint16_t port) {
    if (!impl_) {
        return false;
    }

    boost::system::error_code ec;
    impl_->acceptor.reset();
    impl_->socket = std::make_unique<tcp::socket>(impl_->ioContext);
    impl_->socket->open(tcp::v4(), ec);
    if (ec) {
        return false;
    }

    impl_->socket->non_blocking(impl_->nonBlocking, ec);
    if (ec) {
        return false;
    }

    tcp::resolver resolver(impl_->ioContext);
    const auto endpoints = resolver.resolve(address, std::to_string(port), ec);
    if (ec) {
        return false;
    }

    const bool ok = runTimedOperation(
        impl_->ioContext,
        impl_->timeoutMs,
        [&](auto&& handler) {
            boost::asio::async_connect(*impl_->socket, endpoints, [handler = std::forward<decltype(handler)>(handler)](
                const boost::system::error_code& connectEc,
                const tcp::endpoint&) mutable {
                handler(connectEc, 0);
            });
        },
        [&]() {
            boost::system::error_code ignoredEc;
            impl_->socket->cancel(ignoredEc);
            ignoredEc.clear();
            impl_->socket->close(ignoredEc);
        },
        ec);

    return ok;
}

int Socket::send(const uint8_t* data, std::size_t len) {
    if (!impl_ || !impl_->socket || !impl_->socket->is_open()) {
        return -1;
    }

    std::size_t transferred = 0;
    boost::system::error_code ec;
    const bool ok = runTimedOperation(
        impl_->ioContext,
        impl_->timeoutMs,
        [&](auto&& handler) {
            boost::asio::async_write(*impl_->socket, boost::asio::buffer(data, len), std::forward<decltype(handler)>(handler));
        },
        [&]() {
            boost::system::error_code ignoredEc;
            impl_->socket->cancel(ignoredEc);
        },
        ec,
        &transferred);

    return ok ? static_cast<int>(transferred) : -1;
}

int Socket::receive(uint8_t* buffer, std::size_t maxLen) {
    if (!impl_ || !impl_->socket || !impl_->socket->is_open()) {
        return -1;
    }

    std::size_t transferred = 0;
    boost::system::error_code ec;
    const bool ok = runTimedOperation(
        impl_->ioContext,
        impl_->timeoutMs,
        [&](auto&& handler) {
            impl_->socket->async_read_some(boost::asio::buffer(buffer, maxLen), std::forward<decltype(handler)>(handler));
        },
        [&]() {
            boost::system::error_code ignoredEc;
            impl_->socket->cancel(ignoredEc);
        },
        ec,
        &transferred);

    return ok ? static_cast<int>(transferred) : -1;
}

bool Socket::setReuseAddr(bool enable) {
    if (!impl_) {
        return false;
    }

    impl_->reuseAddr = enable;
    if (!impl_->acceptor || !impl_->acceptor->is_open()) {
        return true;
    }

    boost::system::error_code ec;
    impl_->acceptor->set_option(boost::asio::socket_base::reuse_address(enable), ec);
    return !ec;
}

bool Socket::setNonBlocking(bool enable) {
    if (!impl_) {
        return false;
    }

    impl_->nonBlocking = enable;
    boost::system::error_code ec;

    if (impl_->socket && impl_->socket->is_open()) {
        impl_->socket->non_blocking(enable, ec);
        if (ec) {
            return false;
        }
    }

    if (impl_->acceptor && impl_->acceptor->is_open()) {
        ec.clear();
        impl_->acceptor->non_blocking(enable, ec);
        if (ec) {
            return false;
        }
    }

    return true;
}

bool Socket::setTimeout(int timeoutMs) {
    if (!impl_) {
        return false;
    }

    impl_->timeoutMs = timeoutMs;
    return true;
}