#ifndef TARRACSH_APP_RUNTIME_H
#define TARRACSH_APP_RUNTIME_H

#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "app/Context.h"

namespace kapa::tarracsh::app {
class TarracshCli;
}

namespace kapa::tarracsh::app::runtime {

domain::stats::Results &global_results();
domain::Options &global_options();
Context &context();
TarracshCli &app();

}

#endif
