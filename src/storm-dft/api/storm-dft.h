#pragma once

#include <type_traits>

#include "storm-dft/parser/DFTGalileoParser.h"
#include "storm-dft/parser/DFTJsonParser.h"
#include "storm-dft/storage/dft/DftJsonExporter.h"

#include "storm-dft/modelchecker/dft/DFTModelChecker.h"
#include "storm-dft/modelchecker/dft/DFTASFChecker.h"

#include "storm-dft/transformations/DftToGspnTransformator.h"
#include "storm-gspn/api/storm-gspn.h"

namespace storm {
    namespace api {

        /*!
         * Load DFT from Galileo file.
         *
         * @param file File containing DFT description in Galileo format.
         *
         * @return DFT.
         */
        template<typename ValueType>
        std::shared_ptr<storm::storage::DFT<ValueType>> loadDFTGalileo(std::string const& file) {
             return std::make_shared<storm::storage::DFT<ValueType>>(storm::parser::DFTGalileoParser<ValueType>::parseDFT(file));
        }

        /*!
         * Load DFT from JSON file.
         *
         * @param file File containing DFT description in JSON format.
         *
         * @return DFT.
         */
        template<typename ValueType>
        std::shared_ptr<storm::storage::DFT<ValueType>> loadDFTJson(std::string const& file) {
                storm::parser::DFTJsonParser<ValueType> parser;
                return std::make_shared<storm::storage::DFT<ValueType>>(parser.parseJson(file));
        }

        /*!
         * Analyse the given DFT according to the given properties.
         * First the Markov model is built from the DFT and then this model is checked against the given properties.
         *
         * @param dft DFT.
         * @param properties PCTL formulas capturing the properties to check.
         * @param symred Flag whether symmetry reduction should be used.
         * @param allowModularisation Flag whether modularisation should be applied if possible.
         * @param enableDC Flag whether Don't Care propagation should be used.
         *
         * @return Result.
         */
        template <typename ValueType>
        typename storm::modelchecker::DFTModelChecker<ValueType>::dft_results analyzeDFT(storm::storage::DFT<ValueType> const& dft, std::vector<std::shared_ptr<storm::logic::Formula const>> const& properties, bool symred, bool allowModularisation, bool enableDC, bool printOutput) {
            storm::modelchecker::DFTModelChecker<ValueType> modelChecker;
            typename storm::modelchecker::DFTModelChecker<ValueType>::dft_results results = modelChecker.check(dft, properties, symred, allowModularisation, enableDC, 0.0);
            if (printOutput) {
                modelChecker.printTimings();
                modelChecker.printResults();
            }
            return results;
        }

        /*!
         * Approximate the analysis result of the given DFT according to the given properties.
         * First the Markov model is built from the DFT and then this model is checked against the given properties.
         *
         * @param dft DFT.
         * @param properties PCTL formulas capturing the properties to check.
         * @param symred Flag whether symmetry reduction should be used.
         * @param allowModularisation Flag whether modularisation should be applied if possible.
         * @param enableDC Flag whether Don't Care propagation should be used.
         * @param approximationError Allowed approximation error.
         *
         * @return Result.
         */
        template <typename ValueType>
        typename storm::modelchecker::DFTModelChecker<ValueType>::dft_results analyzeDFTApprox(storm::storage::DFT<ValueType> const& dft, std::vector<std::shared_ptr<storm::logic::Formula const>> const& properties, bool symred, bool allowModularisation, bool enableDC, double approximationError, bool printOutput) {
            storm::modelchecker::DFTModelChecker<ValueType> modelChecker;
            typename storm::modelchecker::DFTModelChecker<ValueType>::dft_results results = modelChecker.check(dft, properties, symred, allowModularisation, enableDC, approximationError);
            if (printOutput) {
                modelChecker.printTimings();
                modelChecker.printResults();
            }
            return results;
        }

        /*!
         * Export DFT to JSON file.
         *
         * @param dft DFT.
         * @param file File.
         */
        template<typename ValueType>
        void exportDFTToJson(storm::storage::DFT<ValueType> const& dft, std::string const& file);

        /*!
         * Export DFT to SMT encoding.
         *
         * @param dft DFT.
         * @param file File.
         */
        template<typename ValueType>
        void exportDFTToSMT(storm::storage::DFT<ValueType> const& dft, std::string const& file);

        /*!
         * Transform DFT to GSPN.
         *
         * @param dft DFT.
         */
        template<typename ValueType>
        void transformToGSPN(storm::storage::DFT<ValueType> const& dft);

    }
}
