#ifndef MCTSPLAYER_H
#define MCTSPLAYER_H

#include "SimpleGoGame.h"
#include "MCTree.h"
#include "PlayOutSearcher.h"
#include "I_TreePolicy.h"

class MctsPlayer
{
public:
    explicit MctsPlayer(int board_size, boost::shared_ptr<GoReferee> l_referee);

    /**
     * All the search process starts here
     * Creates a Tree that will be used to extract the best move later.
    */
    void StartSearchFor(CompactBoard* cb, StoneState to_play);

    /**
     *
    */
    void PlayInTree(unsigned int count_sim, std::vector<short>& move_sequence);

    /**
     *
    */
    unsigned int ExpandTree();

    /**
     *
    */
    void UpdateSequenceInTree(double result);

    /**
     *
    */
    std::vector<short> GetBestMovesFound() ;


private:

/** Game used to perform the search for the best move.*/
SimpleGoGame m_go_game;

/** Tree Search*/
MCTree m_tree;

/** Entity responsible for performing the play-out simulation*/
PlayOutSearcher m_playout_searcher;

/** Tree policy used to select a Child in the "PlayInTree" phase.*/
I_TreePolicy* m_tree_policy;

/** Minimum number of simulations to be performed.*/
unsigned int m_minimum_simulations;

std::vector<short> fixed_moves;

};

#endif // MCTSPLAYER_H
