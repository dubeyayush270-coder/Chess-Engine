#pragma once

#pragma once

enum class GameEndReason
{
    NONE,
    CHECKMATE,
    STALEMATE,
    INSUFFICIENT_MATERIAL,
    FIFTY_MOVE_RULE,
    THREEFOLD_REPETITION
};