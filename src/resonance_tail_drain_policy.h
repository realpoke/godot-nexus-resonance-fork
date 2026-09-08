#ifndef RESONANCE_TAIL_DRAIN_POLICY_H
#define RESONANCE_TAIL_DRAIN_POLICY_H

#include <cmath>
#include <cstdint>

namespace resonance {

constexpr float kTailDrainSilenceEpsilon = 1.0e-5f;

/// IPL effect tail sizes must not gate this: they freeze > 0 once grace is spent (GetTail
/// stops being pulled) and would deadlock the voice release.
inline bool tail_drain_complete(bool output_ring_drained, bool reverb_ring_drained,
                                int64_t tail_grace_blocks_remaining) {
    return output_ring_drained && reverb_ring_drained && tail_grace_blocks_remaining == 0;
}

inline bool tail_grace_end_early(bool produced_any, bool output_ring_drained, bool reverb_ring_drained,
                                 bool last_out_valid, float last_out_l, float last_out_r) {
    if (produced_any || !output_ring_drained || !reverb_ring_drained || !last_out_valid)
        return false;
    return std::fabs(last_out_l) < kTailDrainSilenceEpsilon && std::fabs(last_out_r) < kTailDrainSilenceEpsilon;
}

} // namespace resonance

#endif // RESONANCE_TAIL_DRAIN_POLICY_H
