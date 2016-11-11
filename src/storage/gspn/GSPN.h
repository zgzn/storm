#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "src/storage/gspn/ImmediateTransition.h"
#include "src/storage/gspn/Marking.h"
#include "src/storage/gspn/Place.h"
#include "src/storage/gspn/TimedTransition.h"

namespace storm {
    namespace gspn {
        // Stores a GSPN
        class GSPN {
        public:
            static uint64_t timedTransitionIdToTransitionId(uint64_t);
            static uint64_t immediateTransitionIdToTransitionId(uint64_t);
            static uint64_t transitionIdToTimedTransitionId(uint64_t);
            static uint64_t transitionIdToImmediateTransitionId(uint64_t);
            
            
            // Later, the rates and probabilities type should become a template, for now, let it be doubles.
            typedef double RateType;
            typedef double WeightType;
            
            
            GSPN(std::string const& name, std::vector<Place> const& places, std::vector<ImmediateTransition<WeightType>> const& itransitions, std::vector<TimedTransition<RateType>> const& ttransitions);
            
            /*!
             * Returns the number of places in this gspn.
             *
             * @return The number of places.
             */
            uint_fast64_t getNumberOfPlaces() const;

            /*!
             * Returns the vector of timed transitions in this gspn.
             *
             * @return The vector of timed transitions.
             */
            std::vector<TimedTransition<GSPN::RateType>> const& getTimedTransitions() const;

            /*!
             * Returns the vector of immediate transitions in this gspn.
             *
             * @return The vector of immediate tansitions.
             */
            std::vector<ImmediateTransition<GSPN::WeightType>> const& getImmediateTransitions() const;

            /*!
             * Returns the places of this gspn
             */
            std::vector<storm::gspn::Place> const& getPlaces() const;

            /*
             * Computes the initial marking of the gspn.
             *
             * @param map The Map determines the number of bits for each place.
             * @return The initial Marking
             */
            std::shared_ptr<storm::gspn::Marking> getInitialMarking(std::map<uint_fast64_t, uint_fast64_t>& numberOfBits, uint_fast64_t const& numberOfTotalBits) const;

            /*!
             * Returns the place with the corresponding id.
             *
             * @param id The ID of the place.
             * @return The first element is true if the place was found.
             *         If the first element is true, then the second element is the wanted place.
             *         If the first element is false, then the second element is not defined.
             */
            storm::gspn::Place const* getPlace(uint64_t id) const;
            storm::gspn::Place const* getPlace(std::string const& name) const;
            
            /*!
             * Returns the timed transition with the corresponding id.
             *
             * @param id The ID of the timed transition.
             */
            storm::gspn::TimedTransition<GSPN::RateType> const* getTimedTransition(std::string const& id) const;

            /*!
             * Returns the immediate transition with the corresponding id.
             *
             * @param id The ID of the timed transition.
             *         If the first element is false, then the second element is the nullptr.
             */
            storm::gspn::ImmediateTransition<GSPN::WeightType> const* getImmediateTransition(std::string const& id) const;

            /*!
             * Returns the transition with the corresponding id.
             *
             */
            storm::gspn::Transition const* getTransition(std::string const& id) const;

            /*!
             * Write the gspn in a dot(graphviz) configuration.
             *
             * @param outStream The stream to which the output is written to.
             */
            void writeDotToStream(std::ostream& outStream) const;

            /*!
             * Set the name of the gspn to the given name.
             *
             * @param name The new name.
             */
            void setName(std::string const& name);

            /*!
             * Returns the name of the gspn.
             *
             * @return The name.
             */
            std::string const& getName() const;
            
            
            /**
             *  Set Capacities according to name->capacity map.
             */
            void setCapacities(std::unordered_map<std::string, uint64_t> const& mapping);

            /*!
             * Performe some checks
             * - testPlaces()
             * - testTransitions()
             *
             * @return true if no errors are found
             */
            bool isValid() const;
            // TODO doc
            void toPnpro(std::ostream &stream) const;
            // TODO doc
            void toPnml(std::ostream &stream) const;
        private:
            storm::gspn::Place* getPlace(uint64_t id);
            storm::gspn::Place* getPlace(std::string const& name);
            
            /*!
             * Test
             *  - if places are unique (ids and names)
             *  - if the capacity is greater than the number of initial tokens
             *
             * @return true if no errors found
             */
            bool testPlaces() const;

            /*!
             * Test
             * - if transition have at least on input/inhibitor and one output place
             *
             * @return true if no errors found
             */
            bool testTransitions() const;

            
            // set containing all places
            std::vector<storm::gspn::Place> places;
            
            // set containing all immediate transitions
            std::vector<storm::gspn::ImmediateTransition<WeightType>> immediateTransitions;

            // set containing all timed transitions
            std::vector<storm::gspn::TimedTransition<RateType>> timedTransitions;


            // name of the gspn
            std::string name;
        };
    }
}