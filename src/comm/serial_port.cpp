#include "iloabot/comm/serial_port.h"

#include <boost/asio.hpp>

#include <chrono>
#include <memory>

namespace {

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
            timer.cancel(ignoredEc);
        }
    });

    ioContext.run();

    if (timedOut && ec == boost::asio::error::operation_aborted) {
        ec = boost::asio::error::timed_out;
    }

    return completed && !ec;
}

unsigned int toBaudRateValue(BaudRate baudRate) {
    switch (baudRate) {
        case BaudRate::Baud9600:
            return 9600;
        case BaudRate::Baud19200:
            return 19200;
        case BaudRate::Baud38400:
            return 38400;
        case BaudRate::Baud57600:
            return 57600;
        case BaudRate::Baud115200:
            return 115200;
    }

    return 115200;
}

boost::asio::serial_port_base::parity::type toParity(Parity parity) {
    switch (parity) {
        case Parity::Odd:
            return boost::asio::serial_port_base::parity::odd;
        case Parity::Even:
            return boost::asio::serial_port_base::parity::even;
        case Parity::None:
        default:
            return boost::asio::serial_port_base::parity::none;
    }
}

boost::asio::serial_port_base::stop_bits::type toStopBits(StopBits stopBits) {
    return stopBits == StopBits::Two
        ? boost::asio::serial_port_base::stop_bits::two
        : boost::asio::serial_port_base::stop_bits::one;
}

}  // namespace

struct SerialPort::Impl {
    boost::asio::io_context ioContext;
    boost::asio::serial_port serial{ioContext};
};

SerialPort::SerialPort(const SerialConfig& config) : config_(config) {}

SerialPort::~SerialPort() { close(); }

bool SerialPort::open() {
    close();

    impl_ = std::make_unique<Impl>();
    boost::system::error_code ec;
    impl_->serial.open(config_.device, ec);
    if (ec) {
        impl_.reset();
        return false;
    }

    if (!applyConfig()) {
        close();
        return false;
    }
    return true;
}

void SerialPort::close() {
    if (!impl_) {
        return;
    }

    boost::system::error_code ec;
    impl_->serial.cancel(ec);
    ec.clear();
    impl_->serial.close(ec);
    impl_.reset();
}

bool SerialPort::isOpen() const { return impl_ && impl_->serial.is_open(); }

int SerialPort::send(const uint8_t* data, std::size_t len) {
    if (!isOpen()) return -1;

    std::size_t transferred = 0;
    boost::system::error_code ec;
    const bool ok = runTimedOperation(
        impl_->ioContext,
        config_.timeoutMs,
        [&](auto&& handler) {
            boost::asio::async_write(impl_->serial, boost::asio::buffer(data, len), std::forward<decltype(handler)>(handler));
        },
        [&]() {
            boost::system::error_code ignoredEc;
            impl_->serial.cancel(ignoredEc);
        },
        ec,
        &transferred);

    return ok ? static_cast<int>(transferred) : -1;
}

int SerialPort::receive(uint8_t* buffer, std::size_t maxLen) {
    if (!isOpen()) return -1;

    std::size_t transferred = 0;
    boost::system::error_code ec;
    const bool ok = runTimedOperation(
        impl_->ioContext,
        config_.timeoutMs,
        [&](auto&& handler) {
            impl_->serial.async_read_some(boost::asio::buffer(buffer, maxLen), std::forward<decltype(handler)>(handler));
        },
        [&]() {
            boost::system::error_code ignoredEc;
            impl_->serial.cancel(ignoredEc);
        },
        ec,
        &transferred);

    return ok ? static_cast<int>(transferred) : -1;
}

void SerialPort::setConfig(const SerialConfig& config) {
    config_ = config;
}

const SerialConfig& SerialPort::config() const { return config_; }

bool SerialPort::applyConfig() {
    if (!isOpen()) {
        return false;
    }

    boost::system::error_code ec;
    impl_->serial.set_option(boost::asio::serial_port_base::baud_rate(toBaudRateValue(config_.baudRate)), ec);
    if (ec) {
        return false;
    }

    impl_->serial.set_option(
        boost::asio::serial_port_base::character_size(static_cast<unsigned int>(config_.dataBits)),
        ec);
    if (ec) {
        return false;
    }

    impl_->serial.set_option(boost::asio::serial_port_base::parity(toParity(config_.parity)), ec);
    if (ec) {
        return false;
    }

    impl_->serial.set_option(boost::asio::serial_port_base::stop_bits(toStopBits(config_.stopBits)), ec);
    if (ec) {
        return false;
    }

    impl_->serial.set_option(
        boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
        ec);

    return !ec;
}
