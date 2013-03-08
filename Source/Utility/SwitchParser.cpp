/*
 *   Copyright (C) 2010-2013 Manuel Riecke <m.riecke@mrbeast.org>
 *
 *   This software is provided 'as-is', without any express or implied
 *   warranty.  In no event will the authors be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 */

#include "SwitchParser.h"

namespace Blackguard
{
	namespace Utility
	{
		void SwitchParser::parseArgv(int argc, char** argv)
		{
			bool isAtBeginning=true;
			AbstractSwitch* lastSwitch=NULL;

			for(int i=1; i<argc; i++)
			{
				string arg=argv[i];

				if(arg[0]=='-')
				{
					// Long name
					if(arg[1]=='-')
					{
						int equalSign=arg.find_first_of('=');

						if(equalSign != string::npos)
						{
							try
							{
								lastSwitch=mSwitches.at(arg.substr(2, equalSign-2));
								lastSwitch->set();
								string value=arg.substr(equalSign+1);

								if(lastSwitch->acceptsMoreArguments())
									lastSwitch->giveArgument(value);
								else;

								// TODO Bad syntax
							}
							catch(std::exception&) { }; // TODO Bad syntax
						}
						else
						{
							lastSwitch=mSwitches[arg.substr(2)];
							lastSwitch->set();
						}
					}
					// Short name
					else
					{
						int equalSign=arg.find_first_of('=');
						string switchs;

						if(equalSign != -1)
							switchs=arg.substr(1, equalSign-1);
						else
							switchs=arg.substr(1);

						for(const char c : switchs)
						{
							try
							{
								lastSwitch=mSwitchesShortName.at(c);
								lastSwitch->set();
							}
							catch(std::exception&) { }; // TODO Bad syntax
						}

						if(equalSign != -1)
						{
							if(lastSwitch->acceptsMoreArguments())
								lastSwitch->giveArgument(arg.substr(equalSign+1));
							else
								; // TODO Bad syntax
						}
					}
				}
				// Value
				else
				{
					if(lastSwitch && lastSwitch->acceptsMoreArguments())
						lastSwitch->giveArgument(arg);
					else if(mAnonymousSwitch->acceptsMoreArguments())
						mAnonymousSwitch->giveArgument(arg);
					else
						; // TODO Bad syntax
				}
			}
		}

		bool MultiSwitch::acceptsMoreArguments()
		{
			if(maxNumOfArguments < 0) return true;
			else return arguments.size() < (unsigned int)maxNumOfArguments;
		}

		unsigned int MultiSwitch::argumentCount()
		{
			return arguments.size();
		}

		void MultiSwitch::setMaxNumOfArguments(int number)
		{
			maxNumOfArguments = number;
		}

		void MultiSwitch::giveArgument(std::string argument)
		{
			this->arguments.push_back(argument);
		}

		MultiSwitch::MultiSwitch(int numArgs)
		{
			maxNumOfArguments = numArgs;
		}

		void SwitchParser::registerSwitch(AbstractSwitch& argSwitch, const std::string& longName, char shortName)
		{
			mSwitchesShortName[shortName] = mSwitches[longName] = &argSwitch;
		}

		void SwitchParser::registerSwitch(AbstractSwitch& argSwitch, const std::string& longName)
		{
		  mSwitches[longName] = &argSwitch;
		}

		void SwitchParser::setAnonymousSwitch(AbstractSwitch& argSwitch)
		{
			mAnonymousSwitch = &argSwitch;
		}

		BooleanSwitch::BooleanSwitch()
		{
			isSet = false;
		}

		void BooleanSwitch::set(bool set)
		{
			isSet = set;
		}

		bool BooleanSwitch::acceptsMoreArguments()
		{
			return false;
		}

		bool SimpleSwitch::acceptsMoreArguments()
		{
			return argument.empty();
		}

		void SimpleSwitch::giveArgument(string argument)
		{
			this->argument = argument;
		}
	}
}
