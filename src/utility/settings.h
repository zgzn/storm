/*
 * settings.h
 *
 *  Created on: 22.11.2012
 *      Author: Gereon Kremer
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <iostream>
#include <list>
#include <boost/program_options.hpp>
#include "src/exceptions/InvalidSettings.h"

namespace mrmc {

/*!
 *	@brief Contains Settings class and associated methods.
 *
 *	The settings namespace contains the Settings class some friend methods like instance().
 */
namespace settings {
	
	namespace bpo = boost::program_options;
	/*!
	 *	@brief	Simple wrapper around boost::program_options to handle configuration options.
	 *
	 *	This class uses boost::program_options to read options from the
	 *	commandline and additionally load options from a file.
	 *
	 *	It is meant to be used as a singleton. Call 
	 *	@code mrmc::settings::Settings::instance(argc, argv, filename) @endcode
	 *	to initialize it and obtain an instance for the first time.
	 *	Afterwards, it is possible to use
	 *	@code mrmc::settings::instance() @endcode
	 */
	class Settings
	{
		public:
		
		/*!
		 *	@brief Get value of a generic option.
		 */
		template <typename T>
		const T& get(const std::string &name) const {
			return this->vm[name].as<T>();
		}
		
		/*!
		 *	@brief Get value of string option
		 */
		const std::string& getString(const std::string &name) const {
			return this->get<std::string>(name);
		}
		
		/*!
		 *	@brief Check if an option is set
		 */
		const bool isSet(const std::string &name) const {
			return this->vm.count(name) > 0;
		}
	
		friend std::ostream& help(std::ostream& os);
		friend std::ostream& helpConfigfile(std::ostream& os);
		friend Settings* instance();

		/*!
		 *	@brief Creates a new instance.
		 */
		static Settings* instance(const int argc, const char* argv[], const char* filename);

		private:
			/*!
			 *	@brief	Constructor.
			 */
			Settings(const int argc, const char* argv[], const char* filename);
		
			/*!
			 *	@brief Option descriptions.
			 */
			static bpo::options_description configfile;
			static bpo::options_description generic;
			static bpo::options_description commandline;
			static bpo::positional_options_description positional;
			
			/*!
			 *	@brief Collecting option descriptions.
			 *
			 *	The options for command line and config file are collected
			 *	here.
			 */
			static bpo::options_description cli;
			static bpo::options_description conf;
			
			/*!
			 *	@brief	option mapping.
			 */
			bpo::variables_map vm;
			
			/*!
			 *	@brief	actual instance of this class.
			 */
			static Settings* inst;
	};

	/*!
	 *	@brief	Print usage help.
	 */
	std::ostream& help(std::ostream& os);
	
	/*!
	 *	@brief	Print help for config file options.
	 */
	std::ostream& helpConfigfile(std::ostream& os);
	
	/*!
	 *	@brief	Return current instance.
	 */
	Settings* instance();


	/*!
	 *	@brief	Function type for functions registering new options.
	 */
	typedef void(*fptr_regOption)(bpo::options_description&);
	
	/*!
	 *	@brief	Function type for function checking constraints on settings.
	 */
	typedef bool(*fptr_checkOption)(bpo::variables_map&);
	
	
	/*!
	 *	@brief This class handles callbacks for registering new options and
	 *	checking constraints on them afterwards.
	 *
	 *	This class is also a singleton (like Settings) and is implemented much
	 *	simpler as we don't need any custom initialization code.
	 */
	class Callbacks
	{
		private:
			/*!
			 *	@brief Stores register callbacks.
			 */
			std::list<fptr_regOption> reg_list;
			/*!
			 *	@brief Stores check callbacks.
			 */
			std::list<fptr_checkOption> check_list;
			
			/*!
			 *	@brief Private constructor.
			 */
			Callbacks() {}
			/*!
			 *	@brief Private copy constructor.
			 */
			Callbacks(const Callbacks&) {}
			/*!
			 *	@brief Private destructor.
			 */
			~Callbacks() {}			
		public:
			/*!
			 *	@brief Returns current instance to create singleton.
			 *	@return current instance
			 */
			static Callbacks& getInstance()
			{
				static Callbacks instance;
				return instance;
			}
		
		/*!
		 *	@brief Register class needs access to lists.
		 */
		friend class Register;
	};
	
	/*!
	 *	@brief Wrapper class to allow for registering callbacks during
	 *	static initialization.
	 */
	class Register
	{
		public:
			/*!
			 *	@brief Registers given function as register callback.
			 */
			Register(const fptr_regOption ptr)
			{
				mrmc::settings::Callbacks::getInstance().reg_list.push_back(ptr);
			}
			
			/*!
			 *	@brief Registers given function as check callback.
			 */
			Register(const fptr_checkOption ptr)
			{
				mrmc::settings::Callbacks::getInstance().check_list.push_back(ptr);
			}
	};
	
} // namespace settings
} // namespace mrmc

#endif // SETTINGS_H_
