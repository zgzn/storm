#include "gtest/gtest.h"
#include "storm-config.h"
#include "src/parser/AutoParser.h"
#include "src/storage/MaximalEndComponentDecomposition.h"

TEST(MaximalEndComponentDecomposition, AsynchronousLeader) {
    storm::parser::AutoParser<double> parser(STORM_CPP_BASE_PATH "/examples/mdp/asynchronous_leader/leader7.tra", STORM_CPP_BASE_PATH "/examples/mdp/asynchronous_leader/leader7.lab", "", "");
	std::shared_ptr<storm::models::Mdp<double>> mdp = parser.getModel<storm::models::Mdp<double>>();
    
    storm::storage::MaximalEndComponentDecomposition<double> mecDecomposition;
    ASSERT_NO_THROW(mecDecomposition = storm::storage::MaximalEndComponentDecomposition<double>(*mdp));
    
    ASSERT_EQ(7, mecDecomposition.size());
    mdp = nullptr;
}

TEST(MaximalEndComponentDecomposition, Consensus) {
    storm::parser::AutoParser<double> parser(STORM_CPP_BASE_PATH "/examples/mdp/consensus/coin6_4.tra", STORM_CPP_BASE_PATH "/examples/mdp/consensus/coin6_4.lab", "", "");
	std::shared_ptr<storm::models::Mdp<double>> mdp = parser.getModel<storm::models::Mdp<double>>();
    
    storm::storage::MaximalEndComponentDecomposition<double> mecDecomposition;
    ASSERT_NO_THROW(mecDecomposition = storm::storage::MaximalEndComponentDecomposition<double>(*mdp));
    
    ASSERT_EQ(384, mecDecomposition.size());
    mdp = nullptr;
}