/* afa.hh -- alternating finite automaton (over finite words)
 *
 * Copyright (c) TODO
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

#ifndef _MATA_AFA_HH_
#define _MATA_AFA_HH_

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// MATA headers
#include <mata/nfa.hh>
#include <mata/parser.hh>
#include <mata/util.hh>

namespace Mata
{
namespace Afa
{
extern const std::string TYPE_AFA;

// START OF THE DECLARATIONS

using State = Mata::Nfa::State;
using Symbol = Mata::Nfa::Symbol;

using SymbolToStringMap = Mata::Nfa::SymbolToStringMap;
using StateToStringMap = Mata::Nfa::StateToStringMap;
using StringToStateMap = Mata::Nfa::StringToStateMap;
using StringToSymbolMap = Mata::Nfa::StringToSymbolMap;

using Path = Mata::Nfa::Path;
using Word = Mata::Nfa::Word;

using StringDict = Mata::Nfa::StringDict;

using Alphabet = Mata::Nfa::Alphabet;

/// A transition
struct Trans
{
	State src;
  std::string formula;

	Trans() : src(), formula() { }
	Trans(State src, const std::string& formula) : src(src), formula(formula) { }

	bool operator==(const Trans& rhs) const
	{ // {{{
		return src == rhs.src && formula == rhs.formula;
	} // operator== }}}
	bool operator!=(const Trans& rhs) const { return !this->operator==(rhs); }
};


struct Afa;

/// serializes Afa into a ParsedSection
Mata::Parser::ParsedSection serialize(
	const Afa&                aut,
	const SymbolToStringMap*  symbol_map = nullptr,
	const StateToStringMap*   state_map = nullptr);


///  An AFA
struct Afa
{ // {{{
private:

public: // TODO: make private
  // TODO: transition relation
  std::vector<Trans> transitions;

public:

	std::set<State> initialstates = {};
	std::set<State> finalstates = {};

	void add_initial(State state) { this->initialstates.insert(state); }
	void add_initial(const std::vector<State> vec)
	{ // {{{
		for (const State& st : vec) { this->add_initial(st); }
	} // }}}
	bool has_initial(State state) const
	{ // {{{
		return Mata::util::haskey(this->initialstates, state);
	} // }}}
	void add_final(State state) { this->finalstates.insert(state); }
	void add_final(const std::vector<State> vec)
	{ // {{{
		for (const State& st : vec) { this->add_final(st); }
	} // }}}
	bool has_final(State state) const
	{ // {{{
		return Mata::util::haskey(this->finalstates, state);
	} // }}}

	void add_trans(const Trans& trans);
	void add_trans(State src, const std::string& formula)
	{ // {{{
		this->add_trans({src, formula});
	} // }}}

	bool has_trans(const Trans& trans) const;
	bool has_trans(State src, const std::string& formula) const
	{ // {{{
		return this->has_trans({src, formula});
	} // }}}

	bool trans_empty() const { /* TODO */ assert(false);};// no transitions
	size_t trans_size() const;/// number of transitions; has linear time complexity


}; // Afa }}}


/// a wrapper encapsulating @p Afa for higher-level use
struct AfaWrapper
{ // {{{
	/// the AFA
	Afa afa;

	/// the alphabet
	Alphabet* alphabet;

	/// mapping of state names (as strings) to their numerical values
	StringToStateMap state_dict;
}; // AfaWrapper }}}


/// Do the automata have disjoint sets of states?
bool are_state_disjoint(const Afa& lhs, const Afa& rhs);
/// Is the language of the automaton empty?
bool is_lang_empty(const Afa& aut, Path* cex = nullptr);
bool is_lang_empty_cex(const Afa& aut, Word* cex);

/// Retrieves the states reachable from initial states
std::unordered_set<State> get_fwd_reach_states(const Afa& aut);

/// Is the language of the automaton universal?
bool is_universal(
	const Afa&         aut,
	const Alphabet&    alphabet,
	Word*              cex = nullptr,
	const StringDict&  params = {{"algo", "antichains"}});

inline bool is_universal(
	const Afa&         aut,
	const Alphabet&    alphabet,
	const StringDict&  params)
{ // {{{
	return is_universal(aut, alphabet, nullptr, params);
} // }}}

/// Does the language of the automaton contain epsilon?
bool accepts_epsilon(const Afa& aut);

/// Checks inclusion of languages of two automata (smaller <= bigger)?
bool is_incl(
	const Afa&         smaller,
	const Afa&         bigger,
	const Alphabet&    alphabet,
	Word*              cex = nullptr,
	const StringDict&  params = {{"algo", "antichains"}});

inline bool is_incl(
	const Afa&         smaller,
	const Afa&         bigger,
	const Alphabet&    alphabet,
	const StringDict&  params)
{ // {{{
	return is_incl(smaller, bigger, alphabet, nullptr, params);
} // }}}

/// Compute union of a pair of automata
/// Assumes that sets of states of lhs, rhs, and result are disjoint
void union_norename(
	Afa*        result,
	const Afa&  lhs,
	const Afa&  rhs);

/// Compute union of a pair of automata
inline Afa union_norename(
	const Afa&  lhs,
	const Afa&  rhs)
{ // {{{
	Afa result;
	union_norename(&result, lhs, rhs);
	return result;
} // union_norename }}}

/// Compute union of a pair of automata
/// The states of the automata do not need to be disjoint; renaming will be done
Afa union_rename(
	const Afa&  lhs,
	const Afa&  rhs);


/// makes the transition relation complete
void make_complete(
	Afa*             aut,
	const Alphabet&  alphabet,
	State            sink_state);

/// Reverting the automaton
void revert(Afa* result, const Afa& aut);

inline Afa revert(const Afa& aut)
{ // {{{
	Afa result;
	revert(&result, aut);
	return result;
} // revert }}}

/// Removing epsilon transitions
void remove_epsilon(Afa* result, const Afa& aut, Symbol epsilon);

inline Afa remove_epsilon(const Afa& aut, Symbol epsilon)
{ // {{{
	Afa result;
	remove_epsilon(&result, aut, epsilon);
	return result;
} // }}}


/// Minimizes an AFA.  The method can be set using @p params
void minimize(
	Afa*               result,
	const Afa&         aut,
	const StringDict&  params = {});


inline Afa minimize(
	const Afa&         aut,
	const StringDict&  params = {})
{ // {{{
	Afa result;
	minimize(&result, aut, params);
	return result;
} // minimize }}}


/// Test whether an automaton is deterministic, i.e., whether it has exactly
/// one initial state and every state has at most one outgoing transition over
/// every symbol.  Checks the whole automaton, not only the reachable part
bool is_deterministic(const Afa& aut);

/// Test for automaton completeness wrt an alphabet.  An automaton is complete
/// if every reachable state has at least one outgoing transition over every
/// symbol.
bool is_complete(const Afa& aut, const Alphabet& alphabet);

/** Loads an automaton from Parsed object */
void construct(
	Afa*                                 aut,
	const Mata::Parser::ParsedSection&  parsec,
	StringToSymbolMap*                   symbol_map = nullptr,
	StringToStateMap*                    state_map = nullptr);

/** Loads an automaton from Parsed object */
inline Afa construct(
	const Mata::Parser::ParsedSection&  parsec,
	StringToSymbolMap*                   symbol_map = nullptr,
	StringToStateMap*                    state_map = nullptr)
{ // {{{
	Afa result;
	construct(&result, parsec, symbol_map, state_map);
	return result;
} // construct }}}

/** Loads an automaton from Parsed object */
void construct(
	Afa*                                 aut,
	const Mata::Parser::ParsedSection&  parsec,
	Alphabet*                            alphabet,
	StringToStateMap*                    state_map = nullptr);

/** Loads an automaton from Parsed object */
inline Afa construct(
	const Mata::Parser::ParsedSection&  parsec,
	Alphabet*                            alphabet,
	StringToStateMap*                    state_map = nullptr)
{ // {{{
	Afa result;
	construct(&result, parsec, alphabet, state_map);
	return result;
} // construct(Alphabet) }}}

/**
 * @brief  Obtains a word corresponding to a path in an automaton (or sets a flag)
 *
 * Returns a word that is consistent with @p path of states in automaton @p
 * aut, or sets a flag to @p false if such a word does not exist.  Note that
 * there may be several words with the same path (in case some pair of states
 * is connected by transitions over more than one symbol).
 *
 * @param[in]  aut   The automaton
 * @param[in]  path  The path of states
 *
 * @returns  A pair (word, bool) where if @p bool is @p true, then @p word is a
 *           word consistent with @p path, and if @p bool is @p false, this
 *           denotes that the path is invalid in @p aut
 */
std::pair<Word, bool> get_word_for_path(const Afa& aut, const Path& path);


/// Checks whether a string is in the language of an automaton
bool is_in_lang(const Afa& aut, const Word& word);

/// Checks whether the prefix of a string is in the language of an automaton
bool is_prfx_in_lang(const Afa& aut, const Word& word);

/** Encodes a vector of strings (each corresponding to one symbol) into a
 *  @c Word instance
 */
inline Word encode_word(
	const StringToSymbolMap&         symbol_map,
	const std::vector<std::string>&  input)
{ // {{{
	Word result;
	for (auto str : input) { result.push_back(symbol_map.at(str)); }
	return result;
} // encode_word }}}

/// operator<<
std::ostream& operator<<(std::ostream& os, const Afa& afa);

/// global constructor to be called at program startup (from vm-dispatch)
void init();

// CLOSING NAMESPACES AND GUARDS
} /* Afa */
} /* Mata */

namespace std
{ // {{{
template <>
struct hash<Mata::Afa::Trans>
{
	inline size_t operator()(const Mata::Afa::Trans& trans) const
	{
		size_t accum = std::hash<Mata::Afa::State>{}(trans.src);
		accum = Mata::util::hash_combine(accum, trans.formula);
		return accum;
	}
};

std::ostream& operator<<(std::ostream& os, const Mata::Afa::Trans& trans);
std::ostream& operator<<(std::ostream& os, const Mata::Afa::AfaWrapper& afa_wrap);
} // std }}}


#endif /* _MATA_AFA_HH_ */
