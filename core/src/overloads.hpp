
#pragma once

#include <variant>

template<typename... Tp>
struct Overloads : Tp... { using Tp::operator()...; };
template<typename... Tp>
Overloads(Tp...) -> Overloads<Tp...>;

namespace qtil {
    template<typename Variant, typename... Tp>
    decltype(auto) match(Variant&& var, Tp&&... overloads) {
        return ::std::visit(
            Overloads{::std::forward<Tp>(overloads)...},
            ::std::forward<Variant>(var));
    }

    template<typename Variant, typename... Tp>
    decltype(auto) match_overloads(Variant&& var, Overloads<Tp...> overloads) {
        return ::std::visit(::std::move(overloads), ::std::forward<Variant>(var));
    }
}
