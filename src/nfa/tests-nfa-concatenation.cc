/* tests-nfa-concatenation.cc -- Tests for concatenation of NFAs
 *
 * Copyright (c) 2022 David Chocholatý <chocholaty.david@protonmail.com>
 *
 * This file is a part of libmata.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <unordered_set>

#include "../3rdparty/catch.hpp"

#include <mata/nfa.hh>

using namespace Mata::Nfa;
using namespace Mata::util;
using namespace Mata::Parser;

// Some common automata {{{

// Automaton A
#define FILL_WITH_AUT_A(x) \
    x.initialstates = {1, 3}; \
    x.finalstates = {5}; \
    x.add_trans(1, 'a', 3); \
    x.add_trans(1, 'a', 10); \
    x.add_trans(1, 'b', 7); \
    x.add_trans(3, 'a', 7); \
    x.add_trans(3, 'b', 9); \
    x.add_trans(9, 'a', 9); \
    x.add_trans(7, 'b', 1); \
    x.add_trans(7, 'a', 3); \
    x.add_trans(7, 'c', 3); \
    x.add_trans(10, 'a', 7); \
    x.add_trans(10, 'b', 7); \
    x.add_trans(10, 'c', 7); \
    x.add_trans(7, 'a', 5); \
    x.add_trans(5, 'a', 5); \
    x.add_trans(5, 'c', 9); \


// Automaton B
#define FILL_WITH_AUT_B(x) \
    x.initialstates = {4}; \
    x.finalstates = {2, 12}; \
    x.add_trans(4, 'c', 8); \
    x.add_trans(4, 'a', 8); \
    x.add_trans(8, 'b', 4); \
    x.add_trans(4, 'a', 6); \
    x.add_trans(4, 'b', 6); \
    x.add_trans(6, 'a', 2); \
    x.add_trans(2, 'b', 2); \
    x.add_trans(2, 'a', 0); \
    x.add_trans(0, 'a', 2); \
    x.add_trans(2, 'c', 12); \
    x.add_trans(12, 'a', 14); \
    x.add_trans(14, 'b', 12); \

// }}}

TEST_CASE("Mata::Nfa::concatenate()") {
    Nfa lhs{};
    Nfa rhs{};
    Nfa result{};

    SECTION("Empty automaton without states") {
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("One empty automaton without states") {
        rhs.increase_size(1);
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("Other empty automaton without states") {
        lhs.increase_size(1);
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("One empty automaton without states with other with initial states") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("Other empty automaton without states with other with initial states") {
        rhs.increase_size(1);
        rhs.make_initial(0);
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("One empty automaton without states with other non-empty automaton") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("Other empty automaton without states with other non-empty automaton") {
        rhs.increase_size(1);
        rhs.make_initial(0);
        rhs.make_final(0);
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("Empty automaton") {
        lhs.increase_size(1);
        rhs.increase_size(1);
        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("Empty language") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        rhs.increase_size(1);
        rhs.make_initial(0);

        result = concatenate(lhs, rhs);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
    }

    SECTION("Empty language rhs automaton") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(1);
        rhs.make_initial(0);

        result = concatenate(lhs, rhs);

        CHECK(result.has_initial(0));
        CHECK(result.finalstates.empty());
        CHECK(result.get_num_of_states() == 1);
        CHECK(result.trans_empty());
    }

    SECTION("Single state automata accepting an empty string") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(1);
        rhs.make_initial(0);
        rhs.make_final(0);

        result = concatenate(lhs, rhs);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(0));
        CHECK(result.get_num_of_states() == 1);
        CHECK(result.trans_empty());
    }

    SECTION("Empty language rhs automaton") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(2);
        rhs.make_initial(0);
        rhs.make_final(1);

        result = concatenate(lhs, rhs);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(1));
        CHECK(result.get_num_of_states() == 2);
        CHECK(result.trans_empty());
    }

    SECTION("Simple two state rhs automaton") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(2);
        rhs.make_initial(0);
        rhs.make_final(1);
        rhs.add_trans(0, 'a', 1);

        result = concatenate(lhs, rhs);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(1));
        CHECK(result.get_num_of_states() == 2);
        CHECK(result.has_trans(0, 'a', 1));
    }

    SECTION("Simple two state automata") {
        lhs.increase_size(2);
        lhs.make_initial(0);
        lhs.make_final(1);
        lhs.add_trans(0, 'b', 1);
        rhs.increase_size(2);
        rhs.make_initial(0);
        rhs.make_final(1);
        rhs.add_trans(0, 'a', 1);

        result = concatenate(lhs, rhs);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(2));
        CHECK(result.get_num_of_states() == 3);
        CHECK(result.has_trans(0, 'b', 1));
        CHECK(result.has_trans(1, 'a', 2));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 1);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a' }) != shortest_words.end());
    }

    SECTION("Simple two state automata with higher state num for non-final state") {
        lhs.increase_size(2);
        lhs.make_initial(0);
        lhs.make_final(1);
        lhs.add_trans(0, 'b', 1);
        rhs.increase_size(4);
        rhs.make_initial(0);
        rhs.make_final(1);
        rhs.add_trans(0, 'a', 1);
        rhs.add_trans(0, 'c', 3);

        result = concatenate(lhs, rhs);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(2));
        CHECK(result.get_num_of_states() == 5);
        CHECK(result.has_trans(0, 'b', 1));
        CHECK(result.has_trans(1, 'a', 2));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 1);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a' }) != shortest_words.end());
    }

    SECTION("Simple two state lhs automaton") {
        lhs.increase_size(2);
        lhs.make_initial(0);
        lhs.make_final(1);
        lhs.add_trans(0, 'b', 1);
        rhs.increase_size(1);
        rhs.make_initial(0);
        rhs.make_final(0);
        rhs.add_trans(0, 'a', 0);

        result = concatenate(lhs, rhs);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(1));
        CHECK(result.get_num_of_states() == 2);
        CHECK(result.has_trans(0, 'b', 1));
        CHECK(result.has_trans(1, 'a', 1));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 1);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b' }) != shortest_words.end());
    }

    SECTION("Automaton A concatenate automaton B") {
        lhs.increase_size_for_state(10);
        FILL_WITH_AUT_A(lhs);
        rhs.increase_size_for_state(14);
        FILL_WITH_AUT_B(rhs);

        result = concatenate(lhs, rhs);

        CHECK(result.initialstates.size() == 2);
        CHECK(result.has_initial(1));
        CHECK(result.has_initial(3));

        CHECK(result.get_num_of_states() == 25);

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 4);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', 'b', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', 'b', 'a' }) != shortest_words.end());
    }

    SECTION("Automaton B concatenate automaton A") {
        lhs.increase_size_for_state(10);
        FILL_WITH_AUT_A(lhs);
        rhs.increase_size_for_state(14);
        FILL_WITH_AUT_B(rhs);

        result = concatenate(rhs, lhs);

        CHECK(result.get_num_of_states() == 24);

        CHECK(result.initialstates.size() == 1);
        // Final state 2 in automaton B will not stay in the result automaton.
        // Hence, initial state 4 in aut B will be initial state 3 in the result.
        CHECK(result.has_initial(3));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 4);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', 'b', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', 'b', 'a' }) != shortest_words.end());
    }
}

TEST_CASE("Mata::Nfa::concatenate() over epsilon symbol") {
    Nfa lhs{};
    Nfa rhs{};
    Nfa result{};
    Symbol epsilon{ 'x' };

    SECTION("Empty automaton") {
        lhs.increase_size(1);
        rhs.increase_size(1);
        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
        CHECK(is_lang_empty(result));
    }

    SECTION("Empty language") {
        lhs.increase_size(1);
        lhs.make_initial(0);
        rhs.increase_size(1);
        rhs.make_initial(0);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.get_num_of_states() == 0);
        CHECK(result.initialstates.empty());
        CHECK(result.finalstates.empty());
        CHECK(result.trans_empty());
    }

    SECTION("Empty language rhs automaton")
    {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(1);
        rhs.make_initial(0);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.has_initial(0));
        CHECK(result.finalstates.empty());
        CHECK(result.get_num_of_states() == 2);
        CHECK(result.get_num_of_trans() == 1);
        CHECK(result.has_trans(0, epsilon, 1));
    }

    SECTION("Single state automata accepting an empty string")
    {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(1);
        rhs.make_initial(0);
        rhs.make_final(0);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(1));
        CHECK(result.get_num_of_states() == 2);
        CHECK(result.get_num_of_trans() == 1);
        CHECK(result.has_trans(0, epsilon, 1));
    }

    SECTION("Empty language rhs automaton")
    {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(2);
        rhs.make_initial(0);
        rhs.make_final(1);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(2));
        CHECK(result.get_num_of_states() == 3);
        CHECK(result.get_num_of_trans() == 1);
        CHECK(result.has_trans(0, epsilon, 1));
    }

    SECTION("Simple two state rhs automaton")
    {
        lhs.increase_size(1);
        lhs.make_initial(0);
        lhs.make_final(0);
        rhs.increase_size(2);
        rhs.make_initial(0);
        rhs.make_final(1);
        rhs.add_trans(0, 'a', 1);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(2));
        CHECK(result.get_num_of_states() == 3);
        CHECK(result.get_num_of_trans() == 2);
        CHECK(result.has_trans(1, 'a', 2));
        CHECK(result.has_trans(0, epsilon, 1));
    }

    SECTION("Simple two state automata")
    {
        lhs.increase_size(2);
        lhs.make_initial(0);
        lhs.make_final(1);
        lhs.add_trans(0, 'b', 1);
        rhs.increase_size(2);
        rhs.make_initial(0);
        rhs.make_final(1);
        rhs.add_trans(0, 'a', 1);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(3));
        CHECK(result.get_num_of_states() == 4);
        CHECK(result.get_num_of_trans() == 3);
        CHECK(result.has_trans(0, 'b', 1));
        CHECK(result.has_trans(2, 'a', 3));
        CHECK(result.has_trans(1, epsilon, 2));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 1);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', epsilon, 'a' }) != shortest_words.end());
    }

    SECTION("Simple two state automata with higher state num for non-final state")
    {
        lhs.increase_size(2);
        lhs.make_initial(0);
        lhs.make_final(1);
        lhs.add_trans(0, 'b', 1);
        rhs.increase_size(4);
        rhs.make_initial(0);
        rhs.make_final(1);
        rhs.add_trans(0, 'a', 1);
        rhs.add_trans(0, 'c', 3);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(3));
        CHECK(result.get_num_of_states() == 6);
        CHECK(result.get_num_of_trans() == 4);
        CHECK(result.has_trans(0, 'b', 1));
        CHECK(result.has_trans(2, 'a', 3));
        CHECK(result.has_trans(2, 'c', 5));
        CHECK(result.has_trans(1, epsilon, 2));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 1);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', epsilon, 'a' }) != shortest_words.end());
    }

    SECTION("Simple two state lhs automaton")
    {
        lhs.increase_size(2);
        lhs.make_initial(0);
        lhs.make_final(1);
        lhs.add_trans(0, 'b', 1);
        rhs.increase_size(1);
        rhs.make_initial(0);
        rhs.make_final(0);
        rhs.add_trans(0, 'a', 0);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.has_initial(0));
        CHECK(result.has_final(2));
        CHECK(result.get_num_of_states() == 3);
        CHECK(result.get_num_of_trans() == 3);
        CHECK(result.has_trans(0, 'b', 1));
        CHECK(result.has_trans(2, 'a', 2));
        CHECK(result.has_trans(1, epsilon, 2));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 1);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', epsilon }) != shortest_words.end());
    }

    SECTION("Automaton A concatenate automaton B")
    {
        lhs.increase_size_for_state(10);
        FILL_WITH_AUT_A(lhs);
        rhs.increase_size_for_state(14);
        FILL_WITH_AUT_B(rhs);

        result = concatenate(lhs, rhs, epsilon);

        CHECK(result.initialstates.size() == 2);
        CHECK(result.has_initial(1));
        CHECK(result.has_initial(3));

        CHECK(result.get_num_of_states() == 26);

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 4);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', epsilon, 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', epsilon, 'b', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', epsilon, 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', epsilon, 'b', 'a' }) != shortest_words.end());
    }

    SECTION("Automaton B concatenate automaton A")
    {
        lhs.increase_size_for_state(10);
        FILL_WITH_AUT_A(lhs);
        rhs.increase_size_for_state(14);
        FILL_WITH_AUT_B(rhs);

        result = concatenate(rhs, lhs, epsilon);

        CHECK(result.get_num_of_states() == 26);

        CHECK(result.initialstates.size() == 1);
        CHECK(result.has_initial(4));

        auto shortest_words{ result.get_shortest_words() };
        CHECK(shortest_words.size() == 4);
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', epsilon, 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'b', 'a', epsilon, 'b', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', epsilon, 'a', 'a' }) != shortest_words.end());
        CHECK(shortest_words.find(std::vector<Symbol>{ 'a', 'a', epsilon, 'b', 'a' }) != shortest_words.end());
    }
}
