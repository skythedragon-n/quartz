
#pragma once

template<typename... Tp>
struct overloads : Tp... { using Tp::operator()...; };
template<typename... Tp>
overloads(Tp...) -> overloads<Tp...>;
