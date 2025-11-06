#include <algorithm>
#include <gtest/gtest.h>
#include <set>
#include <sodium.h>

#include "domain/digest/DigestUtils.h"

using namespace kapa::tarracsh::domain::digestUtils;

namespace {

class SodiumTestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        ASSERT_GE(sodium_init(), 0);
    }
};

bool hasAnyNonZero(const DigestVector &digest) {
    return std::any_of(digest.begin(), digest.end(), [](unsigned char byte) { return byte != 0; });
}

} // namespace

TEST(DigestUtils, DigestOfEmptyVectorIsZeroed) {
    DigestVector input;
    const auto result = digest(input);
    EXPECT_EQ(result.size(), DIGEST_LENGTH);
    EXPECT_FALSE(hasAnyNonZero(result));
}

TEST(DigestUtils, DigestOfDataProducesBytes) {
    const std::vector<unsigned char> data{'t', 'e', 's', 't'};
    const auto result = digest(data);
    EXPECT_EQ(result.size(), DIGEST_LENGTH);
    EXPECT_TRUE(hasAnyNonZero(result));
}

TEST(DigestUtils, DigestSetAggregatesDigests) {
    const auto valueA = digest(reinterpret_cast<const char *>("alpha"), 5);
    const auto valueB = digest(reinterpret_cast<const char *>("beta"), 4);
    std::set<DigestVector> values{valueA, valueB};
    const auto aggregated = digestSet(values);
    EXPECT_EQ(aggregated.size(), DIGEST_LENGTH);
    EXPECT_TRUE(hasAnyNonZero(aggregated));
}

static ::testing::Environment *const sodium_environment =
    ::testing::AddGlobalTestEnvironment(new SodiumTestEnvironment());
