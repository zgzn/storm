#pragma once

#include "storm/modelchecker/AbstractModelChecker.h"
#include "storm/storage/dd/DdType.h"

namespace storm {
    namespace dd {
        template <storm::dd::DdType DdType>
        class Bdd;
    }
    
    namespace models {
        template <typename ValueType>
        class Model;
        
        namespace symbolic {
            template <storm::dd::DdType DdType, typename ValueType>
            class Model;
            
            template <storm::dd::DdType DdType, typename ValueType>
            class Dtmc;
            
            template <storm::dd::DdType DdType, typename ValueType>
            class Mdp;
            
            template <storm::dd::DdType DdType, typename ValueType>
            class StochasticTwoPlayerGame;
        }
    }
    
    namespace abstraction {
        class QualitativeResultMinMax;

        template <storm::dd::DdType DdType>
        class SymbolicQualitativeResultMinMax;
        
        template <storm::dd::DdType DdType>
        class QualitativeMdpResultMinMax;
        
        template <storm::dd::DdType DdType>
        class QualitativeGameResultMinMax;
        
        class StateSet;

        template <storm::dd::DdType DdType>
        class SymbolicStateSet;
    }
    
    namespace modelchecker {
        template <typename ModelType>
        class SymbolicMdpPrctlModelChecker;
        
        template <typename ValueType>
        class QuantitativeCheckResult;
        
        template<typename ModelType>
        class AbstractAbstractionRefinementModelChecker : public AbstractModelChecker<ModelType> {
        public:
            typedef typename ModelType::ValueType ValueType;
            static const storm::dd::DdType DdType = ModelType::DdType;
            
            /*!
             * Constructs a model checker for the given model.
             */
            explicit AbstractAbstractionRefinementModelChecker();
            
            ~AbstractAbstractionRefinementModelChecker();
            
            /// Overridden methods from super class.
            virtual bool canHandle(CheckTask<storm::logic::Formula> const& checkTask) const override;
            virtual std::unique_ptr<CheckResult> computeUntilProbabilities(CheckTask<storm::logic::UntilFormula, ValueType> const& checkTask) override;
            virtual std::unique_ptr<CheckResult> computeReachabilityProbabilities(CheckTask<storm::logic::EventuallyFormula, ValueType> const& checkTask) override;
            virtual std::unique_ptr<CheckResult> computeReachabilityRewards(storm::logic::RewardMeasureType rewardMeasureType, CheckTask<storm::logic::EventuallyFormula, ValueType> const& checkTask) override;
            
        protected:
            /// -------- Methods that need to be overwritten/defined by implementations in subclasses.
            
            /// Determines whether the model checker can handle reachability rewards or only reachability probabilities.
            virtual bool supportsReachabilityRewards() const;
            
            /// Retrieves the name of the underlying method.
            virtual std::string const& getName() const = 0;
            
            /// Called before the abstraction refinement loop to give the implementation a time to set up auxiliary data.
            virtual void initializeAbstractionRefinement() = 0;
            
            /// Retrieves the abstract model.
            virtual std::shared_ptr<storm::models::Model<ValueType>> getAbstractModel() = 0;
            
            /// Retrieves the state sets corresponding to the constraint and target states.
            virtual std::pair<std::unique_ptr<storm::abstraction::StateSet>, std::unique_ptr<storm::abstraction::StateSet>> getConstraintAndTargetStates(storm::models::Model<ValueType> const& abstractModel) = 0;
            
            /// Retrieves the index of the abstraction player. Must be in {0} for DTMCs, {1} for MDPs and in {1, 2} for games.
            virtual uint64_t getAbstractionPlayer() const = 0;
            
            /// Retrieves whether schedulers need to be computed.
            virtual bool requiresSchedulerSynthesis() const = 0;

            /// Refines the abstract model so that the next iteration obtains bounds that are at least as precise as
            /// current ones.
            virtual void refineAbstractModel() = 0;
            
            /// -------- Methods used to implement the abstraction refinement procedure.
            
            /// Performs the actual abstraction refinement loop.
            std::unique_ptr<CheckResult> performAbstractionRefinement();
            
            /// Computes lower and upper bounds on the abstract model and stores them in a member.
            void computeBounds(storm::models::Model<ValueType> const& abstractModel);
            
            /// Solves the current check task qualitatively, i.e. computes all states with probability 0/1.
            std::unique_ptr<storm::abstraction::QualitativeResultMinMax> computeQualitativeResult(storm::models::Model<ValueType> const& abstractModel, storm::abstraction::StateSet const& constraintStates, storm::abstraction::StateSet const& targetStates);
            std::unique_ptr<storm::abstraction::QualitativeResultMinMax> computeQualitativeResult(storm::models::symbolic::Model<DdType, ValueType> const& abstractModel, storm::abstraction::SymbolicStateSet<DdType> const& constraintStates, storm::abstraction::SymbolicStateSet<DdType> const& targetStates);
            std::unique_ptr<storm::abstraction::QualitativeResultMinMax> computeQualitativeResult(storm::models::symbolic::Dtmc<DdType, ValueType> const& abstractModel, storm::abstraction::SymbolicStateSet<DdType> const& constraintStates, storm::abstraction::SymbolicStateSet<DdType> const& targetStates);
            std::unique_ptr<storm::abstraction::QualitativeResultMinMax> computeQualitativeResult(storm::models::symbolic::Mdp<DdType, ValueType> const& abstractModel, storm::abstraction::SymbolicStateSet<DdType> const& constraintStates, storm::abstraction::SymbolicStateSet<DdType> const& targetStates);
            std::unique_ptr<storm::abstraction::QualitativeResultMinMax> computeQualitativeResult(storm::models::symbolic::StochasticTwoPlayerGame<DdType, ValueType> const& abstractModel, storm::abstraction::SymbolicStateSet<DdType> const& constraintStates, storm::abstraction::SymbolicStateSet<DdType> const& targetStates);
            std::unique_ptr<storm::abstraction::QualitativeGameResultMinMax<DdType>> computeQualitativeResultReuse(storm::models::symbolic::StochasticTwoPlayerGame<DdType, ValueType> const& abstractModel, storm::dd::Bdd<DdType> const& transitionMatrixBdd, storm::abstraction::SymbolicStateSet<DdType> const& constraintStates, storm::abstraction::SymbolicStateSet<DdType> const& targetStates, uint64_t abstractionPlayer, storm::OptimizationDirection const& modelNondeterminismDirection, bool requiresSchedulers);
            std::unique_ptr<CheckResult> checkForResultAfterQualitativeCheck(storm::models::Model<ValueType> const& abstractModel);
            std::unique_ptr<CheckResult> checkForResultAfterQualitativeCheck(storm::models::symbolic::Model<DdType, ValueType> const& abstractModel);
            
            // Methods related to the quantitative solution.
            bool skipQuantitativeSolution(storm::models::Model<ValueType> const& abstractModel);
            bool skipQuantitativeSolution(storm::models::symbolic::Model<DdType, ValueType> const& abstractModel);

            /// Tries to obtain the results from the bounds. If either of the two bounds is null, the result is assumed
            /// to be the non-null bound. If neither is null and the bounds are sufficiently close, the average of the
            /// bounds is returned.
            std::unique_ptr<CheckResult> tryToObtainResultFromBounds(storm::models::Model<ValueType> const& model, std::pair<std::unique_ptr<CheckResult>, std::unique_ptr<CheckResult>>& bounds);
            /// Checks whether the provided bounds are sufficiently close to terminate.
            bool boundsAreSufficientlyClose(std::pair<std::unique_ptr<CheckResult>, std::unique_ptr<CheckResult>> const& bounds);
            /// Retrieves the average of the two bounds. This should only be used to derive the overall result when the
            /// bounds are sufficiently close.
            std::unique_ptr<CheckResult> getAverageOfBounds(std::pair<std::unique_ptr<CheckResult>, std::unique_ptr<CheckResult>> const& bounds);
            
            /// Methods to set/get the check task that is currently handled.
            void setCheckTask(CheckTask<storm::logic::Formula, ValueType> const& checkTask);
            CheckTask<storm::logic::Formula, ValueType> const& getCheckTask() const;

            /// Methods that retrieve which results shall be reused.
            bool getReuseQualitativeResults() const;
            bool getReuseQuantitativeResults() const;

        private:
            /// The check task that is currently handled.
            std::unique_ptr<CheckTask<storm::logic::Formula, ValueType> const> checkTask;
            
            /// A flag indicating whether to reuse the qualitative results.
            bool reuseQualitativeResults;
            
            /// A flag indicating whether to reuse the quantitative results.
            bool reuseQuantitativeResults;
            
            /// The last qualitative results.
            std::unique_ptr<storm::abstraction::QualitativeResultMinMax> qualitativeResults;
            
            /// The last full result that was obtained.
            std::pair<std::unique_ptr<CheckResult>, std::unique_ptr<CheckResult>> bounds;
        };
    }
}
