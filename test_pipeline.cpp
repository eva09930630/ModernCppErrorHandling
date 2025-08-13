#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "pipeline.cpp"

TEST_CASE("ConfigReadError is propagated through and_then chain") {
    const std::string missing = "non_existent_file.txt";

    auto result = LoadConfig(missing)
                    .and_then(ValidateData)
                    .and_then(ProcessData);

    REQUIRE_FALSE(result.has_value());

    bool matched = false;
    std::visit(
        Overloaded{
            [&](const ConfigReadError& e) {
                matched = true;
                REQUIRE(e.filename == missing);
            },
            [](const auto&) {
                FAIL("Expected ConfigReadError but got a different error type");
            }
        },
        result.error()
    );
    REQUIRE(matched);
}