#pragma once

#include <memory>

namespace kapa::infrastructure::log {
class Log;
}

namespace kapa::tarracsh::domain {
class Options;
namespace stats {
class Results;
}
} // namespace kapa::tarracsh::domain

namespace kapa::tarracsh::app::runtime::test {

void reset(const std::shared_ptr<kapa::infrastructure::log::Log> &log = nullptr);
kapa::tarracsh::domain::Options &options();
kapa::tarracsh::domain::stats::Results &results();

} // namespace kapa::tarracsh::app::runtime::test
