GBA Audio Mixing Demo

# Project Overview
- This program is an interactive demo of a software audio mixing system for the Game Boy Advance.
- It demonstrates playing background music and layering sample-based sound effects with real-time mixing.
- The demo uses a double-buffer system to seamlessly mix multiple audio sources into a single output stream.

## Core functionality:

- START button plays/stops a musical loop (rate cycling left/right, volume up/down)
- A button triggers sample A (with rate cycling via RIGHT shoulder)
- B button triggers sample B (with rate cycling via LEFT shoulder)
- Samples can play independently or mix together when no background song is playing
- Each sample has variants at 16kHz, 8kHz, and 4kHz sample rates
- Song plays in stereo FIFO_A and FIFO_B
- Mixed samples play mono in FIFO_A

## Required Assets:
-[x] Music loop (2 variants: 16kHz, 8kHz, 8-bit signed)
-[x] Kick sample (2 variants: 16kHz, 8kHz, 8-bit signed)
-[x] Snare sample (2 variants: 16kHz, 8kHz, 8-bit signed)

----------------------------------------------------------------

### Implementation Phases

## Phase 1:
    Hardware Audio Foundation

-[x] Direct DMA audio streaming (current stereo playback system)
-[x] Timer cascade setup (Timer 0 for sample rate, Timer 1 for buffer timing)
-[ ] Double buffer allocation and management
-[ ] Buffer swap interrupt handler

## Phase 2:
    Basic Software Mixing

-[ ] Implement mixing buffer system (256 samples)
-[ ] Create simple mixing function (clear buffer, add samples, handle overflow)
-[ ] Replace direct DMA streaming with mixed buffer streaming
-[ ] Verify clean audio output with single channel

## Phase 3:
    Sample Management System

-[ ] Audio channel data structures (position, increment, volume, active state)
-[ ] Sample triggering system (A/B button handlers)
-[ ] Sample playback with proper start/stop logic
-[ ] Fixed-point position tracking for smooth playback

## Phase 4:
    Sample Rate Conversion

-[ ] Multiple sample rate asset loading
-[ ] Rate cycling input handlers (LEFT/RIGHT shoulder buttons)
-[ ] Fixed-point increment calculation for different rates
-[ ] Sample rate conversion during mixing (interpolation if needed)

## Phase 5:
    Multi-Channel Mixing

-[ ] Simultaneous sample A + B playback
-[ ] Volume balancing to prevent overflow
-[ ] Channel priority system (background song vs samples)
-[ ] Clean mixing algorithm with proper bit depth handling

## Phase 6:
    Background Music Integration

-[ ] Background song playback system
-[ ] Song/sample mutual exclusion logic
-[ ] START button toggle for background music
-[ ] Seamless transitions between song and sample modes

-----------------------------------------------------------------------------
### Technical Specifications
Audio Format:
-   Output: 16kHz, 8-bit signed, mono
-   Buffer: 256 samples (16ms latency)
-   Channels: Up to 3 simultaneous (song OR samples A+B)
Timer Configuration:
-   Timer 0: 0xFBE8 (16kHz sample rate generation)
-   Timer 1: 0xFEFF (256-sample buffer timing)
-   Timer 1 cascade from Timer 0
Memory Layout: 
-   mixBufferA[256]: Primary mixing buffer
-   mixBufferB[256]: Secondary mixing buffer
-   Sample data arrays for each rate variant
Fixed-Point Format
-   Position: 20.12 format (20-bit integer, 12-bit fraction)
-   Increment: 12-bit fraction represents sub-sample advancement
-   4096 = 1.0 sample advancement per mix step
------------------------------------------------------------------------------
