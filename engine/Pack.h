#pragma once

#include <type_traits>

namespace ECSEngine {

namespace pack_detail {

// base case, empty list
template <typename... Args>
constexpr int packSize = 0;

// recursion step, non-empty list
template <typename Arg, typename... Args>
constexpr int packSize<Arg, Args...> = 1 + packSize<Args...>;

template <int QueryIndex, typename... Args>
struct _packIndexToType {
    // empty args and any index -- base case, type does not exist in pack
    static_assert(QueryIndex > 0 && QueryIndex < packSize<Args...>);
};

template <typename Arg, typename... Args>
struct _packIndexToType<0, Arg, Args...> {
    // non-empty args and zero index -- base case, just return Arg
    using type = Arg;
};

template <int QueryIndex, typename Arg, typename... Args>
struct _packIndexToType<QueryIndex, Arg, Args...> {
    // non-empty args and non-zero index -- iterate over on list, decrementing index.
    using type = typename _packIndexToType<QueryIndex - 1, Args...>::type;
};

// copied over alias
template <int QueryIndex, typename... Args>
using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

template <int Accumulated, typename QueryType, typename... Args>
constexpr int _packTypeToIndex = -1;

// Recursion case: check equality. If equal, return Accumulated, else recurse.
template <int Accumulated, typename QueryType, typename Arg, typename... Args>
constexpr int _packTypeToIndex<Accumulated, QueryType, Arg, Args...> =
    std::is_same_v<QueryType, Arg> ? Accumulated
                                   : _packTypeToIndex<Accumulated + 1, QueryType, Args...>;

template <typename QueryType, typename... Args>
constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;

}  // namespace pack_detail

template <typename... Args>
struct Pack {
    // (d)
    static constexpr int size = pack_detail::packSize<Args...>;

    // (e)
    template <typename QueryType>
    static constexpr int index = pack_detail::packTypeToIndex<QueryType, Args...>;

    // (f)
    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    // Trivial addition:
    template <typename QueryType>
    static constexpr bool contains = (index<QueryType> != -1);
};

}  // namespace ECSEngine
