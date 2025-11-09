#include <gtest/gtest.h>

#include "domain/Utils.h"

using namespace kapa::tarracsh::domain::utils;

TEST(UtilsTests, BuildsStrongClassnameWithJarAndClass) {
    const auto strong = getStrongClassname("foo.jar", "pkg/Clazz");
    EXPECT_EQ(strong, "foo.jar@pkg/Clazz");
}

TEST(UtilsTests, HandlesMissingClassname) {
    const auto strong = getStrongClassname("foo.jar", "");
    EXPECT_EQ(strong, "foo.jar");
}

TEST(UtilsTests, SplitsStrongClassnameIntoParts) {
    const auto parts = splitStrongClassname("foo.jar@pkg/Clazz");
    ASSERT_EQ(parts.size(), 2u);
    EXPECT_EQ(parts[0], "foo.jar");
    EXPECT_EQ(parts[1], "pkg/Clazz");
}
