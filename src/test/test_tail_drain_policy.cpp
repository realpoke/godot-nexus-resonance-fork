#include "../lib/catch2/single_include/catch2/catch.hpp"
#include "../resonance_tail_drain_policy.h"

using namespace resonance;

TEST_CASE("tail drain completes when rings are drained and grace is spent", "[playback][tail-drain]") {
    REQUIRE(tail_drain_complete(true, true, 0));
}

TEST_CASE("tail drain does not require IPL effect tail sizes (voice-release regression)", "[playback][tail-drain]") {
    REQUIRE(tail_drain_complete(true, true, 0));
}

TEST_CASE("tail drain blocked while output ring still has queued frames", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_drain_complete(false, true, 0));
}

TEST_CASE("tail drain blocked while split-reverb ring still has data for the reverb child", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_drain_complete(true, false, 0));
}

TEST_CASE("tail drain blocked while grace budget still counts", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_drain_complete(true, true, 1));
    REQUIRE_FALSE(tail_drain_complete(true, true, 129));
}

TEST_CASE("tail drain blocked while grace is not armed yet", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_drain_complete(true, true, -1));
}

TEST_CASE("grace early end: silence with nothing produced or queued", "[playback][tail-drain]") {
    REQUIRE(tail_grace_end_early(false, true, true, true, 0.0f, 0.0f));
    REQUIRE(tail_grace_end_early(false, true, true, true, 3.0e-6f, -4.0e-6f));
}

TEST_CASE("grace early end: blocked by produced tail block", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_grace_end_early(true, true, true, true, 0.0f, 0.0f));
}

TEST_CASE("grace early end: blocked by queued output or reverb frames", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_grace_end_early(false, false, true, true, 0.0f, 0.0f));
    REQUIRE_FALSE(tail_grace_end_early(false, true, false, true, 0.0f, 0.0f));
}

TEST_CASE("grace early end: blocked by audibly loud last sample", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_grace_end_early(false, true, true, true, 0.2f, 0.0f));
    REQUIRE_FALSE(tail_grace_end_early(false, true, true, true, 0.0f, -0.2f));
    REQUIRE_FALSE(tail_grace_end_early(false, true, true, true, kTailDrainSilenceEpsilon, 0.0f));
}

TEST_CASE("grace early end: blocked when last sample not yet valid", "[playback][tail-drain]") {
    REQUIRE_FALSE(tail_grace_end_early(false, true, true, false, 0.0f, 0.0f));
}
